/* Code::Blocks-style C++ Web Debugger (GDB/MI backend) */
(function (global) {
  "use strict";

  function escHtml(s) {
    if (s == null) return "";
    return String(s)
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;");
  }

  function createCppDebugger(cm, opts) {
    const apiPost = opts.apiPost;
    const getStdin = opts.getStdin;
    const onShowDebugTab = opts.onShowDebugTab;

    const dbg = {
      sessionId: null,
      status: "idle", // idle | busy | paused | exited
      breakpoints: new Set(), // 1-based line numbers
      currentLine: null, // 1-based line number
      watches: [],
      prevValues: Object.create(null),
      activeFrame: 0,
      dirtyWhilePaused: false,
    };

    try {
      const saved = JSON.parse(localStorage.getItem("ttud_dbg_watches") || "[]");
      if (Array.isArray(saved)) dbg.watches = saved.filter((x) => typeof x === "string" && x.trim());
    } catch (_) {}

    const els = {
      toolbar: document.getElementById("debug-toolbar"),
      launchBtn: document.getElementById("btn-debug-launch"),
      startBtn: document.getElementById("dbg-btn-start"),
      startLabel: document.getElementById("dbg-start-label"),
      nextBtn: document.getElementById("dbg-btn-next"),
      stepBtn: document.getElementById("dbg-btn-step"),
      finishBtn: document.getElementById("dbg-btn-finish"),
      untilBtn: document.getElementById("dbg-btn-until"),
      stopBtn: document.getElementById("dbg-btn-stop"),
      bpBtn: document.getElementById("dbg-btn-bp"),
      clearBpBtn: document.getElementById("dbg-btn-clear-bp"),
      statusPill: document.getElementById("dbg-status-pill"),
      tabBadge: document.getElementById("debug-tab-badge"),
      watchInput: document.getElementById("dbg-watch-input"),
      watchAddBtn: document.getElementById("dbg-watch-add"),
      watchesBody: document.getElementById("dbg-watches-body"),
      stackList: document.getElementById("dbg-stack-list"),
      stdoutPre: document.getElementById("dbg-stdout-pre"),
      logPre: document.getElementById("dbg-log-pre"),
      judgePanel: document.getElementById("judge-panel"),
    };

    function saveWatches() {
      try {
        localStorage.setItem("ttud_dbg_watches", JSON.stringify(dbg.watches));
      } catch (_) {}
    }

    function setSideTab(name) {
      document.querySelectorAll(".dbg-side-tab").forEach((b) => {
        b.classList.toggle("active", b.dataset.dtab === name);
      });
      document.querySelectorAll(".dbg-side-pane-content").forEach((p) => {
        p.classList.toggle("active", p.id === `dtab-${name}`);
      });
    }

    function makeGutterMarker(hasBp, isCurrent) {
      const el = document.createElement("div");
      el.className = "cm-bp-marker";
      if (hasBp) {
        const dot = document.createElement("span");
        dot.className = "cm-bp-dot";
        dot.title = "Breakpoint (click để bỏ)";
        el.appendChild(dot);
      }
      if (isCurrent) {
        const arr = document.createElement("span");
        arr.className = "cm-dbg-arrow";
        arr.textContent = "▶";
        arr.title = "Dòng đang dừng";
        el.appendChild(arr);
      }
      return el;
    }

    function refreshEditorMarkers() {
      const lineCount = cm.lineCount();
      cm.operation(() => {
        for (let i = 0; i < lineCount; i++) {
          const ln = i + 1;
          const hasBp = dbg.breakpoints.has(ln);
          const isCur = dbg.status === "paused" && dbg.currentLine === ln;
          cm.setGutterMarker(i, "cm-bp-gutter", hasBp || isCur ? makeGutterMarker(hasBp, isCur) : null);
          cm.removeLineClass(i, "background", "cm-debug-current-line");
          if (isCur) {
            cm.addLineClass(i, "background", "cm-debug-current-line");
          }
        }
      });
    }

    function setStatus(text, tone) {
      if (!els.statusPill) return;
      els.statusPill.textContent = text;
      els.statusPill.className = "dbg-status-pill" + (tone ? ` dbg-tone-${tone}` : "");
    }

    function updateControls() {
      const paused = dbg.status === "paused";
      const busy = dbg.status === "busy";

      if (els.startBtn) els.startBtn.disabled = busy;
      if (els.startLabel) els.startLabel.textContent = paused ? "Continue" : "Start";
      if (els.nextBtn) els.nextBtn.disabled = !paused || busy;
      if (els.stepBtn) els.stepBtn.disabled = !paused || busy;
      if (els.finishBtn) els.finishBtn.disabled = !paused || busy;
      if (els.untilBtn) els.untilBtn.disabled = !paused || busy;
      if (els.stopBtn) els.stopBtn.disabled = !paused && !busy;

      if (els.tabBadge) {
        if (paused) {
          els.tabBadge.innerHTML = `<span class="badge dbg-badge-paused">PAUSED :${dbg.currentLine || "?"}</span>`;
        } else if (busy) {
          els.tabBadge.innerHTML = `<span class="badge dbg-badge-busy">RUNNING</span>`;
        } else {
          els.tabBadge.innerHTML = "";
        }
      }
    }

    function showDebugWorkspace() {
      if (els.toolbar) els.toolbar.classList.remove("hidden");
      if (els.judgePanel) els.judgePanel.classList.add("debug-expanded");
      if (typeof onShowDebugTab === "function") onShowDebugTab();
      setTimeout(() => cm.refresh(), 20);
    }

    function renderWatchesAndLocals(watchesData, localsData) {
      if (!els.watchesBody) return;
      const rows = [];
      const nextValues = Object.create(null);

      // 1. Custom Watch expressions
      if (dbg.watches.length > 0) {
        rows.push(`<tr class="dbg-section-row"><td colspan="3">Watches (${dbg.watches.length})</td></tr>`);
        const map = new Map((watchesData || []).map((w) => [w.expr, w]));
        dbg.watches.forEach((expr, idx) => {
          const item = map.get(expr);
          let valHtml = `<span class="muted">—</span>`;
          let changedCls = "";
          if (item) {
            if (item.error) {
              valHtml = `<span class="dbg-err-val">${escHtml(item.error)}</span>`;
            } else {
              const key = `w:${expr}`;
              nextValues[key] = item.value;
              if (dbg.prevValues[key] !== undefined && dbg.prevValues[key] !== item.value) {
                changedCls = " dbg-val-changed";
              }
              valHtml = `<code class="dbg-val${changedCls}">${escHtml(item.value)}</code>`;
            }
          }
          rows.push(`<tr>
            <td class="dbg-name-cell">
              <button class="dbg-rm-watch" data-widx="${idx}" title="Xóa watch">×</button>
              <code>${escHtml(expr)}</code>
            </td>
            <td>${valHtml}</td>
            <td class="muted small">watch</td>
          </tr>`);
        });
      }

      // 2. Local variables
      if (localsData && localsData.length > 0) {
        rows.push(`<tr class="dbg-section-row"><td colspan="3">Local variables (${localsData.length})</td></tr>`);
        for (const v of localsData) {
          const key = `l:${v.name}`;
          nextValues[key] = v.value;
          const changedCls =
            dbg.prevValues[key] !== undefined && dbg.prevValues[key] !== v.value
              ? " dbg-val-changed"
              : "";
          rows.push(`<tr>
            <td class="dbg-name-cell"><code>${escHtml(v.name)}</code></td>
            <td><code class="dbg-val${changedCls}">${escHtml(v.value)}</code></td>
            <td class="muted small dbg-type-cell" title="${escHtml(v.type)}">${escHtml(v.type)}</td>
          </tr>`);
        }
      } else if (dbg.status === "paused") {
        rows.push(`<tr><td colspan="3" class="muted dbg-empty-cell">Không có biến cục bộ nào trong frame hiện tại.</td></tr>`);
      } else if (dbg.watches.length === 0) {
        rows.push(`<tr><td colspan="3" class="muted dbg-empty-cell">Click vào số dòng để đặt chấm đỏ (Breakpoint), sau đó bấm <strong>Debug (F8)</strong>.</td></tr>`);
      }

      els.watchesBody.innerHTML = rows.join("");
      dbg.prevValues = nextValues;

      els.watchesBody.querySelectorAll(".dbg-rm-watch").forEach((btn) => {
        btn.onclick = () => {
          const idx = Number(btn.dataset.widx);
          if (!Number.isNaN(idx)) {
            dbg.watches.splice(idx, 1);
            saveWatches();
            syncWatches();
          }
        };
      });
    }

    function renderFrames(frames) {
      if (!els.stackList) return;
      if (!frames || !frames.length) {
        els.stackList.innerHTML = `<div class="muted dbg-empty-cell">Chưa có call stack.</div>`;
        return;
      }
      els.stackList.innerHTML = frames
        .map((fr) => {
          const active = fr.level === dbg.activeFrame ? " active" : "";
          const loc = fr.file ? `${fr.file}:${fr.line || "?"}` : "(system)";
          return `<div class="dbg-frame-item${active}" data-level="${fr.level}">
            <span class="dbg-frame-lvl">#${fr.level}</span>
            <span class="dbg-frame-fn">${escHtml(fr.func)}()</span>
            <span class="dbg-frame-loc">${escHtml(loc)}</span>
          </div>`;
        })
        .join("");

      els.stackList.querySelectorAll(".dbg-frame-item").forEach((el) => {
        el.onclick = () => selectFrame(Number(el.dataset.level));
      });
    }

    function applyDebugResponse(res) {
      if (!res) return;

      if (res.compile_error) {
        dbg.status = "idle";
        dbg.sessionId = null;
        dbg.currentLine = null;
        refreshEditorMarkers();
        updateControls();
        setStatus("Lỗi biên dịch (-g -O0)", "err");
        if (els.logPre) els.logPre.textContent = res.compile_error;
        setSideTab("log");
        return;
      }

      if (!res.ok && res.error) {
        if (res.status === "exited" || !res.session_id) {
          dbg.status = "exited";
          dbg.sessionId = null;
          dbg.currentLine = null;
        }
        refreshEditorMarkers();
        updateControls();
        setStatus(res.error, "err");
        if (els.logPre) {
          const logs = (res.logs || []).join("\n");
          els.logPre.textContent = (logs ? logs + "\n\n" : "") + res.error;
        }
        setSideTab("log");
        return;
      }

      dbg.sessionId = res.status === "paused" ? res.session_id : null;
      dbg.status = res.status || "exited";
      dbg.currentLine = res.line || null;

      if (Array.isArray(res.breakpoints) && dbg.status === "paused" && res.breakpoints.length > 0) {
        // Keep user breakpoints synced with GDB resolved lines
        dbg.breakpoints = new Set(res.breakpoints);
      }

      refreshEditorMarkers();
      updateControls();

      if (dbg.status === "paused") {
        if (dbg.currentLine) {
          cm.scrollIntoView({ line: dbg.currentLine - 1, ch: 0 }, 80);
        }
        if (res.signal) {
          setStatus(`Tín hiệu ${res.signal} tại ${res.func || "?"}() · dòng ${dbg.currentLine || "?"}`, "err");
        } else if (res.in_user_file) {
          setStatus(`Đang dừng: ${res.func || "main"}() · dòng ${dbg.currentLine}`, "paused");
        } else {
          setStatus(`Đang dừng trong thư viện (${res.func || "?"}) — bấm Step out để quay lại code`, "warn");
        }
      } else {
        setStatus("Chương trình đã kết thúc (exited)", "ok");
        if (res.stdout) setSideTab("stdout");
      }

      renderWatchesAndLocals(res.watches || [], res.locals || []);
      renderFrames(res.frames || []);
      if (els.stdoutPre) {
        els.stdoutPre.textContent = res.stdout ? res.stdout : "(Chưa có output — lưu ý cout chỉ flush khi dùng endl/flush hoặc kết thúc chương trình)";
      }
      if (els.logPre) {
        els.logPre.textContent = (res.logs || []).join("\n") || "(Trống)";
      }
    }

    async function startOrContinue() {
      showDebugWorkspace();
      if (dbg.status === "busy") return;
      if (dbg.status === "paused" && dbg.sessionId) {
        return sendAction("continue");
      }
      dbg.status = "busy";
      dbg.prevValues = Object.create(null);
      dbg.activeFrame = 0;
      dbg.dirtyWhilePaused = false;
      updateControls();
      setStatus("Đang biên dịch với -g -O0 và khởi chạy GDB...", "busy");

      try {
        const res = await apiPost("/api/debug/start", {
          code: cm.getValue(),
          stdin: typeof getStdin === "function" ? getStdin() : "",
          breakpoints: Array.from(dbg.breakpoints).sort((a, b) => a - b),
          watches: dbg.watches,
          session_id: dbg.sessionId,
        });
        applyDebugResponse(res);
      } catch (err) {
        dbg.status = "idle";
        updateControls();
        setStatus(`Lỗi kết nối server: ${err.message}`, "err");
      }
    }

    async function sendAction(action, extra) {
      if (dbg.status !== "paused" || !dbg.sessionId) return;
      dbg.status = "busy";
      if (action !== "frame") dbg.activeFrame = 0;
      updateControls();
      setStatus(`Đang thực thi (${action})...`, "busy");

      try {
        const res = await apiPost("/api/debug/action", Object.assign({
          session_id: dbg.sessionId,
          action,
        }, extra || {}));
        applyDebugResponse(res);
      } catch (err) {
        dbg.status = "idle";
        dbg.sessionId = null;
        refreshEditorMarkers();
        updateControls();
        setStatus(`Lỗi debug: ${err.message}`, "err");
      }
    }

    async function selectFrame(level) {
      if (dbg.status !== "paused" || !dbg.sessionId) return;
      dbg.activeFrame = level;
      return sendAction("frame", { level });
    }

    async function runToCursor() {
      const ln = cm.getCursor().line + 1;
      if (dbg.status === "paused" && dbg.sessionId) {
        return sendAction("until", { line: ln });
      }
      // If not started yet, start with a temporary breakpoint at cursor line
      showDebugWorkspace();
      const hadBp = dbg.breakpoints.has(ln);
      dbg.breakpoints.add(ln);
      await startOrContinue();
      if (!hadBp) {
        dbg.breakpoints.delete(ln);
        refreshEditorMarkers();
      }
    }

    async function stop() {
      const sid = dbg.sessionId;
      dbg.sessionId = null;
      dbg.status = "idle";
      dbg.currentLine = null;
      dbg.activeFrame = 0;
      refreshEditorMarkers();
      updateControls();
      setStatus("Đã dừng phiên debug", "");
      if (sid) {
        try {
          await apiPost("/api/debug/stop", { session_id: sid });
        } catch (_) {}
      }
    }

    async function toggleBreakpoint(lineNum) {
      if (lineNum < 1 || lineNum > cm.lineCount()) return;
      if (dbg.breakpoints.has(lineNum)) {
        dbg.breakpoints.delete(lineNum);
      } else {
        dbg.breakpoints.add(lineNum);
      }
      refreshEditorMarkers();
      if (dbg.status === "paused" && dbg.sessionId) {
        try {
          const res = await apiPost("/api/debug/breakpoints", {
            session_id: dbg.sessionId,
            breakpoints: Array.from(dbg.breakpoints).sort((a, b) => a - b),
          });
          if (res.ok && Array.isArray(res.breakpoints)) {
            dbg.breakpoints = new Set(res.breakpoints);
            refreshEditorMarkers();
          }
        } catch (_) {}
      }
    }

    async function clearBreakpoints() {
      dbg.breakpoints.clear();
      refreshEditorMarkers();
      if (dbg.status === "paused" && dbg.sessionId) {
        try {
          await apiPost("/api/debug/breakpoints", { session_id: dbg.sessionId, breakpoints: [] });
        } catch (_) {}
      }
    }

    async function syncWatches() {
      if (dbg.status === "paused" && dbg.sessionId) {
        try {
          const res = await apiPost("/api/debug/watches", {
            session_id: dbg.sessionId,
            watches: dbg.watches,
          });
          if (res.ok) {
            renderWatchesAndLocals(res.watches || [], res.locals || []);
            return;
          }
        } catch (_) {}
      }
      renderWatchesAndLocals([], []);
    }

    function addWatchFromInput() {
      if (!els.watchInput) return;
      const expr = els.watchInput.value.trim();
      if (!expr) return;
      if (!dbg.watches.includes(expr)) {
        dbg.watches.push(expr);
        saveWatches();
      }
      els.watchInput.value = "";
      syncWatches();
    }

    // Gutter click -> toggle breakpoint (both line numbers & bp gutter)
    cm.on("gutterClick", (_cm, lineIdx, gutter) => {
      if (gutter === "CodeMirror-linenumbers" || gutter === "cm-bp-gutter") {
        toggleBreakpoint(lineIdx + 1);
      }
    });

    cm.on("change", () => {
      if (dbg.status === "paused" && !dbg.dirtyWhilePaused) {
        dbg.dirtyWhilePaused = true;
        setStatus("Code đã thay đổi — bấm Stop rồi Start (F8) để debug bản mới", "warn");
      }
    });

    // Wire UI buttons
    if (els.launchBtn) {
      els.launchBtn.onclick = () => {
        if (els.toolbar && els.toolbar.classList.contains("hidden")) {
          showDebugWorkspace();
        } else {
          startOrContinue();
        }
      };
    }
    if (els.startBtn) els.startBtn.onclick = startOrContinue;
    if (els.nextBtn) els.nextBtn.onclick = () => sendAction("next");
    if (els.stepBtn) els.stepBtn.onclick = () => sendAction("step");
    if (els.finishBtn) els.finishBtn.onclick = () => sendAction("finish");
    if (els.untilBtn) els.untilBtn.onclick = runToCursor;
    if (els.stopBtn) els.stopBtn.onclick = stop;
    if (els.bpBtn) els.bpBtn.onclick = () => toggleBreakpoint(cm.getCursor().line + 1);
    if (els.clearBpBtn) els.clearBpBtn.onclick = clearBreakpoints;

    if (els.watchAddBtn) els.watchAddBtn.onclick = addWatchFromInput;
    if (els.watchInput) {
      els.watchInput.addEventListener("keydown", (e) => {
        if (e.key === "Enter") {
          e.preventDefault();
          addWatchFromInput();
        }
      });
    }

    document.querySelectorAll(".dbg-side-tab").forEach((btn) => {
      btn.onclick = () => setSideTab(btn.dataset.dtab);
    });

    // Code::Blocks keyboard shortcuts inside Arena view
    document.addEventListener("keydown", (e) => {
      const arena = document.getElementById("view-arena");
      if (!arena || !arena.classList.contains("active")) return;
      if (e.key === "F8" && !e.shiftKey && !e.ctrlKey && !e.altKey) {
        e.preventDefault();
        startOrContinue();
      } else if (e.key === "F8" && e.shiftKey && !e.ctrlKey) {
        e.preventDefault();
        stop();
      } else if (e.key === "F7" && !e.shiftKey && !e.ctrlKey) {
        if (dbg.status === "paused") {
          e.preventDefault();
          sendAction("next");
        }
      } else if (e.key === "F7" && e.shiftKey && !e.ctrlKey) {
        if (dbg.status === "paused") {
          e.preventDefault();
          sendAction("step");
        }
      } else if (e.key === "F7" && e.shiftKey && e.ctrlKey) {
        if (dbg.status === "paused") {
          e.preventDefault();
          sendAction("finish");
        }
      } else if (e.key === "F4" && !e.shiftKey && !e.ctrlKey) {
        e.preventDefault();
        runToCursor();
      } else if (e.key === "F5" && !e.shiftKey && !e.ctrlKey && cm.hasFocus()) {
        e.preventDefault();
        toggleBreakpoint(cm.getCursor().line + 1);
      }
    });

    renderWatchesAndLocals([], []);
    updateControls();

    return {
      startOrContinue,
      stop,
      showToolbar: () => {
        if (els.toolbar) els.toolbar.classList.remove("hidden");
        if (els.judgePanel) els.judgePanel.classList.add("debug-expanded");
      },
      onTabChanged: (tabName) => {
        if (tabName === "debug") {
          if (els.toolbar) els.toolbar.classList.remove("hidden");
          if (els.judgePanel) els.judgePanel.classList.add("debug-expanded");
          setTimeout(() => cm.refresh(), 20);
        } else if (dbg.status !== "paused" && dbg.status !== "busy") {
          if (els.judgePanel) els.judgePanel.classList.remove("debug-expanded");
          setTimeout(() => cm.refresh(), 20);
        }
      },
      onProblemChanged: () => {
        stop();
        dbg.breakpoints.clear();
        refreshEditorMarkers();
      },
    };
  }

  global.createCppDebugger = createCppDebugger;
})(window);

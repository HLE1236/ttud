const TOPIC_LABELS = {
  "stl-basic": "Mảng, STL & Xử lý chuỗi / Log",
  "prefix-sum": "Mảng cộng dồn 1D/2D & Cấu trúc đoạn (RMQ, SegTree)",
  "two-pointers": "Hai con trỏ & Cửa sổ trượt (Sliding Window)",
  "greedy": "Tham lam (Greedy)",
  "divide-conquer": "Chia để trị & Chặt nhị phân kết quả",
  "backtrack-gen": "Quay lui: Sinh cấu hình, Tập con & Tổ hợp",
  "branch-bound": "Nhánh và cận: Phân công, Chia nhóm & Hành trình",
  "dp-1d": "QHĐ Tuyến tính 1 chiều (Cầu thang, Không kề)",
  "dp-knapsack": "QHĐ Ba lô (Knapsack), Đổi tiền & Chứng khoán",
  "dp-subseq": "QHĐ Dãy con & Xâu (LIS, LCS, Đối xứng)",
  "dp-grid": "QHĐ Lưới 2D/3D & Bitmask trên bảng",
  "graph-traversal": "Đồ thị: BFS, DFS, Cây & Tarjan (Khớp/Cầu/SCC)",
  "graph-shortest-mst": "Đồ thị: Đường đi ngắn nhất (Dijkstra) & Cây khung (MST)",
  "graph-advanced": "Đồ thị nâng cao: Luồng, Cặp ghép & Bitmask DP"
};

const STARTER_CPP = `#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Viết code C++ của bạn tại đây
    

    return 0;
}
`;

const state = {
  bootstrap: null,
  currentView: "roadmap",
  roadmapTrack: "all",
  activeTheoryTab: "mindset",
  currentProblem: null,
  editor: null,
  fontSize: 14,
  theme: "dark",
  draftTimer: null,
  noteTimer: null,
  busyJudge: false,
  examFilterTrack: "midterm",
  activeExam: null,
  lastFinishedExam: null,
  examTimerInterval: null,
};

function esc(str) {
  if (str == null) return "";
  return String(str)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;");
}

function renderProse(text) {
  if (!text) return "";
  const lines = String(text).replace(/\r\n/g, "\n").split("\n");
  let html = "";
  let inList = false;
  const fmtInline = (s) =>
    esc(s)
      .replace(/`([^`]+)`/g, "<code>$1</code>")
      .replace(/\*\*([^*]+)\*\*/g, "<strong>$1</strong>")
      .replace(/\*([^*]+)\*/g, "<em>$1</em>");

  for (const raw of lines) {
    const t = raw.trim();
    if (t.startsWith("- ")) {
      if (!inList) {
        html += "<ul>";
        inList = true;
      }
      html += `<li>${fmtInline(t.slice(2))}</li>`;
    } else {
      if (inList) {
        html += "</ul>";
        inList = false;
      }
      if (t !== "") {
        html += `<p>${fmtInline(raw)}</p>`;
      }
    }
  }
  if (inList) html += "</ul>";
  return html;
}

async function apiGet(url) {
  const res = await fetch(url);
  if (!res.ok) throw new Error(`HTTP ${res.status}`);
  return res.json();
}

async function apiPost(url, payload) {
  const res = await fetch(url, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload),
  });
  return res.json();
}

function switchView(view) {
  state.currentView = view;
  document.querySelectorAll(".nav-tab").forEach((b) => {
    b.classList.toggle("active", b.dataset.view === view);
  });
  document.querySelectorAll(".view").forEach((v) => {
    v.classList.toggle("active", v.id === `view-${view}`);
  });
  if (view === "arena" && state.editor) {
    setTimeout(() => state.editor.refresh(), 25);
  }
  if (view === "exam" && typeof renderExamView === "function") {
    renderExamView();
  }
}

function getProblemMap() {
  const m = {};
  (state.bootstrap?.problems || []).forEach((p) => {
    m[p.id] = p;
  });
  return m;
}

function diffBadgeClass(diff) {
  if (diff === "Cơ bản") return "badge-easy";
  if (diff === "Trung bình") return "badge-medium";
  return "badge-hard";
}

function updateTopStats() {
  const s = state.bootstrap.stats;
  document.getElementById("nav-total-count").textContent = s.total;
  document.getElementById("top-ac-progress").textContent = `${s.solved}/${s.total}`;

  // Days until 2026-11-29
  const now = new Date();
  const target = new Date(2026, 10, 29);
  const diffDays = Math.ceil((target - new Date(now.getFullYear(), now.getMonth(), now.getDate())) / 86400000);
  const cd = document.getElementById("countdown-badge");
  cd.textContent = diffDays > 0 ? `Còn ${diffDays} ngày trước 29/11` : "Đích: 29/11";
}

const SUN_SVG = '<svg class="ui-ico" viewBox="0 0 24 24" aria-hidden="true"><circle cx="12" cy="12" r="4"/><path d="M12 2v2M12 20v2M4.93 4.93l1.41 1.41M17.66 17.66l1.41 1.41M2 12h2M20 12h2M6.34 17.66l-1.41 1.41M19.07 4.93l-1.41 1.41"/></svg>';
const MOON_SVG = '<svg class="ui-ico" viewBox="0 0 24 24" aria-hidden="true"><path d="M12 3a6 6 0 0 0 9 9 9 9 0 1 1-9-9Z"/></svg>';

function applyTheme(theme) {
  state.theme = theme;
  document.documentElement.setAttribute("data-theme", theme);
  document.getElementById("theme-toggle").innerHTML = theme === "dark" ? SUN_SVG : MOON_SVG;
  if (state.editor) {
    state.editor.setOption("theme", theme === "dark" ? "dracula" : "default");
  }
}

function renderRoadmap() {
  const { roadmap, stats, roadmap_checked } = state.bootstrap;
  const pmap = getProblemMap();
  const checkedDays = roadmap.days.filter((d) => roadmap_checked?.[String(d.day)]).length;
  const coreIds = [...new Set(roadmap.days.flatMap((d) => d.problems || []))];
  const coreSolved = coreIds.filter((pid) => pmap[pid]?.status?.solved).length;
  const btDpIds = [...new Set(roadmap.days.filter((d) => d.phase === "backtracking" || d.phase === "dp").flatMap((d) => d.problems || []))];
  const btDpSolved = btDpIds.filter((pid) => pmap[pid]?.status?.solved).length;
  const pctAll = stats.total ? Math.round((stats.solved * 100) / stats.total) : 0;
  const pctCore = coreIds.length ? Math.round((coreSolved * 100) / coreIds.length) : 0;
  const pctBtDp = btDpIds.length ? Math.round((btDpSolved * 100) / btDpIds.length) : 0;

  document.getElementById("roadmap-summary").innerHTML = `
    <div class="sum-box">
      <div class="sum-box-label">Bài trọng tâm Lộ trình</div>
      <div class="sum-box-val">${coreSolved} / ${coreIds.length} bài (${pctCore}%)</div>
      <div class="progress-bar"><div class="progress-fill" style="width:${pctCore}%"></div></div>
    </div>
    <div class="sum-box">
      <div class="sum-box-label">Quay lui &amp; QHĐ (Chặng 6–11)</div>
      <div class="sum-box-val">${btDpSolved} / ${btDpIds.length} bài (${pctBtDp}%)</div>
      <div class="progress-bar"><div class="progress-fill" style="width:${pctBtDp}%"></div></div>
    </div>
    <div class="sum-box">
      <div class="sum-box-label">Tổng tiến độ kho đề</div>
      <div class="sum-box-val">${stats.solved} / ${stats.total} bài (${pctAll}%)</div>
      <div class="progress-bar"><div class="progress-fill" style="width:${pctAll}%"></div></div>
    </div>
    <div class="sum-box">
      <div class="sum-box-label">Số chặng đã hoàn thành</div>
      <div class="sum-box-val">${checkedDays} / ${roadmap.days.length} chặng</div>
      <div class="progress-bar"><div class="progress-fill" style="width:${roadmap.days.length ? Math.round((checkedDays * 100) / roadmap.days.length) : 0}%"></div></div>
    </div>
  `;

  const container = document.getElementById("roadmap-days");
  const filteredDays = roadmap.days.filter(
    (d) => state.roadmapTrack === "all" || d.phase === state.roadmapTrack || d.track === state.roadmapTrack
  );

  container.innerHTML = filteredDays
    .map((d) => {
      const done = Boolean(roadmap_checked[String(d.day)]);
      const acInDay = d.problems.filter((pid) => pmap[pid]?.status?.solved).length;
      const trackBadge =
        d.track === "midterm"
          ? `<span class="badge badge-midterm">Giữa kỳ</span>`
          : `<span class="badge badge-final">Cuối kỳ</span>`;

      const theoryBtns = (d.theory_chapters || [])
        .map((cid) => `<button class="btn-link small" onclick="openTheoryChapter('${cid}')">Lý thuyết ${cid.replace("chap", "Chương ")}</button>`)
        .join(" · ");

      const renderDayProb = (pid) => {
        const p = pmap[pid];
        if (!p) return "";
        const solved = Boolean(p.status?.solved);
        const tried = !solved && (p.status?.attempts || 0) > 0;
        const dotCls = solved ? "ac" : tried ? "tried" : "";
        const vBadge = solved
          ? `<span class="badge verdict-AC">AC</span>`
          : tried
          ? `<span class="badge verdict-WA">${esc(p.status.last_verdict)}</span>`
          : `<span class="badge ${diffBadgeClass(p.difficulty)}">${esc(p.difficulty)}</span>`;
        const eloBadge = p.elo ? `<span class="badge badge-elo">Elo ${p.elo}</span>` : "";
        return `
          <div class="day-prob-item" onclick="openProblem('${pid}')">
            <div class="day-prob-left">
              <span class="status-dot ${dotCls}"></span>
              <span class="day-prob-title">${esc(p.title)}</span>
            </div>
            <div style="display:flex;gap:6px;align-items:center;">
              <span class="muted small">${p.exam_tags.length ? esc(p.exam_tags[0]) : ""}</span>
              ${eloBadge}
              ${vBadge}
            </div>
          </div>
        `;
      };

      const probItems = d.problems.map(renderDayProb).join("");
      const extraIds = d.extra_hustack || [];
      const extraAc = extraIds.filter((pid) => pmap[pid]?.status?.solved).length;
      const extraHtml = extraIds.length
        ? `
          <details class="day-extra-hustack">
            <summary>Luyện thêm HUSTack 2025.2 (${extraIds.length} bài · Đạt ${extraAc}/${extraIds.length})</summary>
            <div class="day-problems">${extraIds.map(renderDayProb).join("")}</div>
          </details>
        `
        : "";

      return `
        <div class="surface day-card track-${d.track} ${done ? "day-done" : ""}">
          <div class="day-top-section">
            <div class="day-head">
              <div>
                <div style="display:flex;gap:8px;align-items:center;flex-wrap:wrap;">
                  ${trackBadge}
                  <span class="muted small">${esc(d.date)} · AC ${acInDay}/${d.problems.length}</span>
                </div>
                <div class="day-title">${esc(d.title)}</div>
              </div>
              <label class="day-check-label">
                <input type="checkbox" ${done ? "checked" : ""} onchange="toggleDayDone(${d.day}, this.checked)" />
                Hoàn thành
              </label>
            </div>
            <div class="day-goal">${renderProse(d.goal)}</div>
            <div class="day-theory-row">${theoryBtns}</div>
          </div>
          <div class="day-problems">${probItems}</div>
          ${extraHtml}
        </div>
      `;
    })
    .join("");
}

window.toggleDayDone = async function (day, checked) {
  await apiPost("/api/roadmap/toggle", { day, checked });
  state.bootstrap.roadmap_checked[String(day)] = checked ? 1 : 0;
  renderRoadmap();
};

function renderTheorySidebar() {
  const nav = document.getElementById("theory-chapters-nav");
  nav.innerHTML = state.bootstrap.theory.chapters
    .map(
      (c) =>
        `<button class="theory-nav-item ${state.activeTheoryTab === c.id ? "active" : ""}" data-chapter="${c.id}">
          Chương ${esc(c.number)}: ${esc(c.title)}
        </button>`
    )
    .join("");

  document.querySelectorAll(".theory-nav-item").forEach((btn) => {
    btn.classList.toggle("active", btn.dataset.chapter === state.activeTheoryTab);
    btn.onclick = () => {
      state.activeTheoryTab = btn.dataset.chapter;
      renderTheorySidebar();
      renderTheoryContent();
    };
  });
}

function renderTheoryContent() {
  const theory = state.bootstrap.theory;
  const pmap = getProblemMap();
  const box = document.getElementById("theory-content");

  if (state.activeTheoryTab === "mindset") {
    const mf = theory.mindset_framework;
    box.innerHTML = `
      <div class="theory-doc-inner">
        <h2>${esc(mf.title)}</h2>
        <p class="muted prose-lead">${esc(mf.subtitle)}</p>
      </div>
      <div class="step-grid">
        ${mf.steps
          .map(
            (s) => `
          <div class="step-card">
            <h4>${esc(s.name)}</h4>
            <div>${renderProse(s.detail)}</div>
            <div class="callout callout-tip small" style="margin-bottom:0;"><strong>Lưu ý:</strong> ${esc(s.tip)}</div>
          </div>`
          )
          .join("")}
      </div>
    `;
    return;
  }

  if (state.activeTheoryTab === "path") {
    box.innerHTML = renderLearningPath(theory, pmap);
    return;
  }

  if (state.activeTheoryTab === "cheatsheet") {
    if (theory.recognition) {
      box.innerHTML = renderRecognition(theory, pmap);
      return;
    }
    box.innerHTML = `
      <div class="theory-doc-inner">
        <h2>Đối chiếu giới hạn kích thước dữ liệu và Thuật toán</h2>
        <p class="muted prose-lead">Xác định lớp độ phức tạp khả thi từ giới hạn kích thước đầu vào <code>n</code>:</p>
      </div>
      <table class="theory-table">
        <thead>
          <tr><th>Dấu hiệu đề bài / Giới hạn</th><th>Thuật toán tương ứng</th><th>Độ phức tạp</th><th>Bài tập luyện</th></tr>
        </thead>
        <tbody>
          ${theory.pattern_cheatsheet
            .map(
              (r) => `
            <tr>
              <td><strong>${esc(r.signal)}</strong><div class="muted small">${esc(r.why)}</div></td>
              <td><span class="badge badge-midterm">${esc(r.algorithm)}</span></td>
              <td><code>${esc(r.complexity)}</code></td>
              <td>${(r.examples || [])
                .map((pid) =>
                  pmap[pid]
                    ? `<div><button class="btn-link small" onclick="openProblem('${pid}')">${esc(pmap[pid].title)}</button></div>`
                    : ""
                )
                .join("")}</td>
            </tr>`
            )
            .join("")}
        </tbody>
      </table>
    `;
    return;
  }

  const chap = theory.chapters.find((c) => c.id === state.activeTheoryTab) || theory.chapters[0];
  if (chap.lesson) {
    box.innerHTML = renderLessonChapter(chap, pmap);
    return;
  }
  const lecLinks = (chap.source_lectures || [])
    .map((l) => `<a class="btn btn-sm btn-ghost" href="/api/lecture/${encodeURIComponent(l)}.pdf" target="_blank">${esc(l)}.pdf</a>`)
    .join(" ");

  box.innerHTML = `
    <div class="theory-doc-inner">
      <div style="display:flex;justify-content:space-between;align-items:flex-start;gap:12px;flex-wrap:wrap;margin-bottom:12px;">
        <div>
          <span class="badge ${chap.track === "midterm" ? "badge-midterm" : "badge-final"}">${esc(chap.track === "midterm" ? "Giữa kỳ" : "Cuối kỳ")}</span>
          <h2 style="margin-top:6px;">Chương ${esc(chap.number)}: ${esc(chap.title)}</h2>
        </div>
        <div style="display:flex;gap:6px;flex-wrap:wrap;">${lecLinks}</div>
      </div>
      <div>${renderProse(chap.summary)}</div>

      <div class="callout callout-tip">
        <h4>Bản chất thuật toán</h4>
        ${renderProse(chap.beginner_intuition)}
      </div>

      <div class="section-block">
        <h4>Quy trình phân tích</h4>
        <ol>
          ${(chap.how_to_think || []).map((t) => `<li style="margin-bottom:6px;">${renderProse(t)}</li>`).join("")}
        </ol>
      </div>

      <div class="section-block">
        <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:8px;">
          <h4 style="margin:0;">Mã nguồn C++17 mẫu</h4>
          <button class="btn btn-sm btn-ghost" onclick="copyChapterCode('${chap.id}', this)">Sao chép</button>
        </div>
        <pre class="code-box" id="chap-code-${chap.id}">${esc(chap.template_cpp)}</pre>
      </div>

      <div class="callout callout-warn">
        <h4>Lỗi cài đặt thường gặp (WA / TLE)</h4>
        <ul>${(chap.common_mistakes || []).map((m) => `<li>${esc(m)}</li>`).join("")}</ul>
      </div>

      <div class="section-block">
        <h4>Bài tập thực hành (${(chap.problems || []).length} bài)</h4>
        <div class="day-problems" style="border:1px solid var(--border);border-radius:var(--radius-sm);overflow:hidden;">
          ${(chap.problems || [])
            .map((pid) => {
              const p = pmap[pid];
              if (!p) return "";
              const solved = Boolean(p.status?.solved);
              return `
                <div class="day-prob-item" onclick="openProblem('${pid}')">
                  <div class="day-prob-left">
                    <span class="status-dot ${solved ? "ac" : ""}"></span>
                    <span class="day-prob-title">${esc(p.title)}</span>
                  </div>
                  <div style="display:flex;gap:6px;align-items:center;">
                    <span class="badge ${diffBadgeClass(p.difficulty)}">${esc(p.difficulty)}</span>
                    ${solved ? `<span class="badge verdict-AC">AC</span>` : `<span class="btn btn-sm btn-ghost">Mở bài</span>`}
                  </div>
                </div>`;
            })
            .join("")}
        </div>
      </div>
    </div>
  `;
}

window.copyChapterCode = function (cid, btn) {
  const el = document.getElementById(`chap-code-${cid}`);
  if (el) {
    navigator.clipboard?.writeText(el.textContent);
    if (btn) {
      btn.textContent = "Đã chép";
      setTimeout(() => { btn.textContent = "Sao chép"; }, 1400);
    }
  }
};

window.openTheoryChapter = function (cid, patternId) {
  state.activeTheoryTab = cid;
  switchView("theory");
  renderTheorySidebar();
  renderTheoryContent();
  if (patternId) {
    setTimeout(() => document.getElementById(`pat-${cid}-${patternId}`)?.scrollIntoView({ behavior: "smooth", block: "start" }), 50);
  } else {
    document.getElementById("theory-content")?.scrollTo?.(0, 0);
  }
};

window.scrollToPattern = function (anchorId) {
  document.getElementById(anchorId)?.scrollIntoView({ behavior: "smooth", block: "start" });
};

function probListHtml(pids, pmap, opts = {}) {
  const items = (pids || [])
    .map((pid, i) => {
      const p = pmap[pid];
      if (!p) return "";
      const solved = Boolean(p.status?.solved);
      const tried = !solved && (p.status?.attempts || 0) > 0;
      return `
        <div class="day-prob-item" onclick="openProblem('${pid}')">
          <div class="day-prob-left">
            <span class="status-dot ${solved ? "ac" : tried ? "tried" : ""}"></span>
            ${opts.numbered ? `<span class="muted small">${i + 1}.</span>` : ""}
            <span class="day-prob-title">${esc(p.title)}</span>
          </div>
          <div style="display:flex;gap:6px;align-items:center;">
            ${p.has_guide ? `<span class="badge badge-official" title="Có hướng dẫn từng tầng + khung code">Có hướng dẫn</span>` : ""}
            ${p.exam_tags?.length ? `<span class="muted small">${esc(p.exam_tags[0])}</span>` : ""}
            ${solved ? `<span class="badge verdict-AC">AC</span>` : `<span class="badge ${diffBadgeClass(p.difficulty)}">${esc(p.difficulty)}</span>`}
          </div>
        </div>`;
    })
    .join("");
  return items
    ? `<div class="day-problems" style="border:1px solid var(--border);border-radius:var(--radius-sm);overflow:hidden;">${items}</div>`
    : `<p class="muted small">(Chưa có bài)</p>`;
}

function countSolved(pids, pmap) {
  const list = (pids || []).filter((pid) => pmap[pid]);
  return { done: list.filter((pid) => pmap[pid].status?.solved).length, total: list.length };
}

function renderTraceTable(tr) {
  if (!tr || !tr.headers) return "";
  return `
    <div class="trace-box">
      <div class="trace-title">${esc(tr.title || "Chạy tay từng bước")}</div>
      ${tr.input ? `<div class="muted small" style="margin-bottom:6px;">Dữ liệu: <code>${esc(tr.input)}</code></div>` : ""}
      <div class="trace-scroll">
        <table class="theory-table trace-table">
          <thead><tr>${tr.headers.map((h) => `<th>${esc(h)}</th>`).join("")}</tr></thead>
          <tbody>${(tr.rows || []).map((r) => `<tr>${r.map((c) => `<td><code>${esc(c)}</code></td>`).join("")}</tr>`).join("")}</tbody>
        </table>
      </div>
      ${tr.note ? `<div class="small" style="margin-top:6px;">${renderProse(tr.note)}</div>` : ""}
    </div>`;
}

function renderLessonChapter(chap, pmap) {
  const L = chap.lesson;
  const lp = L.learning_path || {};
  const pats = L.patterns || [];
  const stage = (num, title, desc, pids) => {
    const c = countSolved(pids, pmap);
    return `
      <div class="path-stage">
        <div class="path-stage-head"><span class="guide-num">${num}</span><strong>${esc(title)}</strong>
          ${c.total ? `<span class="muted small" style="margin-left:auto;">AC ${c.done}/${c.total}</span>` : ""}</div>
        <div class="muted small" style="margin:4px 0 8px;">${desc}</div>
        ${pids ? probListHtml(pids, pmap, { numbered: true }) : ""}
      </div>`;
  };
  const lecLinks = (chap.source_lectures || [])
    .map((l) => `<a class="btn btn-sm btn-ghost" href="/api/lecture/${encodeURIComponent(l)}.pdf" target="_blank">${esc(l)}.pdf</a>`)
    .join(" ");

  const patHtml = pats
    .map(
      (p) => `
    <section class="pattern-card" id="pat-${chap.id}-${esc(p.id)}">
      <h3>${esc(p.name)}</h3>
      <div class="callout callout-tip">
        <h4>Khi nào dùng? (Dấu hiệu nhận biết)</h4>
        <ul>${(p.when || []).map((w) => `<li>${renderProse(w)}</li>`).join("")}</ul>
      </div>
      <div class="section-block"><h4>Ý tưởng</h4>${renderProse(p.idea)}</div>
      ${(p.steps || []).length ? `<div class="section-block"><h4>Bắt đầu code từ đâu?</h4><ol>${p.steps.map((s) => `<li style="margin-bottom:6px;">${renderProse(s)}</li>`).join("")}</ol></div>` : ""}
      <div class="section-block">
        <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:8px;">
          <h4 style="margin:0;">Code mẫu chung (có chú thích)</h4>
          <button class="btn btn-sm btn-ghost" onclick="copyCodeById('code-${chap.id}-${esc(p.id)}', this)">Sao chép</button>
        </div>
        <pre class="code-box" id="code-${chap.id}-${esc(p.id)}">${esc(p.template_cpp || "")}</pre>
      </div>
      ${renderTraceTable(p.trace)}
      ${(p.pitfalls || []).length ? `<div class="callout callout-warn"><h4>Lỗi hay gặp</h4><ul>${p.pitfalls.map((m) => `<li>${renderProse(m)}</li>`).join("")}</ul></div>` : ""}
      <div class="section-block"><h4>Bài luyện dạng này (dễ → khó)</h4>${probListHtml(p.problems, pmap, { numbered: true })}</div>
    </section>`
    )
    .join("");

  return `
    <div class="theory-doc-inner">
      <div style="display:flex;justify-content:space-between;align-items:flex-start;gap:12px;flex-wrap:wrap;margin-bottom:12px;">
        <div>
          <span class="badge ${chap.track === "midterm" ? "badge-midterm" : "badge-final"}">${esc(chap.track === "midterm" ? "Giữa kỳ" : chap.track)}</span>
          <h2 style="margin-top:6px;">${esc(chap.title)}</h2>
        </div>
        <div style="display:flex;gap:6px;flex-wrap:wrap;">${lecLinks}</div>
      </div>
      <div class="prose-lead">${renderProse(L.intro)}</div>

      <div class="section-block">
        <h4>Lộ trình chương</h4>
        <div class="path-grid">
          ${stage(1, "Đọc lý thuyết các dạng", "Đọc lần lượt từng dạng bên dưới: dấu hiệu nhận biết → ý tưởng → code mẫu → bảng chạy tay.", null)}
          ${stage(2, "Bài mẫu có hướng dẫn kỹ", "Làm theo tab <strong>Hướng dẫn giải</strong> từng tầng, khung code đã được nạp sẵn vào Editor.", lp.guided)}
          ${stage(3, "Luyện tập", "Tự làm, chỉ mở gợi ý khi bí quá 15–20 phút.", lp.practice)}
          ${stage(4, "Đề thi thật", "Bấm giờ như đi thi.", lp.exam)}
        </div>
      </div>

      <div class="section-block">
        <h4>Các dạng bài trong chương</h4>
        <div class="pattern-toc">
          ${pats.map((p) => `<button class="btn btn-sm btn-ghost" onclick="scrollToPattern('pat-${chap.id}-${esc(p.id)}')">${esc(p.name)}</button>`).join("")}
        </div>
      </div>
      ${patHtml}

      <details class="guide-more" style="margin-top:20px;">
        <summary>Tóm tắt slide gốc của chương</summary>
        ${renderProse(chap.summary)}
        <ol>${(chap.how_to_think || []).map((t) => `<li>${renderProse(t)}</li>`).join("")}</ol>
        <pre class="code-box">${esc(chap.template_cpp || "")}</pre>
      </details>
    </div>`;
}

function renderLearningPath(theory, pmap) {
  const cards = theory.chapters
    .map((c) => {
      const all = c.problems || [];
      const cnt = countSolved(all, pmap);
      const pct = cnt.total ? Math.round((cnt.done * 100) / cnt.total) : 0;
      const L = c.lesson;
      const next = L
        ? [...(L.learning_path?.guided || []), ...(L.learning_path?.practice || []), ...(L.learning_path?.exam || [])].find((pid) => pmap[pid] && !pmap[pid].status?.solved)
        : all.find((pid) => pmap[pid] && !pmap[pid].status?.solved);
      return `
        <div class="surface path-chap-card">
          <div style="display:flex;justify-content:space-between;align-items:center;gap:8px;flex-wrap:wrap;">
            <strong>${esc(c.title)}</strong>
            ${L ? `<span class="badge badge-official">Bài giảng chi tiết</span>` : `<span class="badge badge-medium">Tóm tắt</span>`}
          </div>
          <div class="muted small" style="margin:6px 0;">${L ? `${(L.patterns || []).length} dạng bài · ` : ""}AC ${cnt.done}/${cnt.total} bài</div>
          <div class="progress-bar"><div class="progress-fill" style="width:${pct}%"></div></div>
          <div style="display:flex;gap:8px;margin-top:10px;flex-wrap:wrap;">
            <button class="btn btn-sm btn-ghost" onclick="openTheoryChapter('${c.id}')">Học lý thuyết</button>
            ${next ? `<button class="btn btn-sm btn-ghost" onclick="openProblem('${next}')">Làm bài tiếp theo: ${esc(pmap[next].title)}</button>` : ""}
          </div>
        </div>`;
    })
    .join("");
  return `
    <div class="theory-doc-inner">
      <h2>Lộ trình học theo chương</h2>
      <p class="muted prose-lead">Học theo thứ tự: <strong>Lý thuyết dạng bài → 2–3 bài mẫu có hướng dẫn → Luyện tập → Đề thi thật</strong>. Nếu chưa biết bắt đầu từ đâu, cứ bấm "Làm bài tiếp theo".</p>
    </div>
    <div class="path-chap-grid">${cards}</div>`;
}

function renderRecognition(theory, pmap) {
  const R = theory.recognition;
  const exLinks = (ids) =>
    (ids || []).filter((pid) => pmap[pid]).map((pid) => `<button class="btn-link small" onclick="openProblem('${pid}')">${esc(pmap[pid].title)}</button>`).join("<br>");
  return `
    <div class="theory-doc-inner">
      <h2>${esc(R.title)}</h2>
      <div class="prose-lead">${renderProse(R.intro)}</div>
      <div class="callout callout-tip">
        <h4>Quy trình 60 giây khi đọc đề</h4>
        <ol>${(R.checklist || []).map((s) => `<li style="margin-bottom:4px;">${renderProse(s)}</li>`).join("")}</ol>
      </div>
      <h3>1. Nhìn giới hạn n → đoán độ phức tạp</h3>
      <table class="theory-table">
        <thead><tr><th>Giới hạn</th><th>Độ phức tạp cho phép</th><th>Thường là</th></tr></thead>
        <tbody>${(R.limits || []).map((r) => `<tr><td><code>${esc(r.n)}</code></td><td><code>${esc(r.complexity)}</code></td><td>${renderProse(r.techniques)}</td></tr>`).join("")}</tbody>
      </table>
      <h3 style="margin-top:20px;">2. Đọc đề thấy từ khoá → nghĩ tới thuật toán</h3>
      <table class="theory-table">
        <thead><tr><th>Đề bài nói gì</th><th>Nghĩ ngay tới</th><th>Vì sao</th><th>Bài ví dụ</th></tr></thead>
        <tbody>${(R.keywords || []).map((r) => `<tr>
          <td><strong>${renderProse(r.signal)}</strong></td>
          <td>${(r.chapter && r.pattern) ? `<button class="btn-link small" onclick="openTheoryChapter('${r.chapter}', '${esc(r.pattern)}')">${esc(r.algorithm)}</button>` : `<span class="badge badge-midterm">${esc(r.algorithm)}</span>`}</td>
          <td class="small">${renderProse(r.why)}</td>
          <td>${exLinks(r.examples)}</td></tr>`).join("")}</tbody>
      </table>
      <h3 style="margin-top:20px;">3. Phân biệt các dạng dễ nhầm</h3>
      <table class="theory-table">
        <thead><tr><th>Câu hỏi</th><th>Nếu CÓ</th><th>Nếu KHÔNG</th></tr></thead>
        <tbody>${(R.confusions || []).map((r) => `<tr><td>${renderProse(r.question)}</td><td>${renderProse(r.yes)}</td><td>${renderProse(r.no)}</td></tr>`).join("")}</tbody>
      </table>
    </div>`;
}

window.copyCodeById = function (id, btn) {
  const el = document.getElementById(id);
  if (!el) return;
  navigator.clipboard?.writeText(el.textContent);
  if (btn) {
    btn.textContent = "Đã chép";
    setTimeout(() => { btn.textContent = "Sao chép"; }, 1400);
  }
};

function renderProblemBankFilters() {
  const topicSel = document.getElementById("filter-topic");
  const curVal = topicSel.value;
  topicSel.innerHTML = `<option value="all">Tất cả chuyên đề</option>` +
    Object.entries(TOPIC_LABELS)
      .map(([k, v]) => `<option value="${k}">${esc(v)}</option>`)
      .join("");
  if (curVal) topicSel.value = curVal;

  const strip = document.getElementById("topic-stats-strip");
  const byTopic = state.bootstrap.stats.by_topic || {};
  strip.innerHTML = Object.entries(TOPIC_LABELS)
    .filter(([k]) => byTopic[k])
    .map(([k, label]) => {
      const s = byTopic[k];
      return `<button class="topic-chip ${topicSel.value === k ? "active" : ""}" onclick="pickTopicFilter('${k}')">
        ${esc(label)}: <strong>${s.solved}/${s.total}</strong>
      </button>`;
    })
    .join("");
}

window.pickTopicFilter = function (topic) {
  const sel = document.getElementById("filter-topic");
  sel.value = sel.value === topic ? "all" : topic;
  renderProblemBankFilters();
  renderProblemTable();
};

function renderProblemTable() {
  const q = document.getElementById("search-input").value.trim().toLowerCase();
  const track = document.getElementById("filter-track").value;
  const topic = document.getElementById("filter-topic").value;
  const year = document.getElementById("filter-year").value;
  const status = document.getElementById("filter-status").value;

  const rows = state.bootstrap.problems.filter((p) => {
    if (track !== "all" && p.track !== track) return false;
    if (topic !== "all" && p.topic !== topic) return false;
    if (status === "solved" && !p.status?.solved) return false;
    if (status === "unsolved" && p.status?.solved) return false;
    if (year !== "all" && !p.exam_tags.some((t) => t.includes(year))) return false;
    if (q) {
      const hay = `${p.id} ${p.title} ${p.exam_tags.join(" ")} ${TOPIC_LABELS[p.topic] || ""}`.toLowerCase();
      if (!hay.includes(q)) return false;
    }
    return true;
  });

  const tbody = document.getElementById("problems-tbody");
  if (!rows.length) {
    tbody.innerHTML = `<tr><td colspan="7" class="muted center-empty">Không có bài nào khớp bộ lọc.</td></tr>`;
    return;
  }

  tbody.innerHTML = rows
    .map((p) => {
      const solved = Boolean(p.status?.solved);
      const tried = !solved && (p.status?.attempts || 0) > 0;
      const stHtml = solved
        ? `<span class="badge verdict-AC">AC</span>`
        : tried
        ? `<span class="badge verdict-WA">${esc(p.status.last_verdict)}</span>`
        : `<span class="muted">—</span>`;
      const trackBadge =
        p.track === "midterm"
          ? `<span class="badge badge-midterm">Giữa kỳ</span>`
          : `<span class="badge badge-final">Cuối kỳ</span>`;
      const tagsHtml = (p.exam_tags || [])
        .map((t) => `<span class="badge badge-exam">${esc(t)}</span>`)
        .join("");
      const testBadge = p.has_official_tests
        ? `<span class="badge badge-official">${p.test_count} test đề gốc</span>`
        : `<span class="badge badge-gen">${p.test_count} test chuẩn hóa</span>`;

      return `
        <tr>
          <td>${stHtml}</td>
          <td>
            <div style="font-weight:600;cursor:pointer;" onclick="openProblem('${p.id}')">${esc(p.title)}</div>
            <div class="muted small"><code>${esc(p.id)}</code> · ${p.time_limit_ms} ms ${p.has_checker ? "· Checker đa nghiệm" : ""}</div>
          </td>
          <td>
            <div>${trackBadge}</div>
            <div class="muted small" style="margin-top:3px;">${esc(TOPIC_LABELS[p.topic] || p.topic)}</div>
          </td>
          <td><div class="tag-list">${tagsHtml}</div></td>
          <td>
            <div style="display:flex;gap:5px;align-items:center;flex-wrap:wrap;">
              ${p.elo ? `<span class="badge badge-elo">${p.elo}</span>` : ""}
              <span class="badge ${diffBadgeClass(p.difficulty)}">${esc(p.difficulty)}</span>
            </div>
          </td>
          <td>${testBadge}</td>
          <td style="text-align:right;"><button class="btn btn-sm btn-ghost" onclick="openProblem('${p.id}')">Mở bài</button></td>
        </tr>`;
    })
    .join("");
}

function initEditor() {
  const ta = document.getElementById("code-editor");
  state.editor = CodeMirror.fromTextArea(ta, {
    mode: "text/x-c++src",
    theme: state.theme === "dark" ? "dracula" : "default",
    lineNumbers: true,
    gutters: ["CodeMirror-linenumbers", "cm-bp-gutter"],
    matchBrackets: true,
    autoCloseBrackets: true,
    indentUnit: 4,
    tabSize: 4,
    indentWithTabs: false,
    extraKeys: {
      "Ctrl-Enter": () => runSample(),
      "Ctrl-Shift-Enter": () => submitSolution(),
    },
  });
  state.editor.setValue(STARTER_CPP);
  if (typeof window.attachCppAutocomplete === "function") {
    const enabled = localStorage.getItem("ttud_autocomplete") !== "false";
    state.autocomplete = window.attachCppAutocomplete(state.editor, { enabled });
  }
  if (typeof window.createCppDebugger === "function") {
    state.debugger = window.createCppDebugger(state.editor, {
      apiPost,
      getStdin: () => document.getElementById("custom-input")?.value || "",
      onShowDebugTab: () => setRightTab("debug"),
    });
  }
  state.editor.on("change", () => {
    if (!state.currentProblem) return;
    document.getElementById("draft-status").textContent = "Đang lưu nháp...";
    clearTimeout(state.draftTimer);
    const pid = state.currentProblem.id;
    const code = state.editor.getValue();
    state.draftTimer = setTimeout(async () => {
      await apiPost("/api/draft", { problem_id: pid, code });
      document.getElementById("draft-status").textContent = "Đã lưu nháp";
    }, 600);
  });
}

function setLeftTab(tabName) {
  document.querySelectorAll("#left-subtabs .sub-tab").forEach((b) => {
    b.classList.toggle("active", b.dataset.ltab === tabName);
  });
  document.querySelectorAll(".ltab-pane").forEach((p) => {
    p.classList.toggle("active", p.id === `ltab-${tabName}`);
  });
}

function setRightTab(tabName) {
  document.querySelectorAll(".judge-tabs .sub-tab").forEach((b) => {
    b.classList.toggle("active", b.dataset.rtab === tabName);
  });
  document.querySelectorAll(".rtab-pane").forEach((p) => {
    p.classList.toggle("active", p.id === `rtab-${tabName}`);
  });
  if (state.debugger) state.debugger.onTabChanged(tabName);
}

window.openProblem = async function (pid) {
  if (state.debugger) state.debugger.onProblemChanged();
  switchView("arena");
  const prob = await apiGet(`/api/problem/${encodeURIComponent(pid)}`);
  state.currentProblem = prob;

  document.getElementById("arena-title").textContent = prob.title;
  document.getElementById("arena-badges").innerHTML = `
    <span class="badge ${prob.track === "midterm" ? "badge-midterm" : "badge-final"}">${prob.track === "midterm" ? "Giữa kỳ" : "Cuối kỳ"}</span>
    ${prob.elo ? `<span class="badge badge-elo">Elo ${prob.elo}</span>` : ""}
    <span class="badge ${diffBadgeClass(prob.difficulty)}">${esc(prob.difficulty)}</span>
    ${prob.status?.solved ? `<span class="badge verdict-AC">Đã AC</span>` : ""}
    ${prob.has_checker ? `<span class="badge badge-official">Multi-Answer Checker</span>` : ""}
  `;
  document.getElementById("arena-exam-tags").innerHTML = (prob.exam_tags || [])
    .map((t) => `<span class="badge badge-exam">${esc(t)}</span>`)
    .join("");
  document.getElementById("arena-tl").textContent = `Time Limit: ${prob.time_limit_ms} ms · ${prob.test_count} test cases`;

  document.getElementById("btn-open-theory").onclick = () => openTheoryChapter(prob.theory_chapter || "chap1");

  document.getElementById("prob-statement").innerHTML = renderProse(prob.statement);
  document.getElementById("prob-input").innerHTML = renderProse(prob.input_spec);
  document.getElementById("prob-output").innerHTML = renderProse(prob.output_spec);

  const samplesHtml = (prob.samples || [])
    .map(
      (s, i) => `
      <div class="sample-box">
        <div class="sample-head">
          <span>Ví dụ #${i + 1}</span>
          <button class="btn-link small" onclick="loadSampleIntoRunner(${i})">Nạp vào Input</button>
        </div>
        <div class="io-grid">
          <div><div class="io-label">Input</div><pre class="io-pre">${esc(s.input)}</pre></div>
          <div><div class="io-label">Output</div><pre class="io-pre">${esc(s.output)}</pre></div>
        </div>
      </div>`
    )
    .join("");
  document.getElementById("prob-samples").innerHTML = samplesHtml || `<p class="muted">Không có sample đính kèm — xem ở tab Thư mục Test.</p>`;

  // Huong dan giai mo dan tung tang
  renderGuideLadder(prob);

  const comp = prob.complexity || {};
  document.getElementById("prob-complexity").innerHTML = `
    <div><strong>Thời gian:</strong> <code>${esc(comp.time || "—")}</code> · <strong>Bộ nhớ:</strong> <code>${esc(comp.space || "—")}</code></div>
  `;

  // Notes
  document.getElementById("note-textarea").value = prob.note || "";
  document.getElementById("note-status").textContent = "Đã tải ghi chú";

  // History
  renderHistory(prob.submissions || []);

  // Tests folder
  document.getElementById("test-count").textContent = (prob.test_names || []).length;
  document.getElementById("tests-dir-path").textContent = prob.tests_dir || "";
  document.getElementById("test-preview-box").classList.add("hidden");
  document.getElementById("test-preview-list").innerHTML = (prob.test_names || [])
    .map((tn) => `<button class="test-chip" onclick="previewTest('${esc(tn)}', this)">Test #${esc(tn)}</button>`)
    .join("");

  // Load draft, else guided skeleton (unless in active exam), else starter
  const inActiveExam = Boolean(state.activeExam);
  const skeleton = !inActiveExam ? prob.guide?.skeleton_cpp : null;
  if (prob.draft && prob.draft.trim()) {
    state.editor.setValue(prob.draft);
    document.getElementById("draft-status").textContent = "Đã nạp bản nháp";
  } else if (skeleton) {
    state.editor.setValue(skeleton);
    document.getElementById("draft-status").textContent = "Khung code có sẵn — điền vào các chỗ TODO";
  } else {
    state.editor.setValue(STARTER_CPP);
    document.getElementById("draft-status").textContent = inActiveExam ? "Chế độ Thi thử (Không gợi ý)" : "Template mới";
  }
  setTimeout(() => state.editor.refresh(), 20);

  // Lock mindset/hints/tests tabs during active exam
  const mindsetTabBtn = document.querySelector('#left-subtabs .sub-tab[data-ltab="mindset"]');
  const testsTabBtn = document.querySelector('#left-subtabs .sub-tab[data-ltab="tests"]');
  const theoryBtn = document.getElementById("btn-open-theory");
  if (inActiveExam) {
    if (mindsetTabBtn) mindsetTabBtn.innerHTML = "Hướng dẫn giải 🔒";
    if (testsTabBtn) testsTabBtn.classList.add("hidden");
    if (theoryBtn) theoryBtn.classList.add("hidden");
  } else {
    if (mindsetTabBtn) mindsetTabBtn.innerHTML = "Hướng dẫn giải";
    if (testsTabBtn) testsTabBtn.classList.remove("hidden");
    if (theoryBtn) theoryBtn.classList.remove("hidden");
  }
  renderArenaExamStrip();

  // Preload Sample #1 into custom runner
  if (prob.samples && prob.samples.length > 0) {
    document.getElementById("custom-input").value = prob.samples[0].input;
  } else {
    document.getElementById("custom-input").value = "";
  }
  document.getElementById("sample-output").textContent = 'Nhấn "Chạy thử" (Ctrl+Enter) để biên dịch và thực thi với Input bên trái.';
  document.getElementById("sample-run-meta").textContent = "";
  document.getElementById("verdict-pill-badge").innerHTML = "";
  document.getElementById("verdict-container").innerHTML = `<div class="muted center-empty">Nhấn <strong>Nộp bài</strong> (Ctrl+Shift+Enter) để chấm trên toàn bộ ${prob.test_count} test case.</div>`;
  setLeftTab("statement");
  setRightTab("sample");
};

function findPatternForProblem(pid, patternId) {
  for (const chap of state.bootstrap?.theory?.chapters || []) {
    for (const pat of chap.lesson?.patterns || []) {
      if ((patternId && pat.id === patternId) || (pat.problems || []).includes(pid)) {
        return { chap, pat };
      }
    }
  }
  return null;
}

function buildGuideSteps(prob) {
  const g = prob.guide;
  const hintsHtml = (prob.hints || []).length
    ? `<ul>${prob.hints.map((h) => `<li>${renderProse(h)}</li>`).join("")}</ul>`
    : "";
  const solution = {
    title: "Lời giải đầy đủ (C++17)",
    html: `<pre class="code-box">${esc(prob.ref_cpp || "// Không có code mẫu")}</pre>
      <button class="btn btn-sm btn-ghost" onclick="loadGuideCode('ref')">Nạp lời giải vào Editor</button>`,
  };
  if (!g) {
    return [
      { title: "Nhận dạng & Phân tích", html: renderProse(prob.mindset) || `<p class="muted">Chưa có phân tích.</p>` },
      { title: "Gợi ý cách làm", html: hintsHtml || `<p class="muted">Chưa có gợi ý.</p>` },
      solution,
    ];
  }
  return [
    { title: "Nhận dạng dạng bài", html: renderProse(g.recognize) },
    {
      title: "Ý tưởng thuật toán",
      html: renderProse(g.idea) +
        (prob.mindset ? `<details class="guide-more"><summary>Phân tích thêm</summary>${renderProse(prob.mindset)}${hintsHtml}</details>` : ""),
    },
    { title: "Mã giả", html: `<pre class="code-box">${esc(g.pseudocode || "")}</pre>` },
    {
      title: "Khung code (điền vào chỗ TODO)",
      html: `<pre class="code-box">${esc(g.skeleton_cpp || "")}</pre>
        <button class="btn btn-sm btn-ghost" onclick="loadGuideCode('skeleton')">Nạp khung vào Editor</button>`,
    },
    solution,
  ];
}

function renderGuideLadder(prob) {
  state.guideOpened = 1;
  state.guideSteps = buildGuideSteps(prob);
  const found = findPatternForProblem(prob.id, prob.guide?.pattern);
  state.guidePatternLink = found
    ? `<div class="guide-pattern-link">Thuộc dạng: <strong>${esc(found.pat.name)}</strong>
         <button class="btn-link small" onclick="openTheoryChapter('${found.chap.id}', '${esc(found.pat.id)}')">Xem bài giảng dạng này →</button></div>`
    : "";
  drawGuideLadder();
}

function drawGuideLadder() {
  const el = document.getElementById("prob-guide-ladder");
  const compBox = document.getElementById("prob-complexity-box");
  if (state.activeExam) {
    if (compBox) compBox.classList.add("hidden");
    el.innerHTML = `
      <div class="callout callout-warn">
        <h4>🔒 Đang trong thời gian Thi thử (120 phút)</h4>
        <p style="margin:6px 0 0;">Toàn bộ Nhận dạng dạng bài, Gợi ý, Mã giả, Khung code và Lời giải mẫu được khóa lại để bạn rèn khả năng tự phân tích đề như trong phòng thi thật. Sau khi bấm <strong>Nộp đề &amp; Kết thúc thi</strong>, toàn bộ hướng dẫn sẽ mở lại kèm nhận xét chi tiết cho từng câu.</p>
      </div>`;
    return;
  }
  if (compBox) compBox.classList.remove("hidden");
  const steps = state.guideSteps || [];
  el.innerHTML = state.guidePatternLink + steps
    .map((s, i) => {
      const open = i < state.guideOpened;
      if (open) {
        return `<div class="guide-step open"><div class="guide-step-head"><span class="guide-num">${i + 1}</span>${esc(s.title)}</div>
          <div class="guide-step-body prose">${s.html}</div></div>`;
      }
      const isNext = i === state.guideOpened;
      return `<div class="guide-step locked"><div class="guide-step-head"><span class="guide-num">${i + 1}</span>${esc(s.title)}
          ${isNext ? `<button class="btn btn-sm btn-ghost" style="margin-left:auto;" onclick="openNextGuideStep()">${i === steps.length - 1 ? "Xem lời giải" : "Mở tầng này"}</button>` : `<span class="muted small" style="margin-left:auto;">Đang khoá</span>`}
        </div></div>`;
    })
    .join("");
}

window.openNextGuideStep = function () {
  const steps = state.guideSteps || [];
  if (state.guideOpened === steps.length - 1 && !confirm("Bạn chắc chắn muốn xem lời giải đầy đủ? Hãy thử tự code từ khung trước nhé!")) return;
  state.guideOpened = Math.min(steps.length, state.guideOpened + 1);
  drawGuideLadder();
};

window.loadGuideCode = function (kind) {
  const prob = state.currentProblem;
  if (!prob || !state.editor) return;
  const code = kind === "ref" ? prob.ref_cpp : prob.guide?.skeleton_cpp;
  if (code && confirm("Thay thế toàn bộ code trong Editor?")) state.editor.setValue(code);
};

window.loadSampleIntoRunner = function (idx) {
  const s = state.currentProblem?.samples?.[idx];
  if (!s) return;
  document.getElementById("custom-input").value = s.input;
  setRightTab("sample");
};

window.previewTest = async function (tname, btn) {
  if (!state.currentProblem) return;
  document.querySelectorAll(".test-chip").forEach((c) => c.classList.remove("active"));
  btn.classList.add("active");
  const d = await apiGet(`/api/problem/${encodeURIComponent(state.currentProblem.id)}/test-preview?name=${encodeURIComponent(tname)}`);
  document.getElementById("test-preview-box").classList.remove("hidden");
  document.getElementById("test-preview-title").textContent = `Test #${d.name} (In: ${d.input_size} B, Out: ${d.output_size} B)`;
  document.getElementById("test-preview-in").textContent = d.input;
  document.getElementById("test-preview-out").textContent = d.output;
};

function renderHistory(subs) {
  document.getElementById("hist-count").textContent = subs.length;
  const el = document.getElementById("history-list");
  if (!subs.length) {
    el.innerHTML = `<p class="muted">Bạn chưa nộp bài này lần nào.</p>`;
    return;
  }
  window._lastHistorySubs = subs;
  el.innerHTML = subs
    .map(
      (s, i) => `
      <div class="hist-item">
        <div class="hist-top">
          <div style="display:flex;gap:8px;align-items:center;">
            <span class="badge verdict-${esc(s.verdict)}">${esc(s.verdict)}</span>
            <strong>${s.passed}/${s.total} test</strong>
            <span class="muted small mono-meta">${s.max_time_ms} ms</span>
          </div>
          <span class="muted small">${esc(s.created_at)}</span>
        </div>
        <div style="margin-top:8px;display:flex;gap:10px;">
          <button class="btn-link small" onclick="loadSubmissionCode(${i})">Nạp code này vào Editor</button>
          <button class="btn-link small" onclick="viewSubmissionResult(${i})">Xem chi tiết chấm</button>
        </div>
      </div>`
    )
    .join("");
}

window.loadSubmissionCode = function (idx) {
  const s = window._lastHistorySubs?.[idx];
  if (s && state.editor) {
    state.editor.setValue(s.code || "");
  }
};

window.viewSubmissionResult = function (idx) {
  const s = window._lastHistorySubs?.[idx];
  if (s) {
    renderVerdictResult(s);
    setRightTab("verdict");
  }
};

function renderVerdictResult(res) {
  const container = document.getElementById("verdict-container");
  const pill = document.getElementById("verdict-pill-badge");
  pill.innerHTML = `<span class="badge verdict-${esc(res.verdict)}" style="margin-left:6px;">${esc(res.verdict)} ${res.passed}/${res.total}</span>`;

  const labelMap = {
    AC: "AC — Accepted (Đạt toàn bộ test kiểm thử)",
    WA: "WA — Wrong Answer (Kết quả không khớp)",
    TLE: "TLE — Time Limit Exceeded (Vượt giới hạn thời gian)",
    RE: "RE — Runtime Error (Lỗi khi thực thi)",
    CE: "CE — Compile Error (Lỗi biên dịch)",
  };

  let html = `
    <div class="verdict-banner verdict-${esc(res.verdict)}">
      <span>${labelMap[res.verdict] || esc(res.verdict)}</span>
      <span>Passed: ${res.passed}/${res.total} · Max Time: ${res.max_time_ms} ms</span>
    </div>
  `;

  if (res.compile_error) {
    html += `<div class="io-label">Chi tiết lỗi biên dịch (g++):</div><pre class="io-pre">${esc(res.compile_error)}</pre>`;
    container.innerHTML = html;
    return;
  }

  const details = res.details || [];
  window._lastVerdictDetails = details;
  html += `<div class="tc-grid">` +
    details
      .map(
        (d, idx) =>
          `<button class="tc-pill verdict-${esc(d.verdict)}" onclick="showTestDetail(${idx})">
            #${esc(d.test)} ${esc(d.verdict)} (${d.time_ms}ms)
          </button>`
      )
      .join("") +
    `</div><div id="verdict-detail-box"></div>`;

  container.innerHTML = html;

  // Auto-open first failing test
  const firstBad = details.findIndex((d) => d.verdict !== "AC");
  if (firstBad !== -1) showTestDetail(firstBad);
}

window.showTestDetail = function (idx) {
  const d = window._lastVerdictDetails?.[idx];
  const box = document.getElementById("verdict-detail-box");
  if (!d || !box) return;
  if (d.verdict === "AC") {
    box.innerHTML = `<div class="diff-box"><span class="badge verdict-AC">Test #${esc(d.test)} AC</span> — Thời gian chạy: <strong>${d.time_ms} ms</strong>.</div>`;
    return;
  }
  if (d.verdict === "SKIP") {
    box.innerHTML = `<div class="diff-box">${esc(d.note || "Đã bỏ qua")}</div>`;
    return;
  }
  box.innerHTML = `
    <div class="diff-box">
      <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:6px;">
        <strong>Chi tiết Test #${esc(d.test)} (<span class="badge verdict-${esc(d.verdict)}">${esc(d.verdict)}</span>)</strong>
        ${d.checker_msg ? `<span class="muted small">Checker: ${esc(d.checker_msg)}</span>` : ""}
      </div>
      <div class="diff-cols">
        <div><div class="io-label">Input</div><pre class="io-pre">${esc(d.input_preview || "")}</pre></div>
        <div><div class="io-label">Kết quả đúng (Expected)</div><pre class="io-pre">${esc(d.expected_preview || "")}</pre></div>
        <div><div class="io-label">Output của bạn (Actual)</div><pre class="io-pre">${esc(d.actual_preview || "")}</pre></div>
      </div>
    </div>
  `;
};

async function runSample() {
  if (!state.currentProblem || state.busyJudge) return;
  state.busyJudge = true;
  const btn = document.getElementById("btn-run-sample");
  btn.disabled = true;
  btn.textContent = "Đang chạy...";
  setRightTab("sample");
  document.getElementById("sample-output").textContent = "Đang biên dịch bằng g++ -O2 -std=c++17...";
  try {
    const res = await apiPost("/api/run-sample", {
      problem_id: state.currentProblem.id,
      code: state.editor.getValue(),
      custom_input: document.getElementById("custom-input").value,
    });
    if (res.verdict === "CE") {
      document.getElementById("sample-run-meta").textContent = "Compile Error";
      document.getElementById("sample-output").textContent = res.compile_error;
    } else if (res.verdict === "OK") {
      document.getElementById("sample-run-meta").textContent = `Hoàn tất trong ${res.time_ms} ms`;
      document.getElementById("sample-output").textContent = res.output || "(Output rỗng)";
    } else {
      document.getElementById("sample-run-meta").textContent = `${res.verdict} (${res.time_ms} ms)`;
      document.getElementById("sample-output").textContent = `${res.stderr || ""}\n${res.output || ""}`.trim();
    }
  } catch (e) {
    document.getElementById("sample-output").textContent = "Lỗi kết nối tới local server: " + e.message;
  } finally {
    state.busyJudge = false;
    btn.disabled = false;
    btn.innerHTML = 'Chạy thử <kbd>Ctrl+↵</kbd>';
  }
}

async function submitSolution() {
  if (!state.currentProblem || state.busyJudge) return;
  state.busyJudge = true;
  const btn = document.getElementById("btn-submit");
  btn.disabled = true;
  btn.textContent = "Đang chấm...";
  setRightTab("verdict");
  document.getElementById("verdict-container").innerHTML = `<div class="muted center-empty">Đang biên dịch C++17 và chấm từng test case...</div>`;

  try {
    const pid = state.currentProblem.id;
    const res = await apiPost("/api/submit", {
      problem_id: pid,
      code: state.editor.getValue(),
    });
    renderVerdictResult(res);

    // Sync global stats & problem status
    if (res.stats) state.bootstrap.stats = res.stats;
    const p = state.bootstrap.problems.find((x) => x.id === pid);
    if (p && res.problem_status) p.status = res.problem_status;
    onExamProblemSubmitted(pid, res);
    updateTopStats();
    renderRoadmap();
    renderProblemBankFilters();
    renderProblemTable();

    // Reload history silently
    const full = await apiGet(`/api/problem/${encodeURIComponent(pid)}`);
    renderHistory(full.submissions || []);
  } catch (e) {
    document.getElementById("verdict-container").innerHTML = `<div class="verdict-banner verdict-CE">Lỗi khi chấm: ${esc(e.message)}</div>`;
  } finally {
    state.busyJudge = false;
    btn.disabled = false;
    btn.innerHTML = 'Nộp bài <kbd>Ctrl+⇧+↵</kbd>';
  }
}

function bindEvents() {
  document.querySelectorAll(".nav-tab").forEach((b) => {
    b.onclick = () => {
      const view = b.dataset.view;
      if (view === "arena" && !document.getElementById("prob-statement").innerHTML && state.currentProblem) {
        openProblem(state.currentProblem.id);
      } else {
        switchView(view);
      }
    };
  });
  const brandHome = document.getElementById("brand-home");
  brandHome.onclick = () => switchView("roadmap");
  brandHome.onkeydown = (e) => {
    if (e.key === "Enter" || e.key === " ") {
      e.preventDefault();
      switchView("roadmap");
    }
  };

  document.getElementById("theme-toggle").onclick = async () => {
    const next = state.theme === "dark" ? "light" : "dark";
    applyTheme(next);
    await apiPost("/api/settings", { key: "theme", value: next });
  };

  document.querySelectorAll("#roadmap-track-filter .filter-pill").forEach((b) => {
    b.onclick = () => {
      state.roadmapTrack = b.dataset.track;
      document.querySelectorAll("#roadmap-track-filter .filter-pill").forEach((x) => x.classList.toggle("active", x === b));
      renderRoadmap();
    };
  });

  ["search-input", "filter-track", "filter-topic", "filter-year", "filter-status"].forEach((id) => {
    document.getElementById(id).addEventListener("input", () => {
      renderProblemBankFilters();
      renderProblemTable();
    });
  });

  document.getElementById("btn-back-list").onclick = () => switchView("problems");

  document.querySelectorAll("#left-subtabs .sub-tab").forEach((b) => {
    b.onclick = () => setLeftTab(b.dataset.ltab);
  });
  document.querySelectorAll(".judge-tabs .sub-tab").forEach((b) => {
    b.onclick = () => setRightTab(b.dataset.rtab);
  });

  document.getElementById("btn-template").onclick = () => {
    if (confirm("Đặt lại nội dung Editor về template C++ cơ bản?")) {
      state.editor.setValue(STARTER_CPP);
    }
  };

  document.getElementById("btn-copy-sample1").onclick = () => loadSampleIntoRunner(0);
  document.getElementById("btn-run-sample").onclick = runSample;
  document.getElementById("btn-submit").onclick = submitSolution;

  const acBtn = document.getElementById("btn-autocomplete-toggle");
  if (acBtn && state.autocomplete) {
    const syncAcBtn = () => {
      const on = state.autocomplete.isEnabled();
      acBtn.textContent = on ? "Gợi ý: Bật" : "Gợi ý: Tắt";
      acBtn.classList.toggle("btn-toggle-on", on);
    };
    syncAcBtn();
    acBtn.onclick = () => {
      const next = !state.autocomplete.isEnabled();
      state.autocomplete.setEnabled(next);
      localStorage.setItem("ttud_autocomplete", String(next));
      syncAcBtn();
      state.editor.focus();
    };
  }

  document.getElementById("btn-font-dec").onclick = () => {
    state.fontSize = Math.max(11, state.fontSize - 1);
    document.querySelector(".CodeMirror").style.fontSize = state.fontSize + "px";
    state.editor.refresh();
  };
  document.getElementById("btn-font-inc").onclick = () => {
    state.fontSize = Math.min(22, state.fontSize + 1);
    document.querySelector(".CodeMirror").style.fontSize = state.fontSize + "px";
    state.editor.refresh();
  };

  // File upload + drag & drop .cpp
  document.getElementById("cpp-file-input").onchange = (e) => {
    const file = e.target.files?.[0];
    if (!file) return;
    file.text().then((txt) => {
      state.editor.setValue(txt);
      document.getElementById("draft-status").textContent = `Đã mở ${file.name}`;
    });
    e.target.value = "";
  };

  const dz = document.getElementById("editor-dropzone");
  const ov = document.getElementById("drop-overlay");
  dz.addEventListener("dragover", (e) => {
    e.preventDefault();
    ov.classList.remove("hidden");
  });
  dz.addEventListener("dragleave", () => ov.classList.add("hidden"));
  dz.addEventListener("drop", (e) => {
    e.preventDefault();
    ov.classList.add("hidden");
    const f = e.dataTransfer?.files?.[0];
    if (f) {
      f.text().then((txt) => {
        state.editor.setValue(txt);
        document.getElementById("draft-status").textContent = `Đã nạp ${f.name}`;
      });
    }
  });

  // Notes autosave
  document.getElementById("note-textarea").addEventListener("input", () => {
    if (!state.currentProblem) return;
    document.getElementById("note-status").textContent = "Đang lưu ghi chú...";
    clearTimeout(state.noteTimer);
    const pid = state.currentProblem.id;
    const content = document.getElementById("note-textarea").value;
    state.noteTimer = setTimeout(async () => {
      await apiPost("/api/notes", { problem_id: pid, content });
      document.getElementById("note-status").textContent = "Đã lưu ghi chú!";
    }, 500);
  });
}

// ============================================================================
// MOCK EXAM (THI THỬ 5 CÂU / 120 PHÚT THEO ELO & CẤU TRÚC ĐỀ CHUẨN)
// ============================================================================
const EXAM_STORAGE_KEY = "ttud_active_exam_v1";

function formatMMSS(sec) {
  const s = Math.max(0, Math.floor(sec));
  const m = Math.floor(s / 60);
  const r = s % 60;
  return `${String(m).padStart(2, "0")}:${String(r).padStart(2, "0")}`;
}

function saveActiveExam() {
  if (state.activeExam) {
    localStorage.setItem(EXAM_STORAGE_KEY, JSON.stringify(state.activeExam));
  } else {
    localStorage.removeItem(EXAM_STORAGE_KEY);
  }
}

function restoreActiveExam() {
  try {
    const raw = localStorage.getItem(EXAM_STORAGE_KEY);
    if (!raw) return;
    const ex = JSON.parse(raw);
    if (ex && ex.endTimeMs && Array.isArray(ex.problemIds) && ex.problemIds.length === 5) {
      if (Date.now() >= ex.endTimeMs) {
        state.activeExam = ex;
        finishExam(true);
      } else {
        state.activeExam = ex;
        startExamTimerLoop();
      }
    }
  } catch (e) {
    localStorage.removeItem(EXAM_STORAGE_KEY);
  }
}

function startExamTimerLoop() {
  if (state.examTimerInterval) clearInterval(state.examTimerInterval);
  const pill = document.getElementById("nav-exam-timer-pill");
  const tick = () => {
    if (!state.activeExam) {
      if (pill) pill.classList.add("hidden");
      clearInterval(state.examTimerInterval);
      return;
    }
    const remSec = Math.max(0, Math.floor((state.activeExam.endTimeMs - Date.now()) / 1000));
    if (pill) {
      pill.classList.remove("hidden");
      pill.textContent = formatMMSS(remSec);
    }
    const bigTimers = document.querySelectorAll(".js-exam-timer");
    bigTimers.forEach((el) => {
      el.textContent = formatMMSS(remSec);
      el.classList.toggle("urgent", remSec <= 600);
    });
    if (remSec <= 0) {
      finishExam(true);
    }
  };
  tick();
  state.examTimerInterval = setInterval(tick, 1000);
}

function computeExamCurrentTotal(ex) {
  if (!ex || !ex.results) return 0;
  return Object.values(ex.results).reduce((acc, r) => acc + (r.bestScore || 0), 0);
}

function onExamProblemSubmitted(pid, res) {
  if (!state.activeExam) return;
  if (!state.activeExam.problemIds.includes(pid)) return;
  const total = Math.max(1, res.total || 1);
  const passed = res.passed || 0;
  const score = Math.round((2.0 * passed / total) * 100) / 100;
  const cur = state.activeExam.results[pid] || {
    bestScore: 0,
    passed: 0,
    total: total,
    verdict: "",
    attempts: 0,
  };
  cur.attempts += 1;
  if (score > cur.bestScore || (score === cur.bestScore && res.verdict === "AC") || cur.verdict === "") {
    cur.bestScore = score;
    cur.passed = passed;
    cur.total = total;
    cur.verdict = res.verdict;
  }
  state.activeExam.results[pid] = cur;
  saveActiveExam();
  renderArenaExamStrip();
  if (state.currentView === "exam") renderExamView();
}

function renderArenaExamStrip() {
  const strip = document.getElementById("arena-exam-strip");
  if (!strip) return;
  const ex = state.activeExam;
  if (!ex) {
    strip.classList.add("hidden");
    strip.innerHTML = "";
    return;
  }
  strip.classList.remove("hidden");
  const remSec = Math.max(0, Math.floor((ex.endTimeMs - Date.now()) / 1000));
  const totalScore = computeExamCurrentTotal(ex).toFixed(2);
  const pillsHtml = ex.problemIds
    .map((pid, idx) => {
      const r = ex.results[pid] || { bestScore: 0, verdict: "" };
      const isCur = state.currentProblem?.id === pid;
      const stCls = r.bestScore >= 1.99 ? "solved" : r.bestScore > 0 ? "partial" : "";
      return `<button class="exam-prob-pill ${isCur ? "active" : ""} ${stCls}" onclick="openProblem('${pid}')">
        Câu ${idx + 1} (${(r.bestScore || 0).toFixed(1)}/2.0đ)
      </button>`;
    })
    .join("");

  strip.innerHTML = `
    <div style="display:flex;align-items:center;gap:10px;flex-wrap:wrap;">
      <span class="badge ${ex.track === "midterm" ? "badge-midterm" : "badge-final"}">${ex.track === "midterm" ? "Thi thử Giữa kỳ" : "Thi thử Cuối kỳ"}</span>
      <strong>${esc(ex.title)}</strong>
      <div class="exam-prob-pills">${pillsHtml}</div>
    </div>
    <div style="display:flex;align-items:center;gap:10px;">
      <span>Điểm tạm tính: <strong>${totalScore} / 10.0</strong></span>
      <span class="exam-timer-big js-exam-timer ${remSec <= 600 ? "urgent" : ""}">${formatMMSS(remSec)}</span>
      <button class="btn btn-sm btn-ghost" onclick="switchView('exam'); renderExamView();">Bảng đề</button>
      <button class="btn btn-sm btn-primary" onclick="confirmFinishExam()">Nộp đề &amp; Kết thúc</button>
    </div>
  `;
}

window.startPresetExam = function (presetId) {
  if (state.activeExam && !confirm("Bạn đang có một bài thi thử chưa nộp. Bắt đầu đề mới sẽ hủy đề đang làm. Tiếp tục?")) {
    return;
  }
  const presets = state.bootstrap?.mock_exams?.presets || [];
  const p = presets.find((x) => x.id === presetId);
  if (!p) return;
  launchExamSession(p.id, p.title, p.track, p.problems, p.teacher_intro);
};

window.startRandomExam = function (track) {
  if (state.activeExam && !confirm("Bạn đang có một bài thi thử chưa nộp. Bắt đầu đề mới sẽ hủy đề đang làm. Tiếp tục?")) {
    return;
  }
  const allProbs = state.bootstrap?.problems || [];
  // Strict rule: Midterm ONLY has track === 'midterm' (Chapters 1 to 4B, NEVER Graph)
  const pool = track === "midterm"
    ? allProbs.filter((p) => p.track === "midterm" && p.theory_chapter !== "chap5" && !p.topic.startsWith("graph"))
    : allProbs;

  const picked = new Set();
  const pickOne = (filterFn, fallbackFn) => {
    let cands = pool.filter((p) => !picked.has(p.id) && filterFn(p));
    if (!cands.length && fallbackFn) cands = pool.filter((p) => !picked.has(p.id) && fallbackFn(p));
    if (!cands.length) cands = pool.filter((p) => !picked.has(p.id));
    const unsolved = cands.filter((p) => !p.status?.solved);
    const usePool = unsolved.length ? unsolved : cands;
    const chosen = usePool[Math.floor(Math.random() * usePool.length)];
    if (chosen) picked.add(chosen.id);
    return chosen?.id;
  };

  // Slot 1: Easy warm-up (Elo 800..1050)
  const q1 = pickOne(
    (p) => p.elo <= 1050 && ["stl-basic", "prefix-sum", "two-pointers"].includes(p.topic),
    (p) => p.elo <= 1100
  );
  // Slot 2: Easy-Medium Data structures / Prefix sum / Two pointers / Greedy / Basic Graph (if final)
  const q2 = pickOne(
    (p) => p.elo >= 950 && p.elo <= 1300 && (track === "midterm"
      ? ["prefix-sum", "two-pointers", "greedy", "stl-basic"].includes(p.topic)
      : ["prefix-sum", "two-pointers", "stl-basic", "graph-traversal", "graph-shortest-mst"].includes(p.topic)),
    (p) => p.elo >= 950 && p.elo <= 1350
  );
  // Slot 3: Backtracking (Elo 1050..1550)
  const q3 = pickOne(
    (p) => ["backtrack-gen", "branch-bound"].includes(p.topic) && p.elo >= 1050 && p.elo <= 1550,
    (p) => ["backtrack-gen", "branch-bound"].includes(p.topic)
  );
  // Slot 4: Dynamic Programming (Elo 1100..1650)
  const q4 = pickOne(
    (p) => ["dp-1d", "dp-knapsack", "dp-subseq", "dp-grid"].includes(p.topic) && p.elo >= 1100 && p.elo <= 1650,
    (p) => p.theory_chapter === "chap4b"
  );
  // Slot 5: Super Hard (Elo >= 1550)
  const q5 = pickOne(
    (p) => track === "midterm"
      ? p.elo >= 1550 && ["branch-bound", "dp-knapsack", "dp-grid", "dp-subseq", "dp-1d", "divide-conquer"].includes(p.topic)
      : p.elo >= 1600 && ["graph-advanced", "graph-traversal", "graph-shortest-mst", "dp-grid", "dp-knapsack"].includes(p.topic),
    (p) => p.elo >= 1500
  );

  const pids = [q1, q2, q3, q4, q5].filter(Boolean);
  const pmap = getProblemMap();
  const avgElo = Math.round(pids.reduce((s, id) => s + (pmap[id]?.elo || 1200), 0) / pids.length);
  const title = track === "midterm"
    ? `Đề Giữa kỳ Ngẫu nhiên theo Elo (TB Elo ${avgElo})`
    : `Đề Cuối kỳ Ngẫu nhiên theo Elo (TB Elo ${avgElo})`;
  const intro = track === "midterm"
    ? "Đề sinh tự động đúng chuẩn Giữa kỳ (Chương 1 đến hết Quy hoạch động, không có Đồ thị): 2 câu đầu khởi động & cấu trúc dữ liệu, câu 3 Quay lui, câu 4 Quy hoạch động, câu 5 phân loại điểm 9–10."
    : "Đề sinh tự động đúng chuẩn Cuối kỳ: 2 câu đầu ăn điểm nền tảng/đồ thị cơ bản, câu 3 Quay lui/Nhánh cận, câu 4 Quy hoạch động, câu 5 Đồ thị nâng cao / Bitmask DP.";
  launchExamSession(`rand-${track}-${Date.now()}`, title, track, pids, intro);
};

function launchExamSession(examId, title, track, problemIds, teacherIntro) {
  const now = Date.now();
  const durationSec = 7200; // 120 minutes
  const results = {};
  problemIds.forEach((pid) => {
    results[pid] = { bestScore: 0, passed: 0, total: 0, verdict: "", attempts: 0 };
  });
  state.activeExam = {
    examId,
    title,
    track,
    teacherIntro: teacherIntro || "",
    problemIds,
    startTimeMs: now,
    endTimeMs: now + durationSec * 1000,
    durationSec,
    results,
  };
  state.lastFinishedExam = null;
  saveActiveExam();
  startExamTimerLoop();
  renderExamView();
  if (problemIds[0]) {
    openProblem(problemIds[0]);
  }
}

window.confirmFinishExam = function () {
  if (!state.activeExam) return;
  const total = computeExamCurrentTotal(state.activeExam).toFixed(2);
  if (confirm(`Nộp đề thi thử ngay bây giờ? Điểm hiện tại của bạn là ${total} / 10.0 điểm.`)) {
    finishExam(false);
  }
};

function buildTeacherFeedback(ex, totalScore, pmap) {
  const lines = [];
  const rList = ex.problemIds.map((pid, idx) => ({
    slot: idx + 1,
    prob: pmap[pid],
    res: ex.results[pid] || { bestScore: 0, passed: 0, total: 0, verdict: "" },
  }));

  const easyScore = (rList[0]?.res.bestScore || 0) + (rList[1]?.res.bestScore || 0);
  const btScore = rList[2]?.res.bestScore || 0;
  const dpScore = rList[3]?.res.bestScore || 0;
  const hardScore = rList[4]?.res.bestScore || 0;

  if (totalScore >= 8.5) {
    lines.push("- **Đánh giá tổng quan (Mức A / A+)**: Bạn xử lý trọn vẹn 4 câu đầu và ăn được điểm ở câu 5 phân loại. Tốc độ nhận diện dạng bài và độ chính xác khi cài đặt C++ của bạn đã sẵn sàng cho phòng thi thật.");
  } else if (totalScore >= 6.0) {
    lines.push("- **Đánh giá tổng quan (Mức B / B+)**: Bạn đã nắm được nền tảng nhưng vẫn đánh rơi điểm ở các câu trụ cột (Quay lui hoặc Quy hoạch động) hoặc bị mất test biên.");
  } else if (totalScore >= 4.0) {
    lines.push("- **Đánh giá tổng quan (Mức C / Đạt ngưỡng qua môn)**: Bạn mới ăn được điểm ở nhóm câu dễ hoặc một phần test nhỏ. Cần củng cố ngay khung Quay lui `Try(k)` và 5 bước Quy hoạch động.");
  } else {
    lines.push("- **Đánh giá tổng quan (Cần báo động)**: Bạn đang mất điểm ngay từ 2 câu đầu tiên. Đừng vội học câu khó, hãy quay lại Chặng 1–4 để nắm chắc Fast I/O, `unordered_map`, Mảng cộng dồn và Hai con trỏ trước.");
  }

  if (easyScore < 3.5) {
    lines.push("- **Câu 1 & Câu 2 (Nhóm lấy 4 điểm đầu trong 30 phút)**: Bạn mới đạt **" + easyScore.toFixed(2) + "/4.0đ**. Khi đi thi, 2 câu này bắt buộc phải AC 100% để giữ tâm lý. Hãy kiểm tra xem bạn bị TLE do dùng 2 vòng `for` (quên dùng Mảng cộng dồn / `unordered_map` / Hai con trỏ) hay bị WA do quên `long long`.");
  } else {
    lines.push("- **Câu 1 & Câu 2**: Đạt **" + easyScore.toFixed(2) + "/4.0đ** — phản xạ tốt với nhóm bài cơ bản và cấu trúc dữ liệu.");
  }

  if (btScore < 1.5) {
    lines.push(`- **Câu 3 — Quay lui (${rList[2]?.prob?.title || ""})**: Đạt **${btScore.toFixed(2)}/2.0đ**. Hãy mở lại bài giảng **Chương 3**, xem kỹ khung \`Try(k)\`, điều kiện dừng và nhớ hoàn tác trạng thái ngay sau lời gọi đệ quy.`);
  } else {
    lines.push(`- **Câu 3 — Quay lui**: Đạt **${btScore.toFixed(2)}/2.0đ** — nắm vững khung duyệt đệ quy và cắt nhánh.`);
  }

  if (dpScore < 1.5) {
    lines.push(`- **Câu 4 — Quy hoạch động (${rList[3]?.prob?.title || ""})**: Đạt **${dpScore.toFixed(2)}/2.0đ**. Hãy mở tab **Hướng dẫn giải** của bài này, tự viết lại ý nghĩa của mảng \`dp[...]\` ra giấy và kiểm tra giá trị khởi tạo cơ sở.`);
  } else {
    lines.push(`- **Câu 4 — Quy hoạch động**: Đạt **${dpScore.toFixed(2)}/2.0đ** — xây dựng đúng công thức chuyển trạng thái.`);
  }

  if (hardScore < 1.0) {
    lines.push(`- **Câu 5 — Phân loại điểm 9–10 (${rList[4]?.prob?.title || ""})**: Đạt **${hardScore.toFixed(2)}/2.0đ**. Với câu siêu khó trong phòng thi, nếu chưa nghĩ ra thuật toán tối ưu hoàn toàn, hãy luôn viết nhánh cận hoặc vét cạn cho \`n\` nhỏ để lấy 30%–50% số test.`);
  } else {
    lines.push(`- **Câu 5 — Phân loại điểm 9–10**: Đạt **${hardScore.toFixed(2)}/2.0đ** — ăn được điểm ở câu khó nhất đề.`);
  }

  return lines.join("\n");
}

async function finishExam(autoTimeout = false) {
  const ex = state.activeExam;
  if (!ex) return;
  const pmap = getProblemMap();
  const usedSec = Math.min(ex.durationSec, Math.max(1, Math.floor((Date.now() - ex.startTimeMs) / 1000)));
  const totalScore = Math.round(computeExamCurrentTotal(ex) * 100) / 100;
  const feedback = buildTeacherFeedback(ex, totalScore, pmap);
  const probDetails = ex.problemIds.map((pid, idx) => {
    const r = ex.results[pid] || { bestScore: 0, passed: 0, total: 0, verdict: "", attempts: 0 };
    return {
      slot: idx + 1,
      id: pid,
      title: pmap[pid]?.title || pid,
      elo: pmap[pid]?.elo || 1200,
      topic: pmap[pid]?.topic || "",
      score: r.bestScore || 0,
      passed: r.passed || 0,
      total: r.total || pmap[pid]?.test_count || 0,
      verdict: r.verdict || "Chưa nộp",
      attempts: r.attempts || 0,
    };
  });

  state.activeExam = null;
  saveActiveExam();
  if (state.examTimerInterval) clearInterval(state.examTimerInterval);
  const pill = document.getElementById("nav-exam-timer-pill");
  if (pill) pill.classList.add("hidden");
  renderArenaExamStrip();

  // Re-unlock guide in arena if a problem is open
  if (state.currentProblem) {
    const mindsetTabBtn = document.querySelector('#left-subtabs .sub-tab[data-ltab="mindset"]');
    const testsTabBtn = document.querySelector('#left-subtabs .sub-tab[data-ltab="tests"]');
    const theoryBtn = document.getElementById("btn-open-theory");
    if (mindsetTabBtn) mindsetTabBtn.innerHTML = "Hướng dẫn giải";
    if (testsTabBtn) testsTabBtn.classList.remove("hidden");
    if (theoryBtn) theoryBtn.classList.remove("hidden");
    drawGuideLadder();
  }

  try {
    const saved = await apiPost("/api/exam/finish", {
      exam_id: ex.examId,
      title: ex.title,
      track: ex.track,
      total_score: totalScore,
      duration_used_sec: usedSec,
      problems: probDetails,
      feedback,
    });
    if (saved.record) {
      state.bootstrap.exam_history = [saved.record, ...(state.bootstrap.exam_history || [])];
      state.lastFinishedExam = saved.record;
    }
  } catch (e) {
    state.lastFinishedExam = {
      title: ex.title,
      track: ex.track,
      total_score: totalScore,
      duration_used_sec: usedSec,
      problems: probDetails,
      feedback,
      created_at: "Vừa xong",
    };
  }

  if (autoTimeout) {
    alert("Đã hết 120 phút làm bài! Hệ thống tự động thu bài và mở khóa lời giải chi tiết.");
  }
  switchView("exam");
  renderExamView();
}

window.viewExamHistoryRecord = function (idx) {
  const rec = state.bootstrap?.exam_history?.[idx];
  if (!rec) return;
  state.lastFinishedExam = rec;
  renderExamView();
  document.getElementById("exam-root")?.scrollIntoView({ behavior: "smooth", block: "start" });
};

function renderExamView() {
  const root = document.getElementById("exam-root");
  if (!root || !state.bootstrap) return;
  const pmap = getProblemMap();
  const mockData = state.bootstrap.mock_exams || {};
  const rules = mockData.exam_rules || {};
  const presets = mockData.presets || [];
  const history = state.bootstrap.exam_history || [];

  // 1. Active Exam Banner if currently in an exam
  let activeHtml = "";
  if (state.activeExam) {
    const ex = state.activeExam;
    const remSec = Math.max(0, Math.floor((ex.endTimeMs - Date.now()) / 1000));
    const totalScore = computeExamCurrentTotal(ex).toFixed(2);
    const rowsHtml = ex.problemIds
      .map((pid, idx) => {
        const p = pmap[pid] || { id: pid, title: pid, elo: 1200, topic: "" };
        const r = ex.results[pid] || { bestScore: 0, passed: 0, total: p.test_count || 0, verdict: "", attempts: 0 };
        const slotMeta = rules.slots?.[idx] || { label: `Câu ${idx + 1}` };
        const vBadge = r.verdict
          ? `<span class="badge verdict-${esc(r.verdict)}">${esc(r.verdict)} (${r.passed}/${r.total})</span>`
          : `<span class="muted">Chưa nộp</span>`;
        return `
          <tr>
            <td><strong>Câu ${idx + 1}</strong><div class="muted small">${esc(slotMeta.label.split("·")[1] || "")}</div></td>
            <td>
              <div style="font-weight:600;cursor:pointer;" onclick="openProblem('${pid}')">${esc(p.title)}</div>
              <div class="muted small"><code>${esc(pid)}</code> · ${p.time_limit_ms || 1000} ms</div>
            </td>
            <td><span class="badge badge-elo">Elo ${p.elo || 1200}</span></td>
            <td>${vBadge}</td>
            <td><strong>${(r.bestScore || 0).toFixed(2)} / 2.00</strong></td>
            <td style="text-align:right;"><button class="btn btn-sm btn-primary" onclick="openProblem('${pid}')">Làm Câu ${idx + 1} →</button></td>
          </tr>`;
      })
      .join("");

    activeHtml = `
      <div class="surface hero-card" style="border-color:var(--amber-border);margin-bottom:18px;">
        <div style="display:flex;justify-content:space-between;align-items:flex-start;flex-wrap:wrap;gap:12px;">
          <div>
            <span class="badge ${ex.track === "midterm" ? "badge-midterm" : "badge-final"}">${ex.track === "midterm" ? "Đang thi Giữa kỳ (Không Graph)" : "Đang thi Cuối kỳ"}</span>
            <h2 style="margin:6px 0 4px;">${esc(ex.title)}</h2>
            <p class="muted small-prose" style="margin:0;">${esc(ex.teacherIntro)}</p>
          </div>
          <div style="display:flex;align-items:center;gap:12px;">
            <div style="text-align:right;">
              <div class="muted small">Điểm hiện tại</div>
              <strong style="font-size:18px;">${totalScore} / 10.0</strong>
            </div>
            <div class="exam-timer-big js-exam-timer ${remSec <= 600 ? "urgent" : ""}">${formatMMSS(remSec)}</div>
            <button class="btn btn-primary" onclick="confirmFinishExam()">Nộp đề &amp; Kết thúc</button>
          </div>
        </div>
        <div class="problem-table-wrap" style="margin-top:14px;">
          <table class="problem-table">
            <thead>
              <tr>
                <th>Vị trí</th>
                <th>Đề bài</th>
                <th>Mức Elo</th>
                <th>Trạng thái chấm</th>
                <th>Điểm (Thang 10)</th>
                <th style="text-align:right;">Vào phòng code</th>
              </tr>
            </thead>
            <tbody>${rowsHtml}</tbody>
          </table>
        </div>
      </div>`;
  }

  // 2. Post-Exam Review Card if just finished or viewing history
  let reviewHtml = "";
  if (state.lastFinishedExam) {
    const rec = state.lastFinishedExam;
    const mins = Math.max(1, Math.round((rec.duration_used_sec || 0) / 60));
    const probRows = (rec.problems || [])
      .map((item, idx) => {
        const p = pmap[item.id] || item;
        const vCls = item.verdict === "AC" ? "verdict-AC" : item.score > 0 ? "verdict-TLE" : "verdict-WA";
        return `
          <tr>
            <td><strong>Câu ${item.slot || idx + 1}</strong></td>
            <td>
              <div style="font-weight:600;cursor:pointer;" onclick="openProblem('${item.id}')">${esc(item.title)}</div>
              <div class="muted small">${esc(TOPIC_LABELS[p.topic] || item.topic || "")}</div>
            </td>
            <td><span class="badge badge-elo">Elo ${item.elo || p.elo || 1200}</span></td>
            <td><span class="badge ${vCls}">${esc(item.verdict)} (${item.passed}/${item.total})</span></td>
            <td><strong>${Number(item.score || 0).toFixed(2)} / 2.00</strong></td>
            <td style="text-align:right;">
              <button class="btn btn-sm btn-ghost" onclick="openProblem('${item.id}')">Xem Hướng dẫn &amp; Code mẫu →</button>
            </td>
          </tr>`;
      })
      .join("");

    reviewHtml = `
      <div class="surface hero-card" style="border-color:var(--green-border);margin-bottom:18px;">
        <div style="display:flex;justify-content:space-between;align-items:center;flex-wrap:wrap;gap:12px;">
          <div>
            <span class="badge verdict-AC">Kết quả &amp; Nhận xét của Thầy</span>
            <h2 style="margin:6px 0 2px;">${esc(rec.title)} — Đạt ${Number(rec.total_score).toFixed(2)} / 10.0 điểm</h2>
            <div class="muted small">Thời gian làm bài: ${mins} phút · Ngày nộp: ${esc(rec.created_at || "")}</div>
          </div>
          <button class="btn btn-sm btn-ghost" onclick="state.lastFinishedExam = null; renderExamView();">Đóng bảng kết quả</button>
        </div>
        <div class="callout callout-tip" style="margin-top:12px;">
          <h4>Nhận xét &amp; Hướng khắc phục sau bài thi</h4>
          <div class="prose">${renderProse(rec.feedback)}</div>
        </div>
        <div class="problem-table-wrap" style="margin-top:12px;">
          <table class="problem-table">
            <thead>
              <tr>
                <th>Câu</th>
                <th>Bài toán &amp; Dạng bài</th>
                <th>Elo</th>
                <th>Kết quả</th>
                <th>Điểm</th>
                <th style="text-align:right;">Chữa bài (Đã mở khóa lời giải)</th>
              </tr>
            </thead>
            <tbody>${probRows}</tbody>
          </table>
        </div>
      </div>`;
  }

  // 3. Exam selection & Random generator
  const filteredPresets = presets.filter((p) => state.examFilterTrack === "all" || p.track === state.examFilterTrack);
  const slotsHtml = (rules.slots || [])
    .map(
      (s) => `
      <div class="exam-slot-card">
        <div style="display:flex;justify-content:space-between;align-items:center;margin-bottom:4px;">
          <span class="badge badge-elo">Elo ${esc(s.elo_range)}</span>
          <span class="muted small">~${s.target_minutes} phút</span>
        </div>
        <h4>${esc(s.label)}</h4>
        <div class="muted small">${esc(s.desc)}</div>
      </div>`
    )
    .join("");

  const presetsHtml = filteredPresets
    .map((pr) => {
      const probList = pr.problems
        .map((pid, idx) => {
          const p = pmap[pid];
          if (!p) return "";
          return `<div class="small" style="display:flex;justify-content:space-between;padding:3px 0;border-bottom:1px dashed var(--border-subtle);">
            <span><strong>Câu ${idx + 1}:</strong> ${esc(p.title)}</span>
            <span class="muted mono-meta">Elo ${p.elo || 1200}</span>
          </div>`;
        })
        .join("");

      return `
        <div class="exam-preset-card">
          <div>
            <div style="display:flex;justify-content:space-between;align-items:center;gap:8px;margin-bottom:8px;">
              <span class="badge ${pr.track === "midterm" ? "badge-midterm" : "badge-final"}">${pr.track === "midterm" ? "Giữa kỳ (Không Graph)" : "Cuối kỳ"}</span>
              <span class="badge badge-elo">Elo TB ~${pr.avg_elo}</span>
            </div>
            <h3 style="margin:0 0 6px;font-size:15.5px;">${esc(pr.title)}</h3>
            <p class="muted small" style="margin:0 0 10px;">${esc(pr.teacher_intro)}</p>
            <div style="margin-bottom:14px;">${probList}</div>
          </div>
          <button class="btn btn-primary" style="width:100%;" onclick="startPresetExam('${pr.id}')">Bắt đầu làm đề này (120 phút)</button>
        </div>`;
    })
    .join("");

  const historyRows = history.length
    ? history
        .map(
          (h, idx) => `
        <tr>
          <td><span class="badge ${h.track === "midterm" ? "badge-midterm" : "badge-final"}">${h.track === "midterm" ? "Giữa kỳ" : "Cuối kỳ"}</span></td>
          <td><strong>${esc(h.title)}</strong></td>
          <td><strong style="color:var(--green);">${Number(h.total_score).toFixed(2)} / 10.0</strong></td>
          <td>${Math.max(1, Math.round((h.duration_used_sec || 0) / 60))} phút</td>
          <td class="muted small">${esc(h.created_at)}</td>
          <td style="text-align:right;"><button class="btn btn-sm btn-ghost" onclick="viewExamHistoryRecord(${idx})">Xem nhận xét &amp; Chữa bài</button></td>
        </tr>`
        )
        .join("")
    : `<tr><td colspan="6" class="muted center-empty">Bạn chưa hoàn thành đề thi thử nào. Hãy chọn một đề chuẩn bên trên hoặc bấm Sinh đề ngẫu nhiên!</td></tr>`;

  root.innerHTML = `
    ${activeHtml}
    ${reviewHtml}
    <div class="surface hero-card">
      <div class="hero-head">
        <div class="hero-title-wrap">
          <div class="eyebrow">Phòng Thi thử 120 Phút · Chấm điểm thành phần theo Test Case (Thang 10)</div>
          <h1>Đề Thi thử Giữa kỳ &amp; Cuối kỳ theo Cấu trúc Elo</h1>
          <p class="muted prose-lead">Mỗi đề thi gồm đúng <strong>5 câu làm trong 120 phút</strong> (mỗi câu 2.0 điểm, tính điểm theo tỷ lệ test case đúng): <strong>2 câu đầu dễ</strong> để lấy chắc 4 điểm trong 30 phút đầu, <strong>Câu 3 Quay lui</strong>, <strong>Câu 4 Quy hoạch động</strong>, và <strong>Câu 5 Siêu khó</strong> để phân loại điểm 9–10. Trong lúc thi, toàn bộ gợi ý và lời giải bị khóa. Đặc biệt: <strong>Đề Giữa kỳ chỉ giới hạn đến hết Quy hoạch động, tuyệt đối không có Đồ thị (Graph)</strong>.</p>
        </div>
        <div style="display:flex;gap:8px;flex-wrap:wrap;">
          <button class="btn btn-primary" onclick="startRandomExam('midterm')">🎲 Sinh đề Giữa kỳ ngẫu nhiên (Không Graph)</button>
          <button class="btn btn-secondary" onclick="startRandomExam('final')">🎲 Sinh đề Cuối kỳ ngẫu nhiên</button>
        </div>
      </div>

      <div class="exam-slot-grid">${slotsHtml}</div>

      <div style="display:flex;justify-content:space-between;align-items:center;flex-wrap:wrap;gap:12px;margin-top:20px;">
        <h3 style="margin:0;">Bộ 12 Đề Chuẩn do Thầy biên soạn (Cân đối Elo &amp; Thời lượng 120 phút)</h3>
        <div class="track-pills">
          <button class="filter-pill ${state.examFilterTrack === "midterm" ? "active" : ""}" onclick="state.examFilterTrack='midterm'; renderExamView();">Đề Giữa kỳ (6 đề · Không Graph)</button>
          <button class="filter-pill ${state.examFilterTrack === "final" ? "active" : ""}" onclick="state.examFilterTrack='final'; renderExamView();">Đề Cuối kỳ (6 đề)</button>
          <button class="filter-pill ${state.examFilterTrack === "all" ? "active" : ""}" onclick="state.examFilterTrack='all'; renderExamView();">Tất cả (12 đề)</button>
        </div>
      </div>
      <div class="exam-presets-grid">${presetsHtml}</div>
    </div>

    <div class="surface hero-card" style="margin-top:18px;">
      <h3 style="margin:0 0 12px;">Lịch sử các lần Thi thử của bạn</h3>
      <div class="problem-table-wrap">
        <table class="problem-table">
          <thead>
            <tr>
              <th>Kỳ thi</th>
              <th>Tên đề</th>
              <th>Điểm đạt được</th>
              <th>Thời gian làm</th>
              <th>Thời điểm nộp</th>
              <th style="text-align:right;">Chi tiết</th>
            </tr>
          </thead>
          <tbody>${historyRows}</tbody>
        </table>
      </div>
    </div>
  `;
}

async function boot() {
  initEditor();
  bindEvents();
  state.bootstrap = await apiGet("/api/bootstrap");
  applyTheme(state.bootstrap.settings?.theme || "dark");
  restoreActiveExam();
  updateTopStats();
  renderRoadmap();
  renderTheorySidebar();
  renderTheoryContent();
  renderProblemBankFilters();
  renderProblemTable();
  renderExamView();

  // Preload Day 1 first problem cleanly in Arena background
  const firstPid = state.bootstrap.roadmap.days[0]?.problems[0];
  if (firstPid) {
    // Load metadata without leaving roadmap view
    const prob = await apiGet(`/api/problem/${encodeURIComponent(firstPid)}`);
    state.currentProblem = prob;
  }
}

boot().catch((err) => {
  console.error("Boot failed:", err);
  alert("Không thể kết nối server local: " + err.message);
});

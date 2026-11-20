/* Interactive Algorithm Visualizer for TTUD (Lec 1 - Lec 10) */
(function () {
  const vizState = {
    tab: "backtracking",
    steps: [],
    idx: 0,
    timer: null,
    speedMs: 650,
    initialized: false,
  };

  function stopAuto() {
    if (vizState.timer) {
      clearInterval(vizState.timer);
      vizState.timer = null;
    }
    const btn = document.getElementById("viz-play-btn");
    if (btn) btn.textContent = "Tự chạy";
  }

  function toggleAuto() {
    if (vizState.timer) {
      stopAuto();
      return;
    }
    if (vizState.idx >= vizState.steps.length - 1) {
      vizState.idx = 0;
    }
    const btn = document.getElementById("viz-play-btn");
    if (btn) btn.textContent = "Tạm dừng";
    vizState.timer = setInterval(() => {
      if (vizState.idx < vizState.steps.length - 1) {
        vizState.idx++;
        renderCurrentStep();
      } else {
        stopAuto();
      }
    }, vizState.speedMs);
  }

  function escHtml(s) {
    return String(s == null ? "" : s)
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;");
  }

  /* ==================== 1. PREFIX SUM & SLIDING WINDOW ==================== */
  function buildPrefixWindowSteps(arr, Q) {
    const n = arr.length;
    const S = [0];
    for (let i = 0; i < n; i++) S.push(S[i] + arr[i]);
    const steps = [];
    let L = 0;
    let curSum = 0;
    let bestLen = 0;
    let bestRange = [-1, -1];

    steps.push({
      type: "prefix-window",
      arr, S, Q, L: 0, R: -1, curSum: 0, bestLen: 0, bestRange: [-1, -1],
      action: "Khởi tạo mảng cộng dồn S[0..n] và L = 0, R = -1, curSum = 0."
    });

    for (let R = 0; R < n; R++) {
      curSum += arr[R];
      steps.push({
        type: "prefix-window",
        arr, S, Q, L, R, curSum, bestLen, bestRange: [...bestRange],
        action: `Mở rộng R = ${R} (+${arr[R]}): curSum = ${curSum} (tính nhanh bằng S[${R + 1}] - S[${L}] = ${S[R + 1] - S[L]}).`
      });
      while (curSum > Q && L <= R) {
        curSum -= arr[L];
        const oldL = L;
        L++;
        steps.push({
          type: "prefix-window",
          arr, S, Q, L, R, curSum, bestLen, bestRange: [...bestRange],
          action: `curSum > ${Q} → Thu hẹp L từ ${oldL} lên ${L} (-${arr[oldL]}), còn curSum = ${curSum}.`
        });
      }
      if (R - L + 1 > bestLen) {
        bestLen = R - L + 1;
        bestRange = [L, R];
        steps.push({
          type: "prefix-window",
          arr, S, Q, L, R, curSum, bestLen, bestRange: [...bestRange],
          action: `Cập nhật kỷ lục mới: đoạn [${L}..${R}] độ dài ${bestLen} (tổng ${curSum} <= ${Q}).`
        });
      }
    }
    return steps;
  }

  /* ==================== 2. BACKTRACKING & BRANCH-AND-BOUND (CBUS n=2, K=1) ==================== */
  function buildBacktrackingSteps(mode) {
    const steps = [];
    if (mode === "subset") {
      const a = [3, 5, 2, 4];
      const M = 7;
      const nodes = [];
      let nextId = 0;
      let solutions = 0;

      function dfs(i, curSum, chosen, parentId, depth) {
        const id = nextId++;
        const node = { id, parentId, depth, label: i === 0 ? "Gốc" : `x[${i - 1}]=${chosen[chosen.length - 1] ? 1 : 0}`, sum: curSum, status: "active" };
        nodes.push(node);

        if (curSum > M) {
          node.status = "pruned";
          node.reason = `Cắt nhánh: sum=${curSum} > ${M}`;
          steps.push({
            type: "backtracking", mode, a, M, nodes: JSON.parse(JSON.stringify(nodes)),
            activeId: id, chosen: [...chosen], solutions,
            action: `Bước ${i}: tổng = ${curSum} > ${M} → CẮT NHÁNH (Pruning)!`
          });
          return;
        }
        if (i === a.length) {
          if (curSum === M) {
            node.status = "solution";
            solutions++;
            node.reason = "Đạt tổng = " + M;
            steps.push({
              type: "backtracking", mode, a, M, nodes: JSON.parse(JSON.stringify(nodes)),
              activeId: id, chosen: [...chosen], solutions,
              action: `Nghiệm #${solutions}: chọn [${a.filter((_, idx) => chosen[idx]).join(", ")}] = ${M}!`
            });
          } else {
            node.status = "dead";
            steps.push({
              type: "backtracking", mode, a, M, nodes: JSON.parse(JSON.stringify(nodes)),
              activeId: id, chosen: [...chosen], solutions,
              action: `Lá: tổng = ${curSum} != ${M}, quay lui.`
            });
          }
          return;
        }

        steps.push({
          type: "backtracking", mode, a, M, nodes: JSON.parse(JSON.stringify(nodes)),
          activeId: id, chosen: [...chosen], solutions,
          action: `Đang xét phần tử a[${i}] = ${a[i]} (tổng hiện tại = ${curSum}). Thử chọn 1 rồi 0.`
        });

        chosen.push(1);
        dfs(i + 1, curSum + a[i], chosen, id, depth + 1);
        chosen.pop();

        chosen.push(0);
        dfs(i + 1, curSum, chosen, id, depth + 1);
        chosen.pop();
        node.status = "done";
      }
      dfs(0, 0, [], null, 0);
      return steps;
    }

    // CBUS n=2, K=1 (5 điểm: 0=bến, 1,2=đón, 3,4=trả)
    const n = 2, K = 1;
    const d = [
      [0, 3, 7, 4, 6],
      [3, 0, 5, 2, 4],
      [7, 5, 0, 6, 3],
      [4, 2, 6, 0, 5],
      [6, 4, 3, 5, 0]
    ];
    let cmin = 999;
    for (let i = 0; i <= 2 * n; i++)
      for (let j = 0; j <= 2 * n; j++)
        if (i !== j) cmin = Math.min(cmin, d[i][j]);

    let fBest = 999;
    let bestPath = [];
    const visited = [true, false, false, false, false];
    const x = [0];
    const nodes = [];
    let nextId = 0;

    function tryCbus(k, load, f, parentId) {
      const id = nextId++;
      const rem = 2 * n + 1 - k + 1;
      const g = f + cmin * rem;
      const node = {
        id, parentId, depth: k - 1,
        label: k === 1 ? "0" : `${x[k - 1]} (f=${f})`,
        sum: f, g, status: "active"
      };
      nodes.push(node);

      if (k > 2 * n) {
        const total = f + d[x[2 * n]][0];
        if (total < fBest) {
          fBest = total;
          bestPath = [...x, 0];
          node.status = "solution";
          node.reason = `f*=${fBest}`;
          steps.push({
            type: "backtracking", mode: "cbus", d, cmin, fBest, bestPath: [...bestPath],
            nodes: JSON.parse(JSON.stringify(nodes)), activeId: id,
            action: `Hoàn thành lộ trình [${bestPath.join(" → ")}], cập nhật kỷ lục f* = ${fBest}!`
          });
        } else {
          node.status = "dead";
          steps.push({
            type: "backtracking", mode: "cbus", d, cmin, fBest, bestPath: [...bestPath],
            nodes: JSON.parse(JSON.stringify(nodes)), activeId: id,
            action: `Hoàn thành [${[...x, 0].join(" → ")}] tổng ${total} >= f* (${fBest}).`
          });
        }
        return;
      }

      steps.push({
        type: "backtracking", mode: "cbus", d, cmin, fBest, bestPath: [...bestPath],
        nodes: JSON.parse(JSON.stringify(nodes)), activeId: id,
        action: `Bước k=${k}: đang ở ${x[k - 1]}, f=${f}, load=${load}/${K}, C_min=${cmin}.`
      });

      for (let v = 1; v <= 2 * n; v++) {
        if (visited[v]) continue;
        if (v > n && !visited[v - n]) continue;
        if (v <= n && load + 1 > K) continue;

        const nf = f + d[x[k - 1]][v];
        const nrem = 2 * n + 1 - k;
        const ng = nf + cmin * nrem;
        if (ng >= fBest) {
          const pid = nextId++;
          nodes.push({
            id: pid, parentId: id, depth: k,
            label: `${v} (g=${ng})`, sum: nf, g: ng, status: "pruned", reason: `${ng} >= ${fBest}`
          });
          steps.push({
            type: "backtracking", mode: "cbus", d, cmin, fBest, bestPath: [...bestPath],
            nodes: JSON.parse(JSON.stringify(nodes)), activeId: pid,
            action: `CẮT NHÁNH đi ${v}: cận dưới g = ${nf} + ${cmin}×${nrem} = ${ng} >= f* (${fBest})!`
          });
          continue;
        }
        visited[v] = true;
        x[k] = v;
        tryCbus(k + 1, load + (v <= n ? 1 : -1), nf, id);
        visited[v] = false;
      }
      if (node.status === "active") node.status = "done";
    }

    tryCbus(1, 0, 0, null);
    return steps;
  }

  /* ==================== 3. DP TABLE (MONEY CHANGE) ==================== */
  function buildDpTableSteps(coins, X) {
    const n = coins.length;
    const INF = 999;
    const dp = Array.from({ length: n + 1 }, () => Array(X + 1).fill(INF));
    for (let i = 0; i <= n; i++) dp[i][0] = 0;

    const steps = [{
      type: "dp-table", coins, X, dp: dp.map(r => [...r]), cur: null, deps: [], traceCells: [],
      action: `Khởi tạo dp[i][0] = 0 và các ô khác = ∞ (INF).`
    }];

    for (let i = 1; i <= n; i++) {
      const c = coins[i - 1];
      for (let x = 1; x <= X; x++) {
        const deps = [[i - 1, x]];
        let best = dp[i - 1][x];
        let msg = `dp[${i}][${x}] (mệnh giá ${c}): không dùng ${c} → dp[${i - 1}][${x}]=${dp[i - 1][x] >= INF ? "∞" : dp[i - 1][x]}`;
        if (x >= c) {
          deps.push([i, x - c]);
          const take = dp[i][x - c] >= INF ? INF : 1 + dp[i][x - c];
          if (take < best) best = take;
          msg += `; dùng ${c} → 1 + dp[${i}][${x - c}] = ${take >= INF ? "∞" : take}`;
        }
        dp[i][x] = best;
        msg += ` ⇒ chọn ${best >= INF ? "∞" : best}.`;
        steps.push({
          type: "dp-table", coins, X, dp: dp.map(r => [...r]), cur: [i, x], deps, traceCells: [],
          action: msg
        });
      }
    }

    // Truy vết
    const traceCells = [];
    const used = [];
    let ci = n, cx = X;
    if (dp[n][X] < INF) {
      while (cx > 0 && ci > 0) {
        traceCells.push([ci, cx]);
        if (cx >= coins[ci - 1] && dp[ci][cx] === 1 + dp[ci][cx - coins[ci - 1]]) {
          used.push(coins[ci - 1]);
          cx -= coins[ci - 1];
        } else {
          ci--;
        }
      }
      traceCells.push([ci, 0]);
      steps.push({
        type: "dp-table", coins, X, dp: dp.map(r => [...r]), cur: null, deps: [], traceCells,
        action: `Truy vết xong! Cần tối thiểu ${dp[n][X]} đồng: [${used.join(" + ")}] = ${X}.`
      });
    } else {
      steps.push({
        type: "dp-table", coins, X, dp: dp.map(r => [...r]), cur: null, deps: [], traceCells: [],
        action: `Không thể đổi được số tiền ${X} từ các mệnh giá đã cho.`
      });
    }
    return steps;
  }

  /* ==================== 4. LIS: O(n^2) vs O(n log n) ==================== */
  function buildLisSteps(arr) {
    const n = arr.length;
    const dp = Array(n).fill(1);
    const tail = [];
    const steps = [{
      type: "lis", arr, dp: [...dp], tail: [], i: -1, replacedIdx: -1,
      action: `Khởi tạo dp[i] = 1 cho mọi i và mảng tail[] rỗng.`
    }];

    for (let i = 0; i < n; i++) {
      // O(n^2)
      for (let j = 0; j < i; j++) {
        if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
          dp[i] = dp[j] + 1;
        }
      }
      // O(n log n) lower_bound
      let lo = 0, hi = tail.length;
      while (lo < hi) {
        const mid = (lo + hi) >> 1;
        if (tail[mid] < arr[i]) lo = mid + 1;
        else hi = mid;
      }
      let msg = `Xét A[${i}] = ${arr[i]}: dp[${i}] = ${dp[i]}. `;
      if (lo === tail.length) {
        tail.push(arr[i]);
        msg += `tail.push_back(${arr[i]}) (độ dài LIS tăng lên ${tail.length}).`;
      } else {
        const old = tail[lo];
        tail[lo] = arr[i];
        msg += `lower_bound tại vị trí ${lo}: thay ${old} → ${arr[i]} để tối ưu đuôi dãy độ dài ${lo + 1}.`;
      }
      steps.push({
        type: "lis", arr, dp: [...dp], tail: [...tail], i, replacedIdx: lo,
        action: msg
      });
    }
    return steps;
  }

  /* ==================== 5. TSP BITMASK DP (n=4) ==================== */
  function buildTspSteps() {
    const n = 4;
    const C = [
      [0, 10, 15, 20],
      [10, 0, 35, 25],
      [15, 35, 0, 30],
      [20, 25, 30, 0]
    ];
    const INF = 999;
    const FULL = (1 << n) - 1;
    const dp = Array.from({ length: n }, () => Array(1 << n).fill(INF));
    for (let i = 0; i < n; i++) dp[i][0] = C[i][0];

    const steps = [{
      type: "tsp-bitmask", n, C, dp: dp.map(r => [...r]), curI: -1, curS: -1, bestJ: -1,
      action: `Cơ sở: TSP(i, ∅) = C[i][0] — nếu không còn đỉnh nào phải thăm, quay thẳng về 0.`
    }];

    for (let S = 1; S < (1 << n); S++) {
      if (S & 1) continue; // S chỉ gồm tập con của {1,2,3}
      for (let i = 0; i < n; i++) {
        if (i !== 0 && ((S >> i) & 1)) continue;
        let best = INF, bestJ = -1;
        for (let j = 1; j < n; j++) {
          if ((S >> j) & 1) {
            const sub = S & ~(1 << j);
            const cand = C[i][j] + dp[j][sub];
            if (cand < best) {
              best = cand;
              bestJ = j;
            }
          }
        }
        dp[i][S] = best;
        const binS = S.toString(2).padStart(n, "0");
        const setList = [1, 2, 3].filter(x => (S >> x) & 1).join(",");
        steps.push({
          type: "tsp-bitmask", n, C, dp: dp.map(r => [...r]), curI: i, curS: S, bestJ,
          action: `TSP(i=${i}, S={${setList}} [${binS}]) = C[${i}][${bestJ}] + TSP(${bestJ}, S\\{${bestJ}}) = ${C[i][bestJ]} + ${dp[bestJ][S & ~(1 << bestJ)]} = ${best}.`
        });
      }
    }
    return steps;
  }

  /* ==================== RENDER STEP ==================== */
  function renderCurrentStep() {
    const stage = document.getElementById("viz-stage");
    const stepText = document.getElementById("viz-step-desc");
    const counter = document.getElementById("viz-step-counter");
    if (!stage || !vizState.steps.length) return;

    const st = vizState.steps[vizState.idx];
    counter.textContent = `Bước ${vizState.idx + 1} / ${vizState.steps.length}`;
    stepText.textContent = st.action || "";

    if (st.type === "prefix-window") {
      const cellsA = st.arr.map((v, idx) => {
        const inWin = idx >= st.L && idx <= st.R;
        const isBest = idx >= st.bestRange[0] && idx <= st.bestRange[1];
        const cls = inWin ? "viz-cell active" : isBest ? "viz-cell best" : "viz-cell";
        const ptr = (idx === st.L ? "L " : "") + (idx === st.R ? "R" : "");
        return `<div class="${cls}"><div class="viz-idx">${idx}</div><div class="viz-val">${v}</div><div class="viz-ptr">${ptr || "&nbsp;"}</div></div>`;
      }).join("");
      const cellsS = st.S.map((v, idx) => `<div class="viz-cell small"><div class="viz-idx">S[${idx}]</div><div class="viz-val">${v}</div></div>`).join("");
      stage.innerHTML = `
        <div class="viz-metrics">
          <span><strong>Q = ${st.Q}</strong></span>
          <span>Cửa sổ [${st.L}..${st.R}] tổng = <strong>${st.curSum}</strong></span>
          <span>Kỷ lục: <strong>${st.bestLen}</strong> ${st.bestRange[0] >= 0 ? `(đoạn [${st.bestRange[0]}..${st.bestRange[1]}])` : ""}</span>
        </div>
        <div class="viz-label">Mảng A (2 con trỏ L, R):</div>
        <div class="viz-row">${cellsA}</div>
        <div class="viz-label">Mảng cộng dồn S (S[r+1] - S[l] = tổng đoạn [l..r] trong O(1)):</div>
        <div class="viz-row">${cellsS}</div>
      `;
    } else if (st.type === "backtracking") {
      const byDepth = {};
      st.nodes.forEach(nd => {
        if (!byDepth[nd.depth]) byDepth[nd.depth] = [];
        byDepth[nd.depth].push(nd);
      });
      const levelsHtml = Object.keys(byDepth).sort((a, b) => a - b).map(d => {
        const items = byDepth[d].map(nd => {
          const isCur = nd.id === st.activeId ? "current" : "";
          return `<div class="viz-tree-node st-${nd.status} ${isCur}" title="${escHtml(nd.reason || "")}">
            <div>${escHtml(nd.label)}</div>
            ${nd.reason ? `<div class="viz-node-reason">${escHtml(nd.reason)}</div>` : ""}
          </div>`;
        }).join("");
        return `<div class="viz-tree-level"><span class="viz-depth-tag">Mức ${d}</span>${items}</div>`;
      }).join("");

      const info = st.mode === "cbus"
        ? `<span><strong>CBUS (n=2, K=1, C_min=${st.cmin})</strong></span><span>Kỷ lục f* = <strong>${st.fBest >= 999 ? "∞" : st.fBest}</strong></span><span>Lộ trình tốt nhất: <strong>${st.bestPath.length ? st.bestPath.join(" → ") : "Chưa có"}</strong></span>`
        : `<span><strong>Subset Sum = ${st.M}</strong> (A = [${st.a.join(", ")}])</span><span>Số nghiệm tìm thấy: <strong>${st.solutions}</strong></span>`;
      stage.innerHTML = `
        <div class="viz-metrics">${info}</div>
        <div class="viz-legend">
          <span class="leg-item"><i class="dot st-active"></i> Đang xét</span>
          <span class="leg-item"><i class="dot st-pruned"></i> Bị cắt nhánh (Pruned)</span>
          <span class="leg-item"><i class="dot st-solution"></i> Nghiệm / Kỷ lục</span>
          <span class="leg-item"><i class="dot st-done"></i> Đã quay lui</span>
        </div>
        <div class="viz-tree-wrap">${levelsHtml}</div>
      `;
    } else if (st.type === "dp-table") {
      const hdr = `<th>i \\ x</th>` + Array.from({ length: st.X + 1 }, (_, x) => `<th>${x}</th>`).join("");
      const rows = st.dp.map((row, i) => {
        const lbl = i === 0 ? "∅" : `D[${i}]=${st.coins[i - 1]}`;
        const tds = row.map((val, x) => {
          let cls = "";
          if (st.cur && st.cur[0] === i && st.cur[1] === x) cls = "dp-cur";
          else if (st.deps.some(p => p[0] === i && p[1] === x)) cls = "dp-dep";
          else if (st.traceCells.some(p => p[0] === i && p[1] === x)) cls = "dp-trace";
          return `<td class="${cls}">${val >= 999 ? "∞" : val}</td>`;
        }).join("");
        return `<tr><th>${lbl}</th>${tds}</tr>`;
      }).join("");
      stage.innerHTML = `
        <div class="viz-metrics">
          <span>Mệnh giá: <strong>[${st.coins.join(", ")}]</strong></span>
          <span>Số tiền cần đổi X = <strong>${st.X}</strong></span>
          <span>Công thức: <code>dp[i][x] = min(dp[i-1][x], 1 + dp[i][x - D[i]])</code></span>
        </div>
        <div class="viz-table-wrap"><table class="viz-dp-table"><thead><tr>${hdr}</tr></thead><tbody>${rows}</tbody></table></div>
      `;
    } else if (st.type === "lis") {
      const cellsA = st.arr.map((v, idx) => {
        const cls = idx === st.i ? "viz-cell active" : "viz-cell";
        return `<div class="${cls}"><div class="viz-idx">A[${idx}]</div><div class="viz-val">${v}</div><div class="viz-ptr">dp=${st.dp[idx]}</div></div>`;
      }).join("");
      const cellsTail = st.tail.length
        ? st.tail.map((v, idx) => `<div class="viz-cell ${idx === st.replacedIdx ? "best" : ""}"><div class="viz-idx">len=${idx + 1}</div><div class="viz-val">${v}</div></div>`).join("")
        : `<span class="muted small">(rỗng)</span>`;
      stage.innerHTML = `
        <div class="viz-metrics">
          <span>Độ dài LIS hiện tại: <strong>${st.tail.length}</strong></span>
          <span>Cách 1: QHĐ <code>dp[i]</code> O(n²) · Cách 2: mảng <code>tail[]</code> + <code>lower_bound</code> O(n log n)</span>
        </div>
        <div class="viz-label">Mảng A &amp; giá trị dp[i] (O(n²)):</div>
        <div class="viz-row">${cellsA}</div>
        <div class="viz-label">Mảng tail[] (O(n log n) — phần tử kết thúc nhỏ nhất cho mỗi độ dài):</div>
        <div class="viz-row">${cellsTail}</div>
      `;
    } else if (st.type === "tsp-bitmask") {
      const subsets = [0, 2, 4, 6, 8, 10, 12, 14];
      const hdr = `<th>i \\ S</th>` + subsets.map(s => `<th>${s.toString(2).padStart(4, "0")}</th>`).join("");
      const rows = [0, 1, 2, 3].map(i => {
        const tds = subsets.map(s => {
          const isCur = st.curI === i && st.curS === s;
          const v = st.dp[i][s];
          return `<td class="${isCur ? "dp-cur" : ""}">${v >= 999 ? "∞" : v}</td>`;
        }).join("");
        return `<tr><th>Đỉnh ${i}</th>${tds}</tr>`;
      }).join("");
      stage.innerHTML = `
        <div class="viz-metrics">
          <span><strong>TSP Bitmask (n=4)</strong>: bit thứ j = 1 nghĩa là đỉnh j vẫn còn trong tập S cần thăm</span>
          <span>Đáp án tối ưu: <code>TSP(0, 1110₂) = ${st.dp[0][14] >= 999 ? "?" : st.dp[0][14]}</code></span>
        </div>
        <div class="viz-table-wrap"><table class="viz-dp-table"><thead><tr>${hdr}</tr></thead><tbody>${rows}</tbody></table></div>
      `;
    }
  }

  function rebuildSteps() {
    stopAuto();
    const tab = vizState.tab;
    if (tab === "prefix-window") {
      const rawA = (document.getElementById("viz-arr-input")?.value || "2 1 4 3 1 1 5").trim();
      const arr = rawA.split(/[\s,]+/).map(Number).filter(x => !isNaN(x) && x >= 0).slice(0, 14);
      const Q = Math.max(1, Number(document.getElementById("viz-q-input")?.value || 6));
      vizState.steps = buildPrefixWindowSteps(arr.length ? arr : [2, 1, 4, 3, 1, 1, 5], Q);
    } else if (tab === "backtracking") {
      const mode = document.getElementById("viz-bt-mode")?.value || "cbus";
      vizState.steps = buildBacktrackingSteps(mode);
    } else if (tab === "dp-table") {
      const rawC = (document.getElementById("viz-coins-input")?.value || "1 3 4").trim();
      const coins = rawC.split(/[\s,]+/).map(Number).filter(x => !isNaN(x) && x > 0).slice(0, 5);
      const X = Math.min(15, Math.max(1, Number(document.getElementById("viz-x-input")?.value || 6)));
      vizState.steps = buildDpTableSteps(coins.length ? coins : [1, 3, 4], X);
    } else if (tab === "lis") {
      const rawA = (document.getElementById("viz-lis-input")?.value || "4 2 6 3 8 4 5 9").trim();
      const arr = rawA.split(/[\s,]+/).map(Number).filter(x => !isNaN(x)).slice(0, 14);
      vizState.steps = buildLisSteps(arr.length ? arr : [4, 2, 6, 3, 8, 4, 5, 9]);
    } else if (tab === "tsp-bitmask") {
      vizState.steps = buildTspSteps();
    }
    vizState.idx = 0;
    renderCurrentStep();
  }

  function renderControls() {
    const box = document.getElementById("viz-params");
    if (!box) return;
    if (vizState.tab === "prefix-window") {
      box.innerHTML = `
        <label class="viz-param">Mảng không âm: <input id="viz-arr-input" class="input-sm" value="2 1 4 3 1 1 5" /></label>
        <label class="viz-param">Q: <input id="viz-q-input" type="number" class="input-sm" style="width:68px;" value="6" /></label>
        <button class="btn btn-sm btn-primary" id="viz-apply-btn">Áp dụng</button>
      `;
    } else if (vizState.tab === "backtracking") {
      box.innerHTML = `
        <label class="viz-param">Bài toán:
          <select id="viz-bt-mode" class="select-input">
            <option value="cbus">CBUS Nhánh &amp; Cận (n=2, K=1)</option>
            <option value="subset">Quay lui Subset Sum (A=[3,5,2,4], M=7)</option>
          </select>
        </label>
        <button class="btn btn-sm btn-primary" id="viz-apply-btn">Tạo cây</button>
      `;
    } else if (vizState.tab === "dp-table") {
      box.innerHTML = `
        <label class="viz-param">Mệnh giá: <input id="viz-coins-input" class="input-sm" style="width:120px;" value="1 3 4" /></label>
        <label class="viz-param">Tiền X (≤15): <input id="viz-x-input" type="number" class="input-sm" style="width:68px;" value="6" /></label>
        <button class="btn btn-sm btn-primary" id="viz-apply-btn">Chạy bảng</button>
      `;
    } else if (vizState.tab === "lis") {
      box.innerHTML = `
        <label class="viz-param">Dãy số A: <input id="viz-lis-input" class="input-sm" value="4 2 6 3 8 4 5 9" /></label>
        <button class="btn btn-sm btn-primary" id="viz-apply-btn">Áp dụng</button>
      `;
    } else {
      box.innerHTML = `<span class="muted small">Đồ thị mẫu 4 đỉnh (Cố định) — minh họa quá trình lấp bảng Bitmask DP.</span>`;
    }
    const applyBtn = document.getElementById("viz-apply-btn");
    if (applyBtn) applyBtn.onclick = rebuildSteps;
    const btSel = document.getElementById("viz-bt-mode");
    if (btSel) btSel.onchange = rebuildSteps;
  }

  window.renderVisualizerView = function (initialTab) {
    if (initialTab) vizState.tab = initialTab;
    const root = document.getElementById("view-visualizer");
    if (!root) return;

    if (!vizState.initialized) {
      vizState.initialized = true;
      root.querySelectorAll(".viz-topic-pill").forEach(btn => {
        btn.onclick = () => {
          vizState.tab = btn.dataset.viz;
          root.querySelectorAll(".viz-topic-pill").forEach(b => b.classList.toggle("active", b.dataset.viz === vizState.tab));
          renderControls();
          rebuildSteps();
        };
      });
      document.getElementById("viz-prev-btn").onclick = () => {
        stopAuto();
        if (vizState.idx > 0) { vizState.idx--; renderCurrentStep(); }
      };
      document.getElementById("viz-next-btn").onclick = () => {
        stopAuto();
        if (vizState.idx < vizState.steps.length - 1) { vizState.idx++; renderCurrentStep(); }
      };
      document.getElementById("viz-play-btn").onclick = toggleAuto;
      document.getElementById("viz-reset-btn").onclick = () => {
        stopAuto();
        vizState.idx = 0;
        renderCurrentStep();
      };
    }

    root.querySelectorAll(".viz-topic-pill").forEach(b => b.classList.toggle("active", b.dataset.viz === vizState.tab));
    renderControls();
    rebuildSteps();
  };
})();

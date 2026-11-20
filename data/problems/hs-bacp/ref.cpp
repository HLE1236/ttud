#include <bits/stdc++.h>
using namespace std;
int N, M, best = 1e9;
int c[20], A[20][20], load[10], per[20], order[20], min_k[20], max_k[20];
vector<int> prereq[20];

void dfs(int idx, int cur_max) {
    if (cur_max >= best) return;
    if (idx == N) { best = cur_max; return; }
    int u = order[idx];
    int lo = min_k[u];
    for (int p : prereq[u]) lo = max(lo, per[p] + 1);
    int hi = max_k[u];
    for (int k = lo; k <= hi; ++k) {
        if (load[k] + c[u] >= best) continue;
        per[u] = k;
        load[k] += c[u];
        dfs(idx + 1, max(cur_max, load[k]));
        load[k] -= c[u];
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> N >> M;
    for (int i = 0; i < N; ++i) cin >> c[i];
    vector<int> indeg(N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            cin >> A[i][j];
            if (A[i][j]) { prereq[j].push_back(i); indeg[j]++; }
        }
    // Topological order
    queue<int> q;
    for (int i = 0; i < N; ++i) { min_k[i] = 1; max_k[i] = M; if (indeg[i] == 0) q.push(i); }
    int pos = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order[pos++] = u;
        for (int v = 0; v < N; ++v) if (A[u][v]) {
            min_k[v] = max(min_k[v], min_k[u] + 1);
            if (--indeg[v] == 0) q.push(v);
        }
    }
    for (int idx = N - 1; idx >= 0; --idx) {
        int u = order[idx];
        for (int v = 0; v < N; ++v) if (A[u][v]) max_k[u] = min(max_k[u], max_k[v] - 1);
    }
    dfs(0, 0);
    cout << best << "\n";
    return 0;
}

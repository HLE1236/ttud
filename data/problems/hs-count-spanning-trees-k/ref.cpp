#include <bits/stdc++.h>
using namespace std;
struct Edge { int u, v, w; };
int n, m, K;
vector<Edge> E;
long long ans = 0;

void dfs(int idx, int taken, int cur_w, array<int, 21> p) {
    if (cur_w > K) return;
    if (taken == n - 1) { ans++; return; }
    int need = (n - 1) - taken;
    if (m - idx < need) return;
    // Lower bound using next 'need' edges (E is sorted by w ascending)
    int lb = cur_w;
    for (int i = 0; i < need; ++i) lb += E[idx + i].w;
    if (lb > K) return;

    // Option 1: pick edge idx if no cycle
    int u = E[idx].u, v = E[idx].v;
    while (p[u] != u) u = p[u];
    while (p[v] != v) v = p[v];
    if (u != v) {
        auto np = p;
        np[v] = u;
        dfs(idx + 1, taken + 1, cur_w + E[idx].w, np);
    }
    // Option 2: skip edge idx
    dfs(idx + 1, taken, cur_w, p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> K;
    E.resize(m);
    for (int i = 0; i < m; ++i) cin >> E[i].u >> E[i].v >> E[i].w;
    sort(E.begin(), E.end(), [](const Edge& a, const Edge& b){ return a.w < b.w; });
    array<int, 21> p;
    for (int i = 0; i <= 20; ++i) p[i] = i;
    dfs(0, 0, 0, p);
    cout << ans << "\n";
    return 0;
}

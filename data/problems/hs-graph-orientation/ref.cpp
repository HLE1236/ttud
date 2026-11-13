#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    vector<int> tin(n + 1, 0), low(n + 1, 0);
    vector<pair<int, int>> ans(m, {-1, -1});
    int timer = 0, visited = 0;
    bool has_bridge = false;

    auto dfs = [&](auto& self, int u, int peid) -> void {
        tin[u] = low[u] = ++timer;
        visited++;
        for (auto [v, eid] : adj[u]) {
            if (eid == peid) continue;
            if (!tin[v]) {
                ans[eid] = {u, v}; // tree edge goes down
                self(self, v, eid);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) has_bridge = true;
            } else if (tin[v] < tin[u]) {
                ans[eid] = {u, v}; // back edge goes up
                low[u] = min(low[u], tin[v]);
            }
        }
    };

    dfs(dfs, 1, -1);
    if (visited != n || has_bridge) {
        cout << -1 << "\n";
        return 0;
    }
    cout << m << "\n";
    for (auto [u, v] : ans) cout << u << " " << v << "\n";
    return 0;
}

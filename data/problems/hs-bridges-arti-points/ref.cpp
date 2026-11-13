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
    vector<char> arti(n + 1, 0);
    int timer = 0, bridges = 0;

    auto dfs = [&](auto& self, int u, int peid) -> void {
        tin[u] = low[u] = ++timer;
        int child = 0;
        for (auto [v, eid] : adj[u]) {
            if (eid == peid) continue;
            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                child++;
                self(self, v, eid);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges++;
                if (peid != -1 && low[v] >= tin[u]) arti[u] = 1;
            }
        }
        if (peid == -1 && child > 1) arti[u] = 1;
    };

    for (int i = 1; i <= n; ++i)
        if (!tin[i]) dfs(dfs, i, -1);
    int num_arti = accumulate(arti.begin(), arti.end(), 0);
    cout << num_arti << " " << bridges << "\n";
    return 0;
}

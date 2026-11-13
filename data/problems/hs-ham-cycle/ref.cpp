#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m; cin >> n >> m;
        vector<vector<char>> g(n + 1, vector<char>(n + 1, 0));
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < m; ++i) {
            int u, v; cin >> u >> v;
            if (u != v && !g[u][v]) {
                g[u][v] = g[v][u] = 1;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
        if (n == 1) { cout << 1 << "\n"; continue; }
        if (n == 2) { cout << (m >= 1 ? 0 : 0) << "\n"; continue; } // simple graph n=2 has no simple cycle of length >= 3
        bool bad = false;
        for (int i = 1; i <= n; ++i) if ((int)adj[i].size() < 2) bad = true;
        if (bad) { cout << 0 << "\n"; continue; }

        vector<char> vis(n + 1, 0);
        vis[1] = 1;
        bool found = false;
        auto dfs = [&](auto& self, int u, int cnt) -> void {
            if (found) return;
            if (cnt == n) {
                if (g[u][1]) found = true;
                return;
            }
            for (int v : adj[u]) {
                if (!vis[v]) {
                    vis[v] = 1;
                    self(self, v, cnt + 1);
                    vis[v] = 0;
                    if (found) return;
                }
            }
        };
        dfs(dfs, 1, 1);
        cout << (found ? 1 : 0) << "\n";
    }
    return 0;
}

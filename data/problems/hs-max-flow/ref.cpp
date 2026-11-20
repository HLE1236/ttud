#include <bits/stdc++.h>
using namespace std;
struct Edge { int to, rev; long long cap; };
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    int s, t; cin >> s >> t;
    vector<vector<Edge>> adj(n + 1);
    auto add_edge = [&](int u, int v, long long c) {
        adj[u].push_back({v, (int)adj[v].size(), c});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0});
    };
    for (int i = 0; i < m; ++i) {
        int u, v; long long c; cin >> u >> v >> c;
        add_edge(u, v, c);
    }
    vector<int> level(n + 1), ptr(n + 1);
    auto bfs = [&]() -> bool {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto& e : adj[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    };
    auto dfs = [&](auto& self, int u, long long pushed) -> long long {
        if (pushed == 0 || u == t) return pushed;
        for (int& cid = ptr[u]; cid < (int)adj[u].size(); ++cid) {
            auto& e = adj[u][cid];
            if (level[e.to] != level[u] + 1 || e.cap <= 0) continue;
            long long tr = self(self, e.to, min(pushed, e.cap));
            if (tr == 0) continue;
            e.cap -= tr;
            adj[e.to][e.rev].cap += tr;
            return tr;
        }
        return 0;
    };
    long long flow = 0;
    const long long INF = 4e18;
    while (bfs()) {
        fill(ptr.begin(), ptr.end(), 0);
        while (long long pushed = dfs(dfs, s, INF)) flow += pushed;
    }
    cout << flow << "\n";
    return 0;
}

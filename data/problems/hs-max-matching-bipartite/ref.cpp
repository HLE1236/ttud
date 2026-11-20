#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 1; i <= n; ++i) {
        int k; cin >> k;
        adj[i].resize(k);
        for (int j = 0; j < k; ++j) cin >> adj[i][j];
    }
    const int INF = 1e9;
    vector<int> pairU(n + 1, 0), pairV(m + 1, 0), dist(n + 1, 0);

    auto bfs = [&]() -> bool {
        queue<int> q;
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0) { dist[u] = 0; q.push(u); }
            else dist[u] = INF;
        }
        dist[0] = INF;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] < dist[0]) {
                for (int v : adj[u]) {
                    int nu = pairV[v];
                    if (dist[nu] == INF) {
                        dist[nu] = dist[u] + 1;
                        q.push(nu);
                    }
                }
            }
        }
        return dist[0] != INF;
    };

    auto dfs = [&](auto& self, int u) -> bool {
        if (u != 0) {
            for (int v : adj[u]) {
                int nu = pairV[v];
                if (dist[nu] == dist[u] + 1 && self(self, nu)) {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
            dist[u] = INF;
            return false;
        }
        return true;
    };

    int matching = 0;
    while (bfs()) {
        for (int u = 1; u <= n; ++u)
            if (pairU[u] == 0 && dfs(dfs, u)) matching++;
    }
    cout << matching << "\n";
    return 0;
}

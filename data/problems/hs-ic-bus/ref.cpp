#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> C(n + 1), D(n + 1);
    for (int i = 1; i <= n; ++i) cin >> C[i] >> D[i];
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    const long long INF = 4e18;
    vector<long long> cost(n + 1, INF);
    vector<int> bfs_dist(n + 1), bfs_vis(n + 1, 0);
    int stamp = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    cost[1] = 0;
    pq.push({0, 1});
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du > cost[u]) continue;
        if (u == n) break;
        ++stamp;
        queue<int> q;
        bfs_vis[u] = stamp;
        bfs_dist[u] = 0;
        q.push(u);
        while (!q.empty()) {
            int x = q.front(); q.pop();
            if (bfs_dist[x] == D[u]) continue;
            for (int y : adj[x]) {
                if (bfs_vis[y] != stamp) {
                    bfs_vis[y] = stamp;
                    bfs_dist[y] = bfs_dist[x] + 1;
                    q.push(y);
                    if (du + C[u] < cost[y]) {
                        cost[y] = du + C[u];
                        pq.push({cost[y], y});
                    }
                }
            }
        }
    }
    cout << cost[n] << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, rev, cap;
    long long cost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, K;
    cin >> n >> m >> K;
    int S = 0, T = n + m + 1, V = T + 1;
    vector<vector<Edge>> g(V);
    auto addEdge = [&](int u, int v, int cap, long long cost) {
        g[u].push_back({v, (int)g[v].size(), cap, cost});
        g[v].push_back({u, (int)g[u].size() - 1, 0, -cost});
    };
    for (int i = 1; i <= n; i++) addEdge(S, i, 1, 0);
    for (int j = 1; j <= m; j++) addEdge(n + j, T, 1, 0);
    for (int i = 0; i < K; i++) {
        int u, v; long long w;
        cin >> u >> v >> w;
        if (w > 0) addEdge(u, n + v, 1, -w);
    }

    const long long INF = 4e18;
    long long minCost = 0;
    vector<long long> dist(V);
    vector<int> pv(V), pe(V);
    vector<bool> inq(V);

    while (true) {
        fill(dist.begin(), dist.end(), INF);
        fill(inq.begin(), inq.end(), false);
        queue<int> q;
        dist[S] = 0;
        q.push(S);
        inq[S] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = false;
            for (int i = 0; i < (int)g[u].size(); i++) {
                const Edge& e = g[u][i];
                if (e.cap > 0 && dist[u] + e.cost < dist[e.to]) {
                    dist[e.to] = dist[u] + e.cost;
                    pv[e.to] = u;
                    pe[e.to] = i;
                    if (!inq[e.to]) {
                        inq[e.to] = true;
                        q.push(e.to);
                    }
                }
            }
        }
        if (dist[T] >= 0) break;
        minCost += dist[T];
        for (int v = T; v != S; v = pv[v]) {
            Edge& e = g[pv[v]][pe[v]];
            e.cap -= 1;
            g[v][e.rev].cap += 1;
        }
    }
    cout << -minCost << '\n';
    return 0;
}

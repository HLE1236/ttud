#include <bits/stdc++.h>
using namespace std;
static const long long INF = (long long)4e18;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<vector<pair<int, long long>>> adj(N);
    for (int i = 0; i < M; ++i) {
        int u, v, type;
        long long d, t;
        cin >> u >> v >> d >> t >> type;
        long long w = d * t;
        adj[u].push_back({v, w});
        if (type == 2) adj[v].push_back({u, w});
    }
    int S, T; cin >> S >> T;
    vector<long long> dist(N, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[S] = 0;
    pq.push({0, S});
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du != dist[u]) continue;
        if (u == T) break;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    cout << (dist[T] >= INF / 2 ? -1 : dist[T]) << "\n";
    return 0;
}

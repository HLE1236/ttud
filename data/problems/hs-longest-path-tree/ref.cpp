#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v, w; cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    auto farthest = [&](int s) -> pair<int, long long> {
        vector<long long> dist(n + 1, -1);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        int best_u = s;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] > dist[best_u]) best_u = u;
            for (auto [v, w] : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + w;
                    q.push(v);
                }
            }
        }
        return {best_u, dist[best_u]};
    };
    int a = farthest(1).first;
    long long ans = farthest(a).second;
    cout << ans << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<vector<pair<int, long long>>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v; long long w; cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    vector<int> par(n + 1, 0), ord;
    ord.reserve(n);
    queue<int> q;
    q.push(1); par[1] = -1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ord.push_back(u);
        for (auto [v, w] : adj[u])
            if (v != par[u]) { par[v] = u; q.push(v); }
    }
    vector<long long> sz(n + 1, 1), down(n + 1, 0), f(n + 1, 0);
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = ord[idx];
        for (auto [v, w] : adj[u]) {
            if (v == par[u]) continue;
            sz[u] += sz[v];
            down[u] += down[v] + w * sz[v];
        }
    }
    f[1] = down[1];
    long long ans = f[1];
    for (int idx = 0; idx < n; ++idx) {
        int u = ord[idx];
        for (auto [v, w] : adj[u]) {
            if (v == par[u]) continue;
            f[v] = f[u] + w * (n - 2LL * sz[v]);
            ans = max(ans, f[v]);
        }
    }
    cout << ans << "\n";
    return 0;
}

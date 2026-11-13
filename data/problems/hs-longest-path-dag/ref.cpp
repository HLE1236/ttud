#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w; cin >> u >> v >> w;
        if (u >= 1 && u <= n && v >= 1 && v <= n)
            adj[u].push_back({v, w});
    }
    const long long NEG = -4e18;
    vector<long long> dp(n + 1, NEG);
    dp[1] = 0;
    for (int u = 1; u <= n; ++u) {
        if (dp[u] < NEG / 2) continue;
        for (auto [v, w] : adj[u]) {
            dp[v] = max(dp[v], dp[u] + w);
        }
    }
    cout << (dp[n] < NEG / 2 ? -1 : dp[n]) << "\n";
    return 0;
}

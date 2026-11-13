#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, K;
    if (!(cin >> n >> K)) return 0;
    vector<vector<pair<int, long long>>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v; long long w; cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    const long long NEG = -4e18;
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
    vector<vector<long long>> dp(n + 1, vector<long long>(K + 1, NEG));
    for (int i = 1; i <= n; ++i) dp[i][0] = 0;
    long long ans = NEG;
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = ord[idx];
        for (auto [v, w] : adj[u]) {
            if (v == par[u]) continue;
            for (int d1 = 0; d1 <= K - 1; ++d1) {
                int d2 = K - 1 - d1;
                if (dp[u][d1] > NEG / 2 && dp[v][d2] > NEG / 2) {
                    ans = max(ans, dp[u][d1] + w + dp[v][d2]);
                }
            }
            for (int d = 0; d < K; ++d) {
                if (dp[v][d] > NEG / 2) {
                    dp[u][d + 1] = max(dp[u][d + 1], dp[v][d] + w);
                }
            }
        }
    }
    cout << (ans < NEG / 2 ? -1 : ans) << "\n";
    return 0;
}

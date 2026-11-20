#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;
int n, K;
vector<vector<pair<int, long long>>> adj;
vector<vector<long long>> dp;
long long ans = -INF;

void dfs(int u, int p) {
    dp[u][0] = 0;
    for (auto [v, w] : adj[u]) {
        if (v == p) continue;
        dfs(v, u);
        // Ghep nhanh da co cua u (do dai i) voi nhanh tu con v (do dai K - 1 - i)
        for (int i = 0; i < K; i++) {
            ans = max(ans, dp[u][i] + w + dp[v][K - 1 - i]);
        }
        // Cap nhat nhanh di sau xuong cay con u
        for (int i = 0; i < K; i++) {
            dp[u][i + 1] = max(dp[u][i + 1], dp[v][i] + w);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> K;
    adj.resize(n + 1);
    dp.assign(n + 1, vector<long long>(K + 1, -INF));

    for (int i = 1; i < n; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    dfs(1, 0);
    cout << (ans < -INF / 2 ? -1 : ans) << '\n';
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
// Exact Steiner Tree via Dreyfus-Wagner DP: dp[mask][v] for |R| <= 12, N <= 30
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    cin >> N >> M;
    const long long INF = 1e15;
    vector<vector<long long>> d(N + 1, vector<long long>(N + 1, INF));
    for (int i = 1; i <= N; i++) d[i][i] = 0;
    for (int i = 0; i < M; i++) {
        int u, v; long long w;
        cin >> u >> v >> w;
        d[u][v] = min(d[u][v], w);
        d[v][u] = min(d[v][u], w);
    }
    for (int k = 1; k <= N; k++)
        for (int i = 1; i <= N; i++)
            for (int j = 1; j <= N; j++)
                if (d[i][k] + d[k][j] < d[i][j]) d[i][j] = d[i][k] + d[k][j];
    int K;
    cin >> K;
    vector<int> R(K);
    for (int i = 0; i < K; i++) cin >> R[i];
    if (K <= 1) { cout << 0 << "\n"; return 0; }
    int FULL = 1 << K;
    vector<vector<long long>> dp(FULL, vector<long long>(N + 1, INF));
    for (int i = 0; i < K; i++) {
        for (int v = 1; v <= N; v++) dp[1 << i][v] = d[R[i]][v];
    }
    for (int mask = 1; mask < FULL; mask++) {
        if ((mask & (mask - 1)) == 0) continue;
        for (int v = 1; v <= N; v++) {
            for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
                dp[mask][v] = min(dp[mask][v], dp[sub][v] + dp[mask ^ sub][v]);
            }
        }
        for (int v = 1; v <= N; v++) {
            for (int u = 1; u <= N; u++) {
                dp[mask][v] = min(dp[mask][v], dp[mask][u] + d[u][v]);
            }
        }
    }
    long long ans = INF;
    for (int v = 1; v <= N; v++) ans = min(ans, dp[FULL - 1][v]);
    cout << ans << "\n";
    return 0;
}

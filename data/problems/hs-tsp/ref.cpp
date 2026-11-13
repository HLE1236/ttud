#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<vector<long long>> c(n, vector<long long>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) cin >> c[i][j];
    if (n == 1) { cout << 0 << "\n"; return 0; }
    int M = n - 1, FULL = 1 << M;
    const long long INF = 4e18;
    vector<long long> dp((size_t)FULL * M, INF);
    for (int i = 0; i < M; ++i) dp[(size_t)(1 << i) * M + i] = c[0][i + 1];
    for (int mask = 1; mask < FULL; ++mask) {
        for (int u = 0; u < M; ++u) {
            if (!((mask >> u) & 1)) continue;
            long long cur = dp[(size_t)mask * M + u];
            if (cur >= INF) continue;
            for (int v = 0; v < M; ++v) if (!((mask >> v) & 1)) {
                size_t ni = (size_t)(mask | (1 << v)) * M + v;
                dp[ni] = min(dp[ni], cur + c[u + 1][v + 1]);
            }
        }
    }
    long long ans = INF;
    for (int u = 0; u < M; ++u)
        ans = min(ans, dp[(size_t)(FULL - 1) * M + u] + c[u + 1][0]);
    cout << ans << "\n";
    return 0;
}

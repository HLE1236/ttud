#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, K, Q; if (!(cin >> n >> K >> Q)) return 0;
    vector<int> d(n + 1, 0);
    for (int i = 1; i <= n; ++i) cin >> d[i];
    vector<vector<int>> c(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= n; ++j) cin >> c[i][j];

    int FULL = 1 << n;
    const int INF = 1e9;
    vector<vector<int>> tsp(FULL, vector<int>(n, INF));
    for (int i = 0; i < n; ++i) tsp[1 << i][i] = c[0][i + 1];
    for (int mask = 1; mask < FULL; ++mask) {
        int demand = 0;
        for (int i = 0; i < n; ++i) if ((mask >> i) & 1) demand += d[i + 1];
        if (demand > Q) continue;
        for (int u = 0; u < n; ++u) {
            if (tsp[mask][u] >= INF) continue;
            for (int v = 0; v < n; ++v) if (!((mask >> v) & 1)) {
                tsp[mask | (1 << v)][v] = min(tsp[mask | (1 << v)][v], tsp[mask][u] + c[u + 1][v + 1]);
            }
        }
    }
    vector<int> routeCost(FULL, INF);
    routeCost[0] = 0;
    for (int mask = 1; mask < FULL; ++mask) {
        int demand = 0;
        for (int i = 0; i < n; ++i) if ((mask >> i) & 1) demand += d[i + 1];
        if (demand > Q) continue;
        for (int u = 0; u < n; ++u)
            if (tsp[mask][u] < INF) routeCost[mask] = min(routeCost[mask], tsp[mask][u] + c[u + 1][0]);
    }
    vector<int> dp = routeCost;
    for (int k = 2; k <= K; ++k) {
        vector<int> ndp = dp;
        for (int mask = 1; mask < FULL; ++mask) {
            for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
                if (routeCost[sub] < INF && dp[mask ^ sub] < INF) {
                    ndp[mask] = min(ndp[mask], dp[mask ^ sub] + routeCost[sub]);
                }
            }
        }
        dp.swap(ndp);
    }
    cout << dp[FULL - 1] << "\n";
    return 0;
}

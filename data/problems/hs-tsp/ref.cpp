#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<long long>> c(n, vector<long long>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> c[i][j];
        }
    }
    if (n == 1) {
        cout << 0 << '\n';
        return 0;
    }

    // Xuat phat tu dinh 0, mask luu trang thai cua M = n - 1 dinh con lai (1 .. n-1)
    int m = n - 1;
    int full = 1 << m;
    vector<vector<long long>> dp(full, vector<long long>(m, INF));

    for (int i = 0; i < m; i++) {
        dp[1 << i][i] = c[0][i + 1];
    }

    for (int mask = 1; mask < full; mask++) {
        for (int u = 0; u < m; u++) {
            if (dp[mask][u] == INF) continue;
            for (int v = 0; v < m; v++) {
                if ((mask >> v) & 1) continue;
                int next_mask = mask | (1 << v);
                dp[next_mask][v] = min(dp[next_mask][v], dp[mask][u] + c[u + 1][v + 1]);
            }
        }
    }

    long long ans = INF;
    for (int u = 0; u < m; u++) {
        ans = min(ans, dp[full - 1][u] + c[u + 1][0]);
    }
    cout << ans << '\n';
    return 0;
}

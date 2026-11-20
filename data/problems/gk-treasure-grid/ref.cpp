#include <bits/stdc++.h>
using namespace std;
static const long long INF = (long long)4e18;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<long long>> a(n + 1, vector<long long>(m + 1));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            cin >> a[i][j];
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, INF));
    if (a[1][1] != 0) dp[1][1] = a[1][1];
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (i == 1 && j == 1) continue;
            if (a[i][j] == 0) continue;
            long long prev = min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            if (prev < INF / 2) dp[i][j] = prev + a[i][j];
        }
    }
    if (dp[n][m] >= INF / 2) cout << -1 << "\n";
    else cout << dp[n][m] << "\n";
    return 0;
}

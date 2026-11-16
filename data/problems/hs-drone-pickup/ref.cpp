#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K;
    if (!(cin >> N >> K)) return 0;
    vector<int> c(N + 1), a(N + 1);
    for (int i = 1; i <= N; i++) cin >> c[i];
    for (int i = 1; i <= N; i++) cin >> a[i];

    vector<vector<int>> dp(N + 1, vector<int>(K + 1, -1));
    dp[1][0] = c[1];
    for (int k = 0; k < K; k++) {
        for (int i = 1; i < N; i++) {
            if (dp[i][k] < 0) continue;
            int lim = min(N, i + a[i]);
            for (int j = i + 1; j <= lim; j++) {
                dp[j][k + 1] = max(dp[j][k + 1], dp[i][k] + c[j]);
            }
        }
    }
    int ans = -1;
    for (int k = 0; k <= K; k++) ans = max(ans, dp[N][k]);
    cout << ans << '\n';
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, T, D;
    cin >> N >> T >> D;
    vector<int> a(N + 1), t(N + 1);
    for (int i = 1; i <= N; i++) cin >> a[i];
    for (int i = 1; i <= N; i++) cin >> t[i];

    vector<vector<int>> dp(N + 1, vector<int>(T + 1, -1));
    int ans = 0;
    for (int i = 1; i <= N; i++) {
        if (t[i] <= T) dp[i][t[i]] = a[i];
        for (int j = max(1, i - D); j < i; j++) {
            for (int tm = t[i]; tm <= T; tm++) {
                if (dp[j][tm - t[i]] >= 0) {
                    dp[i][tm] = max(dp[i][tm], dp[j][tm - t[i]] + a[i]);
                }
            }
        }
        for (int tm = 0; tm <= T; tm++) ans = max(ans, dp[i][tm]);
    }
    cout << ans << '\n';
    return 0;
}

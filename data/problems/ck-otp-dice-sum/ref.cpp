#include <bits/stdc++.h>
using namespace std;
static const long long MOD = 1000000007LL;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n, x;
    cin >> m >> n >> x;
    if (x < n || x > n * m) {
        cout << 0 << "\n";
        return 0;
    }
    vector<long long> dp(x + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; ++i) {
        vector<long long> ndp(x + 1, 0);
        long long win = 0;
        for (int s = 1; s <= x; ++s) {
            win = (win + dp[s - 1]) % MOD;
            if (s - m - 1 >= 0) win = (win - dp[s - m - 1] + MOD) % MOD;
            ndp[s] = win;
        }
        dp.swap(ndp);
    }
    cout << dp[x] << "\n";
    return 0;
}

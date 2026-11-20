#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<long long> dp(max(2, n + 1), 0);
    dp[0] = 1;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = (dp[i - 1] + 2 * dp[i - 2]) % MOD;
    }
    cout << dp[n] << '\n';
    return 0;
}

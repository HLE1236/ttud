#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<long long> p(n + 1);
    for (int i = 1; i <= n; ++i) cin >> p[i];
    vector<long long> dp(n + 1, 0);
    for (int len = 1; len <= n; ++len) {
        for (int cut = 1; cut <= len; ++cut) {
            dp[len] = max(dp[len], p[cut] + dp[len - cut]);
        }
    }
    cout << dp[n] << "\n";
    return 0;
}

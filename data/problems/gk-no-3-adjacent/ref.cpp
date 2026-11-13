#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<long long> a(n + 1), dp(n + 1, 0);
    for (int i = 1; i <= n; i++) cin >> a[i];
    if (n >= 1) dp[1] = a[1];
    if (n >= 2) dp[2] = a[1] + a[2];
    for (int i = 3; i <= n; i++) {
        dp[i] = max({dp[i - 1], dp[i - 2] + a[i], dp[i - 3] + a[i - 1] + a[i]});
    }
    cout << dp[n] << "\n";
    return 0;
}

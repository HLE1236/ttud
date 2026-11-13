#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, b;
    cin >> n >> b;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    // DP map/array: offset for sum up to 50000 (since n<=50, |ai|<=1000 -> |sum|<=50000)
    const int OFF = 50000;
    vector<long long> dp(2 * OFF + 1, 0), ndp(2 * OFF + 1, 0);
    dp[OFF + a[0]] = 1;
    for (int i = 1; i < n; i++) {
        fill(ndp.begin(), ndp.end(), 0);
        for (int s = -OFF; s <= OFF; s++) {
            long long ways = dp[OFF + s];
            if (!ways) continue;
            if (abs(s + a[i]) <= OFF) ndp[OFF + s + a[i]] += ways;
            if (abs(s - a[i]) <= OFF) ndp[OFF + s - a[i]] += ways;
        }
        dp.swap(ndp);
    }
    if (abs(b) > OFF) cout << 0 << "\n";
    else cout << dp[OFF + b] << "\n";
    return 0;
}

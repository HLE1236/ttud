#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<long long> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i];
    if (k <= 0 || n <= 1) {
        cout << 0 << "\n";
        return 0;
    }
    if (2 * k >= n) {
        long long ans = 0;
        for (int i = 1; i < n; ++i)
            if (p[i] > p[i - 1]) ans += p[i] - p[i - 1];
        cout << ans << "\n";
        return 0;
    }
    vector<long long> cash(k + 1, 0), hold(k + 1, -(long long)4e18);
    for (long long price : p) {
        for (int t = 1; t <= k; ++t) {
            hold[t] = max(hold[t], cash[t - 1] - price);
            cash[t] = max(cash[t], hold[t] + price);
        }
    }
    cout << cash[k] << "\n";
    return 0;
}

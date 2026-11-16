#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    vector<int> dp(n + 1, 1), ndp(n + 1, 0), bit(n + 1, 0);

    for (int len = 2; len <= k; len++) {
        fill(bit.begin(), bit.end(), 0);
        int total = 0;
        for (int i = 1; i <= n; i++) {
            int lessEq = 0;
            for (int x = a[i]; x > 0; x -= x & -x) {
                lessEq += bit[x];
                if (lessEq >= MOD) lessEq -= MOD;
            }
            ndp[i] = total - lessEq;
            if (ndp[i] < 0) ndp[i] += MOD;

            int v = dp[i];
            total += v;
            if (total >= MOD) total -= MOD;
            for (int x = a[i]; x <= n; x += x & -x) {
                bit[x] += v;
                if (bit[x] >= MOD) bit[x] -= MOD;
            }
        }
        dp.swap(ndp);
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += dp[i];
        if (ans >= MOD) ans -= MOD;
    }
    cout << ans << '\n';
    return 0;
}

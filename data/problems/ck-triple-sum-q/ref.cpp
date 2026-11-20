#include <bits/stdc++.h>
using namespace std;
static const long long MOD = 1000000007LL;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; long long Q;
    cin >> n >> Q;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    long long ans = 0;
    for (int i = 0; i < n - 2; ++i) {
        if (a[i] >= Q) break;
        long long target = Q - a[i];
        int l = i + 1, r = n - 1;
        while (l < r) {
            long long s = a[l] + a[r];
            if (s == target) {
                ans = (ans + 1) % MOD;
                l++; r--;
            } else if (s < target) {
                l++;
            } else {
                r--;
            }
        }
    }
    cout << ans << "\n";
    return 0;
}

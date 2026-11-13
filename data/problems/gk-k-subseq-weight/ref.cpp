#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    long long m;
    cin >> n >> k >> m;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    long long win = 0, ans = 0;
    for (int i = 0; i < k; i++) win += a[i];
    if (win == m) ans++;
    for (int i = k; i < n; i++) {
        win += a[i] - a[i - k];
        if (win == m) ans++;
    }
    cout << ans << "\n";
    return 0;
}

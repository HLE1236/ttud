#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    long long win = 0;
    for (int i = 0; i < k; i++) win += a[i];
    int ans = (win % 2 == 0) ? 1 : 0;
    for (int i = k; i < n; i++) {
        win += a[i] - a[i - k];
        if (win % 2 == 0) ans++;
    }
    cout << ans << '\n';
    return 0;
}

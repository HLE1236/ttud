#include <bits/stdc++.h>
using namespace std;
int n, k, m, a[25], ans = 0;
void Try(int idx, int cnt, int sum) {
    if (cnt == k) {
        if (sum == m) ans++;
        return;
    }
    if (idx == n || cnt + (n - idx) < k) return;
    Try(idx + 1, cnt, sum);
    if (sum + a[idx] <= m) Try(idx + 1, cnt + 1, sum + a[idx]);
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k >> m;
    for (int i = 0; i < n; i++) cin >> a[i];
    Try(0, 0, 0);
    cout << ans << "\n";
    return 0;
}

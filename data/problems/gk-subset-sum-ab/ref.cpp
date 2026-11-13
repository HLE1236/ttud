#include <bits/stdc++.h>
using namespace std;
int n, A, B, a[25], ans = 0;
void Try(int idx, int sum) {
    if (idx == n) {
        if (sum >= A && sum <= B) ans++;
        return;
    }
    Try(idx + 1, sum);
    if (sum + a[idx] <= B) Try(idx + 1, sum + a[idx]);
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> A >> B;
    for (int i = 0; i < n; i++) cin >> a[i];
    Try(0, 0);
    cout << ans << "\n";
    return 0;
}

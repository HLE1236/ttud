#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, K;
    long long D;
    if (!(cin >> n >> K >> D)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    long long win = 0, ans = 0;
    for (int i = 0; i < K; i++) win += a[i];
    if (win >= D) ans++;
    for (int i = K; i < n; i++) {
        win += a[i] - a[i - K];
        if (win >= D) ans++;
    }
    cout << ans << "\n";
    return 0;
}

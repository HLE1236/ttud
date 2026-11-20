#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int L, k;
    cin >> L >> k;
    long long zeros = 0, ans = 0;
    for (int i = 0; i < L; ++i) {
        int x; cin >> x;
        if (x == 0) {
            zeros++;
            if (zeros >= k) ans++;
        } else {
            zeros = 0;
        }
    }
    cout << ans << "\n";
    return 0;
}

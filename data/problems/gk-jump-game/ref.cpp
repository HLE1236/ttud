#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n; cin >> n;
        vector<int> p(n);
        for (int i = 0; i < n; ++i) cin >> p[i];
        if (n <= 1) {
            cout << 0 << "\n";
            continue;
        }
        int jumps = 0, curEnd = 0, farthest = 0;
        bool ok = true;
        for (int i = 0; i < n - 1; ++i) {
            farthest = max(farthest, i + p[i]);
            if (i == curEnd) {
                if (farthest <= i) {
                    ok = false;
                    break;
                }
                jumps++;
                curEnd = farthest;
                if (curEnd >= n - 1) break;
            }
        }
        cout << (ok && curEnd >= n - 1 ? jumps : -1) << "\n";
    }
    return 0;
}

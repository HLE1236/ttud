#include <bits/stdc++.h>
using namespace std;
static long long pref[1005][1005];
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; if (!(cin >> n >> m)) return 0;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) {
            long long x; cin >> x;
            pref[i][j] = x + pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1];
        }
    int q; cin >> q;
    while (q--) {
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        cout << pref[r2][c2] - pref[r1 - 1][c2] - pref[r2][c1 - 1] + pref[r1 - 1][c1 - 1] << "\n";
    }
    return 0;
}

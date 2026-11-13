#include <bits/stdc++.h>
using namespace std;
static long long pref[10005][105];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    for (int i = 0; i < n; ++i) {
        int d, c; long long a;
        cin >> d >> c >> a;
        pref[d][c] += a;
    }
    for (int d = 1; d <= 10000; ++d) {
        for (int c = 1; c <= 100; ++c) {
            pref[d][c] += pref[d][c - 1];
        }
    }
    int m; cin >> m;
    while (m--) {
        int d, c1, c2;
        cin >> d >> c1 >> c2;
        cout << pref[d][c2] - pref[d][c1 - 1] << "\n";
    }
    return 0;
}

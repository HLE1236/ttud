#include <bits/stdc++.h>
using namespace std;
static int st[20][1000005];
static int lg2[1000005];
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    for (int i = 0; i < n; ++i) cin >> st[0][i];
    for (int i = 2; i <= n; ++i) lg2[i] = lg2[i >> 1] + 1;
    for (int k = 1; (1 << k) <= n; ++k) {
        int half = 1 << (k - 1), lim = n - (1 << k);
        for (int i = 0; i <= lim; ++i)
            st[k][i] = min(st[k - 1][i], st[k - 1][i + half]);
    }
    int m; cin >> m;
    long long Q = 0;
    while (m--) {
        int l, r; cin >> l >> r;
        int k = lg2[r - l + 1];
        Q += min(st[k][l], st[k][r - (1 << k) + 1]);
    }
    cout << Q << "\n";
    return 0;
}

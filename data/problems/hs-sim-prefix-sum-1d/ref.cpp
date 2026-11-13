#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) { long long x; cin >> x; pref[i] = pref[i - 1] + x; }
    int q; cin >> q;
    while (q--) { int i, j; cin >> i >> j; cout << pref[j] - pref[i - 1] << "\n"; }
    return 0;
}

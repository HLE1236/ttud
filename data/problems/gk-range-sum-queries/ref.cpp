#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long x; cin >> x;
        pref[i] = pref[i - 1] + x;
    }
    while (q--) {
        int i, k;
        cin >> i >> k;
        cout << pref[i + k - 1] - pref[i - 1] << "\n";
    }
    return 0;
}

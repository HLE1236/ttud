#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<long long> a(n + 1), bit(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        bit[i] += a[i];
        int j = i + (i & -i);
        if (j <= n) bit[j] += bit[i];
    }
    int q;
    cin >> q;
    while (q--) {
        string cmd;
        cin >> cmd;
        if (cmd == "sum") {
            int k; cin >> k;
            long long s = 0;
            for (int i = k; i > 0; i -= i & -i) s += bit[i];
            cout << s << '\n';
        } else {
            int k; long long v; cin >> k >> v;
            long long delta = v - a[k];
            a[k] = v;
            for (int i = k; i <= n; i += i & -i) bit[i] += delta;
        }
    }
    return 0;
}

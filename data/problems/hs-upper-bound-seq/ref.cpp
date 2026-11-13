#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    string cmd;
    while (cin >> cmd && cmd != "#") {
        int k; cin >> k;
        auto it = upper_bound(a.begin(), a.end(), k);
        cout << (it == a.end() ? -1 : *it) << "\n";
    }
    return 0;
}

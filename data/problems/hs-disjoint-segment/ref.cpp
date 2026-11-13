#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; if (!(cin >> n)) return 0;
    vector<pair<int, int>> seg(n);
    for (int i = 0; i < n; ++i) cin >> seg[i].second >> seg[i].first; // (b, a)
    sort(seg.begin(), seg.end());
    int last = -1, ans = 0;
    for (auto &[b, a] : seg) {
        if (a > last) { ans++; last = b; }
    }
    cout << ans << "\n";
    return 0;
}

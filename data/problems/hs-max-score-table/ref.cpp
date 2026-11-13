#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n;
    if (!(cin >> m >> n)) return 0;
    vector<vector<long long>> a(m, vector<long long>(n));
    for (int r = 0; r < m; ++r)
        for (int c = 0; c < n; ++c) cin >> a[r][c];
    vector<long long> cur(m), nxt(m);
    for (int r = 0; r < m; ++r) cur[r] = a[r][0];
    for (int c = 1; c < n; ++c) {
        for (int r = 0; r < m; ++r) {
            long long best = cur[r];
            if (r > 0) best = max(best, cur[r - 1]);
            if (r + 1 < m) best = max(best, cur[r + 1]);
            nxt[r] = best + a[r][c];
        }
        cur.swap(nxt);
    }
    cout << *max_element(cur.begin(), cur.end()) << "\n";
    return 0;
}

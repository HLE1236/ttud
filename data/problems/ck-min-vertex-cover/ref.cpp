#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> adj_mask(n, 0);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        --u; --v;
        if (u != v) {
            adj_mask[u] |= (1 << v);
            adj_mask[v] |= (1 << u);
        }
    }
    int min_sz = n + 1, cnt = 0;
    int full = (1 << n);
    for (int mask = 0; mask < full; ++mask) {
        int sz = __builtin_popcount(mask);
        if (sz > min_sz) continue;
        bool ok = true;
        for (int u = 0; u < n; ++u) {
            if (!((mask >> u) & 1)) {
                // Every neighbor of u must be in mask
                if ((adj_mask[u] & mask) != adj_mask[u]) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            if (sz < min_sz) { min_sz = sz; cnt = 1; }
            else if (sz == min_sz) cnt++;
        }
    }
    cout << min_sz << " " << cnt << "\n";
    return 0;
}

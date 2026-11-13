#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<pair<int, long long>>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v; long long w;
        cin >> u >> v >> w;
        --u; --v;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    int full = (1 << n);
    vector<long long> dp((size_t)full * n, -1);
    vector<int> par((size_t)full * n, -1);
    for (int u = 0; u < n; ++u) dp[(size_t)(1 << u) * n + u] = 0;

    long long best_w = 0;
    int best_mask = 1, best_u = 0;
    for (int mask = 1; mask < full; ++mask) {
        for (int u = 0; u < n; ++u) {
            long long cur = dp[(size_t)mask * n + u];
            if (cur < 0) continue;
            if (cur > best_w) {
                best_w = cur;
                best_mask = mask;
                best_u = u;
            }
            for (auto [v, w] : adj[u]) {
                if ((mask >> v) & 1) continue;
                int nmask = mask | (1 << v);
                size_t nidx = (size_t)nmask * n + v;
                if (cur + w > dp[nidx]) {
                    dp[nidx] = cur + w;
                    par[nidx] = u;
                }
            }
        }
    }
    vector<int> path;
    int cmask = best_mask, cu = best_u;
    while (cu != -1) {
        path.push_back(cu + 1);
        int pu = par[(size_t)cmask * n + cu];
        cmask ^= (1 << cu);
        cu = pu;
    }
    reverse(path.begin(), path.end());
    cout << path.size() << "\n";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << ' ';
        cout << path[i];
    }
    cout << "\n";
    return 0;
}

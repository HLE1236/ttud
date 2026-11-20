#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, long long>>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        u--; v--;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int full = 1 << n;
    // dp[mask][u]: tong trong so lon nhat khi da di qua tap dinh mask va ket thuc tai u
    vector<vector<long long>> dp(full, vector<long long>(n, -1));
    vector<vector<int>> trace(full, vector<int>(n, -1));

    for (int u = 0; u < n; u++) {
        dp[1 << u][u] = 0;
    }

    long long best_w = 0;
    int best_mask = 1, best_u = 0;

    for (int mask = 1; mask < full; mask++) {
        for (int u = 0; u < n; u++) {
            if (dp[mask][u] < 0) continue;
            if (dp[mask][u] > best_w) {
                best_w = dp[mask][u];
                best_mask = mask;
                best_u = u;
            }
            for (auto [v, w] : adj[u]) {
                if ((mask >> v) & 1) continue;
                int next_mask = mask | (1 << v);
                if (dp[mask][u] + w > dp[next_mask][v]) {
                    dp[next_mask][v] = dp[mask][u] + w;
                    trace[next_mask][v] = u;
                }
            }
        }
    }

    // Truy vet lai cac dinh tren duong di tot nhat
    vector<int> path;
    int cur_mask = best_mask, cur_u = best_u;
    while (cur_u != -1) {
        path.push_back(cur_u + 1);
        int prev_u = trace[cur_mask][cur_u];
        cur_mask ^= (1 << cur_u);
        cur_u = prev_u;
    }
    reverse(path.begin(), path.end());

    cout << path.size() << '\n';
    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i] << (i + 1 == (int)path.size() ? '\n' : ' ');
    }
    return 0;
}

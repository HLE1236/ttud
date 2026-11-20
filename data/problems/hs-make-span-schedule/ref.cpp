#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<long long> d(n + 1);
    for (int i = 1; i <= n; ++i) cin >> d[i];
    vector<vector<int>> adj(n + 1);
    vector<int> indeg(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        indeg[v]++;
    }
    vector<long long> start_t(n + 1, 0), fin(n + 1, 0);
    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) {
            fin[i] = d[i];
            q.push(i);
        }
    }
    long long ans = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ans = max(ans, fin[u]);
        for (int v : adj[u]) {
            start_t[v] = max(start_t[v], fin[u]);
            if (--indeg[v] == 0) {
                fin[v] = start_t[v] + d[v];
                q.push(v);
            }
        }
    }
    cout << ans << "\n";
    return 0;
}

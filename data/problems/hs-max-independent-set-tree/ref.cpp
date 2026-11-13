#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<long long> w(n + 1);
    for (int i = 1; i <= n; ++i) cin >> w[i];
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> par(n + 1, 0), ord;
    ord.reserve(n);
    queue<int> q;
    q.push(1);
    par[1] = -1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ord.push_back(u);
        for (int v : adj[u]) {
            if (v != par[u]) { par[v] = u; q.push(v); }
        }
    }
    vector<long long> dp0(n + 1, 0), dp1(n + 1, 0);
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = ord[idx];
        dp1[u] = w[u];
        dp0[u] = 0;
        for (int v : adj[u]) {
            if (v == par[u]) continue;
            dp1[u] += dp0[v];
            dp0[u] += max(dp0[v], dp1[v]);
        }
    }
    cout << max(dp0[1], dp1[1]) << "\n";
    return 0;
}

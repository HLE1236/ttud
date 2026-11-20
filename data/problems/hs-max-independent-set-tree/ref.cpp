#include <bits/stdc++.h>
using namespace std;

int n;
vector<long long> w, dp0, dp1;
vector<vector<int>> adj;

void dfs(int u, int p) {
    dp0[u] = 0;
    dp1[u] = w[u];
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs(v, u);
        dp0[u] += max(dp0[v], dp1[v]);
        dp1[u] += dp0[v];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    w.resize(n + 1);
    dp0.resize(n + 1);
    dp1.resize(n + 1);
    adj.resize(n + 1);

    for (int i = 1; i <= n; i++) cin >> w[i];
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, 0);
    cout << max(dp0[1], dp1[1]) << '\n';
    return 0;
}

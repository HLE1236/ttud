#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<vector<int>> adj;
vector<int> vis, order;

void dfs(int u) {
    vis[u] = 1;
    order.push_back(u);
    for (int v : adj[u]) {
        if (!vis[v]) dfs(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    adj.resize(n + 1);
    vis.assign(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        sort(adj[i].begin(), adj[i].end());
    }
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) dfs(i);
    }
    for (int i = 0; i < (int)order.size(); i++) {
        cout << order[i] << (i + 1 == (int)order.size() ? '\n' : ' ');
    }
    return 0;
}

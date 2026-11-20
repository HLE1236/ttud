#include <bits/stdc++.h>
using namespace std;

int n, m, timer_dfs = 0, bridges = 0;
vector<vector<pair<int, int>>> adj;
vector<int> num, low, is_arti;

void dfs(int u, int parent_edge) {
    num[u] = low[u] = ++timer_dfs;
    int children = 0;

    for (auto [v, edge_id] : adj[u]) {
        if (edge_id == parent_edge) continue;
        if (num[v] != 0) {
            low[u] = min(low[u], num[v]);
        } else {
            children++;
            dfs(v, edge_id);
            low[u] = min(low[u], low[v]);

            if (low[v] > num[u]) bridges++;
            if (parent_edge != -1 && low[v] >= num[u]) is_arti[u] = 1;
        }
    }
    if (parent_edge == -1 && children >= 2) is_arti[u] = 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    adj.resize(n + 1);
    num.assign(n + 1, 0);
    low.assign(n + 1, 0);
    is_arti.assign(n + 1, 0);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    for (int i = 1; i <= n; i++) {
        if (num[i] == 0) dfs(i, -1);
    }

    int arti_count = 0;
    for (int i = 1; i <= n; i++) arti_count += is_arti[i];
    cout << arti_count << ' ' << bridges << '\n';
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int n, m, timer_dfs = 0, visited_cnt = 0;
bool has_bridge = false;
vector<vector<pair<int, int>>> adj;
vector<int> num, low;
vector<pair<int, int>> oriented_edge;

void dfs(int u, int parent_edge) {
    num[u] = low[u] = ++timer_dfs;
    visited_cnt++;

    for (auto [v, id] : adj[u]) {
        if (id == parent_edge) continue;
        if (num[v] == 0) {
            oriented_edge[id] = {u, v}; // Canh cay DFS: dinh huong di xuong
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] > num[u]) has_bridge = true;
        } else if (num[v] < num[u]) {
            oriented_edge[id] = {u, v}; // Canh nguoc: dinh huong di len to tien
            low[u] = min(low[u], num[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    adj.resize(n + 1);
    num.assign(n + 1, 0);
    low.assign(n + 1, 0);
    oriented_edge.resize(m);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    dfs(1, -1);

    // Do thi vo huong dinh huong thanh SCC duoc khi va chi khi lien thong va khong co canh cau
    if (visited_cnt != n || has_bridge) {
        cout << -1 << '\n';
        return 0;
    }

    cout << m << '\n';
    for (int i = 0; i < m; i++) {
        cout << oriented_edge[i].first << ' ' << oriented_edge[i].second << '\n';
    }
    return 0;
}

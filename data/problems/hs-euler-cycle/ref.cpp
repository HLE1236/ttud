#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> adj(n + 1);
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        deg[u]++; deg[v]++;
    }
    int start = -1;
    for (int i = 1; i <= n; ++i) {
        if (deg[i] % 2 != 0) { cout << -1 << "\n"; return 0; }
        if (deg[i] > 0 && start == -1) start = i;
    }
    if (start == -1) start = 1;
    vector<char> used(m, 0);
    vector<size_t> ptr(n + 1, 0);
    vector<int> st = {start}, cyc;
    while (!st.empty()) {
        int u = st.back();
        while (ptr[u] < adj[u].size() && used[adj[u][ptr[u]].second]) ptr[u]++;
        if (ptr[u] < adj[u].size()) {
            auto [v, id] = adj[u][ptr[u]++];
            used[id] = 1;
            st.push_back(v);
        } else {
            cyc.push_back(u);
            st.pop_back();
        }
    }
    if ((int)cyc.size() != m + 1) {
        cout << -1 << "\n";
        return 0;
    }
    cout << cyc.size() << "\n";
    for (int i = (int)cyc.size() - 1; i >= 0; --i) {
        cout << cyc[i] << (i ? ' ' : '\n');
    }
    return 0;
}

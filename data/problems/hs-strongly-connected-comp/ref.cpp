#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
    }
    vector<int> tin(n + 1, 0), low(n + 1, 0);
    vector<char> in_st(n + 1, 0);
    vector<size_t> ptr(n + 1, 0);
    vector<int> scc_st, dfs_st;
    int timer = 0, scc_cnt = 0;

    for (int s = 1; s <= n; ++s) {
        if (tin[s]) continue;
        tin[s] = low[s] = ++timer;
        in_st[s] = 1;
        scc_st.push_back(s);
        dfs_st.push_back(s);
        while (!dfs_st.empty()) {
            int u = dfs_st.back();
            if (ptr[u] < adj[u].size()) {
                int v = adj[u][ptr[u]++];
                if (!tin[v]) {
                    tin[v] = low[v] = ++timer;
                    in_st[v] = 1;
                    scc_st.push_back(v);
                    dfs_st.push_back(v);
                } else if (in_st[v]) {
                    low[u] = min(low[u], tin[v]);
                }
            } else {
                dfs_st.pop_back();
                if (!dfs_st.empty()) {
                    int p = dfs_st.back();
                    low[p] = min(low[p], low[u]);
                }
                if (low[u] == tin[u]) {
                    scc_cnt++;
                    while (true) {
                        int x = scc_st.back(); scc_st.pop_back();
                        in_st[x] = 0;
                        if (x == u) break;
                    }
                }
            }
        }
    }
    cout << scc_cnt << "\n";
    return 0;
}

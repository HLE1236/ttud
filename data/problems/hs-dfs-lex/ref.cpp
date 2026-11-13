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
        adj[v].push_back(u);
    }
    for (int i = 1; i <= n; ++i) sort(adj[i].begin(), adj[i].end());
    vector<char> vis(n + 1, 0);
    vector<size_t> ptr(n + 1, 0);
    vector<int> order, st;
    for (int s = 1; s <= n; ++s) {
        if (vis[s]) continue;
        vis[s] = 1;
        order.push_back(s);
        st.push_back(s);
        while (!st.empty()) {
            int u = st.back();
            if (ptr[u] < adj[u].size()) {
                int v = adj[u][ptr[u]++];
                if (!vis[v]) {
                    vis[v] = 1;
                    order.push_back(v);
                    st.push_back(v);
                }
            } else {
                st.pop_back();
            }
        }
    }
    for (int i = 0; i < (int)order.size(); ++i) {
        if (i) cout << ' ';
        cout << order[i];
    }
    cout << "\n";
    return 0;
}

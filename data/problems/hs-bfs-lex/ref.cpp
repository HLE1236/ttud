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
    vector<int> order;
    queue<int> q;
    for (int s = 1; s <= n; ++s) {
        if (vis[s]) continue;
        vis[s] = 1;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (!vis[v]) { vis[v] = 1; q.push(v); }
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

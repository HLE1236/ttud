#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> col(n + 1, -1);
    queue<int> q;
    for (int s = 1; s <= n; ++s) {
        if (col[s] != -1) continue;
        col[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (col[v] == -1) {
                    col[v] = 1 - col[u];
                    q.push(v);
                } else if (col[v] == col[u]) {
                    cout << 0 << "\n";
                    return 0;
                }
            }
        }
    }
    cout << 1 << "\n";
    return 0;
}

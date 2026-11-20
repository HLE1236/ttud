#include <bits/stdc++.h>
using namespace std;

int n, m;
bool g[25][25], vis[25], found;

void dfs(int u, int cnt) {
    if (found) return;
    if (cnt == n) {
        if (g[u][1]) found = true;
        return;
    }
    for (int v = 1; v <= n; v++) {
        if (g[u][v] && !vis[v]) {
            vis[v] = true;
            dfs(v, cnt + 1);
            vis[v] = false;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        cin >> n >> m;
        memset(g, 0, sizeof(g));
        memset(vis, 0, sizeof(vis));
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            g[u][v] = g[v][u] = true;
        }
        found = false;
        vis[1] = true;
        dfs(1, 1);
        cout << (found ? 1 : 0) << '\n';
    }
    return 0;
}

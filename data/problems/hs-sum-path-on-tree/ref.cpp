#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<pair<int, long long>>> adj;
vector<long long> sz, f;
long long ans = 0;

// DFS 1: Tinh kich thuoc cay con sz[u] va tong khoang cach tu goc 1 (f[1])
void dfs1(int u, int p) {
    sz[u] = 1;
    for (auto [v, w] : adj[u]) {
        if (v == p) continue;
        dfs1(v, u);
        sz[u] += sz[v];
        f[1] += sz[v] * w;
    }
}

// DFS 2: Chuyen goc tu u xuong con v
void dfs2(int u, int p) {
    ans = max(ans, f[u]);
    for (auto [v, w] : adj[u]) {
        if (v == p) continue;
        f[v] = f[u] + w * (n - 2 * sz[v]);
        dfs2(v, u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    adj.resize(n + 1);
    sz.resize(n + 1);
    f.assign(n + 1, 0);

    for (int i = 1; i < n; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    dfs1(1, 0);
    dfs2(1, 0);
    cout << ans << '\n';
    return 0;
}

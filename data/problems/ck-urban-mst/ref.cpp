#include <bits/stdc++.h>
using namespace std;
struct DSU {
    vector<int> p;
    DSU(int n) : p(n + 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        p[a] = b; return true;
    }
};
struct Edge {
    int u, v;
    long long w;
    bool operator<(const Edge& o) const { return w < o.w; }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<long long> x(n + 1), y(n + 1);
    for (int i = 1; i <= n; ++i) cin >> x[i] >> y[i];
    DSU dsu(n);
    int m; cin >> m;
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        dsu.unite(u, v);
    }
    vector<Edge> edges;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (dsu.find(i) == dsu.find(j)) continue;
            long long dx = x[i] - x[j], dy = y[i] - y[j];
            edges.push_back({i, j, dx * dx + dy * dy});
        }
    }
    sort(edges.begin(), edges.end());
    long long ans = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) ans += e.w;
    }
    cout << ans << "\n";
    return 0;
}

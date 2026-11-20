#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int u, v, w; cin >> u >> v >> w;
        edges[i] = {w, u, v};
    }
    sort(edges.begin(), edges.end());
    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    auto findp = [&](int x) {
        while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
        return x;
    };
    long long mst = 0;
    int taken = 0;
    for (auto [w, u, v] : edges) {
        u = findp(u); v = findp(v);
        if (u != v) {
            p[v] = u;
            mst += w;
            if (++taken == n - 1) break;
        }
    }
    cout << mst << "\n";
    return 0;
}

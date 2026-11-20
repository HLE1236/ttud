#include <bits/stdc++.h>
using namespace std;
struct DSU {
    vector<int> p;
    DSU(int n) : p(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        p[a] = b; return true;
    }
};
struct Edge {
    int u, v; long long w;
    bool operator<(const Edge& o) const { return w < o.w; }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int S, P;
    cin >> S >> P;
    vector<long long> x(P), y(P);
    for (int i = 0; i < P; ++i) cin >> x[i] >> y[i];
    if (S >= P) {
        cout << 0 << "\n";
        return 0;
    }
    vector<Edge> edges;
    for (int i = 0; i < P; ++i)
        for (int j = i + 1; j < P; ++j) {
            long long dx = x[i] - x[j], dy = y[i] - y[j];
            edges.push_back({i, j, dx * dx + dy * dy});
        }
    sort(edges.begin(), edges.end());
    DSU dsu(P);
    int comp = P;
    long long ans = 0;
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            comp--;
            if (comp == S) {
                ans = e.w;
                break;
            }
        }
    }
    cout << ans << "\n";
    return 0;
}

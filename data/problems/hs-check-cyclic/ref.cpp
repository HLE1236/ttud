#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    auto findp = [&](int x) {
        while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
        return x;
    };
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        u = findp(u); v = findp(v);
        if (u == v) { cout << 1 << "\n"; return 0; }
        p[v] = u;
    }
    cout << 0 << "\n";
    return 0;
}

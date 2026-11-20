#include <bits/stdc++.h>
using namespace std;

int m, n, crd[35], load_gv[15], x[35], best = 1e9;
vector<int> teach[35];
bool conf[35][35];

void Try(int k, int cur_max) {
    if (cur_max >= best) return;
    if (k > n) { best = cur_max; return; }
    for (int v : teach[k]) {
        bool ok = (load_gv[v] + crd[k] < best);
        for (int i = 1; i < k && ok; i++)
            if (conf[i][k] && x[i] == v) ok = false;
        if (!ok) continue;

        x[k] = v;
        load_gv[v] += crd[k];
        Try(k + 1, max(cur_max, load_gv[v]));
        load_gv[v] -= crd[k];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> m >> n;
    for (int i = 1; i <= m; i++) {
        int k, c; cin >> k;
        while (k--) cin >> c, teach[c].push_back(i);
    }
    for (int i = 1; i <= n; i++) cin >> crd[i];
    int K, u, v; cin >> K;
    while (K--) cin >> u >> v, conf[u][v] = conf[v][u] = true;
    Try(1, 0);
    cout << (best == 1e9 ? -1 : best) << '\n';
    return 0;
}

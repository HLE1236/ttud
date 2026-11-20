#include <bits/stdc++.h>
using namespace std;

string canonical(int n) {
    int m; cin >> m;
    vector<vector<int>> adj(n, vector<int>(n, 0));
    vector<int> deg(n, 0);
    int edges = 0;
    while (m--) {
        int u, v; cin >> u >> v; --u; --v;
        if (u != v && !adj[u][v]) adj[u][v] = adj[v][u] = 1, deg[u]++, deg[v]++, edges++;
    }
    if (!edges || edges == n * (n - 1) / 2) return to_string(n) + "#" + to_string(edges);

    vector<vector<int>> sig(n);
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) if (adj[u][v]) sig[u].push_back(deg[v]);
        sort(sig[u].begin(), sig[u].end());
        sig[u].insert(sig[u].begin(), deg[u]);
    }
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b) { return sig[a] < sig[b]; });

    vector<pair<int, int>> grp;
    for (int i = 0, j = 0; i < n; i = j) {
        while (j < n && sig[ord[j]] == sig[ord[i]]) j++;
        grp.push_back({i, j});
    }
    string best;
    auto dfs = [&](auto& self, int g) -> void {
        if (g == (int)grp.size()) {
            string s;
            for (int i = 0; i < n; i++)
                for (int j = i + 1; j < n; j++) s += char('0' + adj[ord[i]][ord[j]]);
            if (best.empty() || s < best) best = move(s);
            return;
        }
        auto [l, r] = grp[g];
        sort(ord.begin() + l, ord.begin() + r);
        do { self(self, g + 1); } while (next_permutation(ord.begin() + l, ord.begin() + r));
    };
    dfs(dfs, 0);
    return to_string(n) + "#" + best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, n, K;
    cin >> N;
    unordered_set<string> db;
    while (N--) cin >> n, db.insert(canonical(n));
    string sep; cin >> sep >> K;
    while (K--) cin >> n, cout << db.count(canonical(n)) << '\n';
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

string canonical(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> adj(n, vector<int>(n, 0));
    vector<int> deg(n, 0);
    for (auto [u, v] : edges) {
        u--; v--;
        if (u != v && !adj[u][v]) {
            adj[u][v] = adj[v][u] = 1;
            deg[u]++; deg[v]++;
        }
    }
    int edgeCnt = 0;
    for (int i = 0; i < n; i++) edgeCnt += deg[i];
    edgeCnt /= 2;
    if (edgeCnt == 0 || edgeCnt == n * (n - 1) / 2) {
        return to_string(n) + "#" + to_string(edgeCnt);
    }

    // Group vertices by (deg, sorted neighbor degrees)
    vector<vector<int>> sig(n);
    for (int u = 0; u < n; u++) {
        sig[u].push_back(deg[u]);
        vector<int> nd;
        for (int v = 0; v < n; v++) if (adj[u][v]) nd.push_back(deg[v]);
        sort(nd.begin(), nd.end());
        for (int d : nd) sig[u].push_back(d);
    }
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) { return sig[a] < sig[b]; });

    vector<pair<int, int>> groups;
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && sig[order[j]] == sig[order[i]]) j++;
        groups.push_back({i, j});
        i = j;
    }

    string best;
    vector<int> cur = order;
    function<void(int)> gen = [&](int gidx) {
        if (gidx == (int)groups.size()) {
            string s;
            s.reserve(n * (n - 1) / 2);
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    s.push_back(adj[cur[i]][cur[j]] ? '1' : '0');
                }
            }
            if (best.empty() || s < best) best = move(s);
            return;
        }
        auto [l, r] = groups[gidx];
        sort(cur.begin() + l, cur.begin() + r);
        do {
            gen(gidx + 1);
        } while (next_permutation(cur.begin() + l, cur.begin() + r));
    };
    gen(0);
    return to_string(n) + "#" + best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    unordered_set<string> db;
    db.reserve(N * 2 + 16);
    for (int i = 0; i < N; i++) {
        int n, m; cin >> n >> m;
        vector<pair<int,int>> e(m);
        for (int j = 0; j < m; j++) cin >> e[j].first >> e[j].second;
        db.insert(canonical(n, e));
    }
    string hashMark;
    cin >> hashMark;
    int K;
    cin >> K;
    for (int i = 0; i < K; i++) {
        int n, m; cin >> n >> m;
        vector<pair<int,int>> e(m);
        for (int j = 0; j < m; j++) cin >> e[j].first >> e[j].second;
        cout << (db.count(canonical(n, e)) ? 1 : 0) << '\n';
    }
    return 0;
}

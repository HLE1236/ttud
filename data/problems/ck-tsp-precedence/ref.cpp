#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<int>> d(n + 1, vector<int>(n + 1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> d[i][j];
    int P;
    cin >> P;
    vector<vector<int>> adj(n + 1);
    vector<int> indeg(n + 1, 0);
    vector<vector<bool>> mustBefore(n + 1, vector<bool>(n + 1, false));
    for (int i = 0; i < P; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        indeg[v]++;
        mustBefore[u][v] = true;
    }
    // Topological greedy + nearest valid + 2-opt/swap preserving precedence constraints
    vector<int> tour;
    vector<bool> used(n + 1, false);
    int cur = 0;
    for (int step = 0; step < n; step++) {
        int best_v = -1, best_w = 1e9;
        for (int v = 1; v <= n; v++) {
            if (!used[v] && indeg[v] == 0) {
                int w = (cur == 0 ? 0 : d[cur][v]);
                if (w < best_w) { best_w = w; best_v = v; }
            }
        }
        tour.push_back(best_v);
        used[best_v] = true;
        cur = best_v;
        for (int nxt : adj[best_v]) indeg[nxt]--;
    }
    auto tour_cost = [&](const vector<int>& t) {
        long long c = 0;
        for (int i = 0; i + 1 < n; i++) c += d[t[i]][t[i + 1]];
        c += d[t[n - 1]][t[0]];
        return c;
    };
    auto valid = [&](const vector<int>& t) {
        vector<int> pos(n + 1);
        for (int i = 0; i < n; i++) pos[t[i]] = i;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (mustBefore[t[j]][t[i]]) return false;
        return true;
    };
    long long best_c = tour_cost(tour);
    bool improved = true;
    int iters = 0;
    while (improved && iters < 200) {
        improved = false;
        iters++;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                swap(tour[i], tour[j]);
                if (valid(tour)) {
                    long long nc = tour_cost(tour);
                    if (nc < best_c) { best_c = nc; improved = true; continue; }
                }
                swap(tour[i], tour[j]);
            }
        }
    }
    cout << n << "\n";
    for (int i = 0; i < n; i++) cout << tour[i] << (i + 1 == n ? "" : " ");
    cout << "\n";
    return 0;
}

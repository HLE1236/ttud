#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<vector<long long>> c(n, vector<long long>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) cin >> c[i][j];

    vector<int> tour;
    vector<bool> vis(n, false);
    int cur = 0;
    vis[0] = true;
    tour.push_back(0);
    for (int step = 1; step < n; ++step) {
        int nxt = -1;
        long long best_w = 4e18;
        for (int v = 0; v < n; ++v) {
            if (!vis[v] && c[cur][v] < best_w) {
                best_w = c[cur][v];
                nxt = v;
            }
        }
        vis[nxt] = true;
        tour.push_back(nxt);
        cur = nxt;
    }

    // 2-opt assuming general cost (verify segment reversal cost if n <= 400 or symmetric)
    bool sym = true;
    for (int i = 0; i < n && sym; ++i)
        for (int j = i + 1; j < n; ++j)
            if (c[i][j] != c[j][i]) { sym = false; break; }

    if (sym && n >= 4) {
        for (int pass = 0; pass < 30; ++pass) {
            bool imp = false;
            for (int i = 0; i < n; ++i) {
                int a = tour[i], b = tour[(i + 1) % n];
                for (int j = i + 2; j < n; ++j) {
                    if (i == 0 && j == n - 1) continue;
                    int u = tour[j], v = tour[(j + 1) % n];
                    if (c[a][u] + c[b][v] < c[a][b] + c[u][v]) {
                        reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                        imp = true;
                        b = tour[(i + 1) % n];
                    }
                }
            }
            if (!imp) break;
        }
    }

    cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << tour[i] + 1;
    }
    cout << "\n";
    return 0;
}

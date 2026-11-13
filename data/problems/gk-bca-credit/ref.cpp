#include <bits/stdc++.h>
using namespace std;
int m, n;
vector<int> D[35];
int credits[35], load[15], x[35];
bool conflict[35][35];
int best = 1e9;

bool check(int v, int k) {
    for (int i = 1; i < k; i++) {
        if (conflict[i][k] && x[i] == v) return false;
    }
    return true;
}

void Try(int k) {
    for (int v : D[k]) {
        if (check(v, k)) {
            x[k] = v;
            load[v] += credits[k];
            if (load[v] < best) {
                if (k == n) {
                    int mx = 0;
                    for (int i = 1; i <= m; i++) mx = max(mx, load[i]);
                    best = min(best, mx);
                } else {
                    Try(k + 1);
                }
            }
            load[v] -= credits[k];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> m >> n;
    for (int i = 1; i <= m; i++) {
        int k; cin >> k;
        while (k--) {
            int c; cin >> c;
            D[c].push_back(i);
        }
    }
    for (int i = 1; i <= n; i++) cin >> credits[i];
    int K; cin >> K;
    while (K--) {
        int u, v; cin >> u >> v;
        conflict[u][v] = conflict[v][u] = true;
    }
    Try(1);
    cout << (best == 1e9 ? -1 : best) << "\n";
    return 0;
}

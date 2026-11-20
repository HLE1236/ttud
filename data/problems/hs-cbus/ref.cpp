#include <bits/stdc++.h>
using namespace std;
int n, cap, c[25][25], x[25], min_out[25];
bool vis[25];
int load = 0, f = 0, best = 1e9, rem_lb = 0;

void dfs(int k) {
    for (int v = 1; v <= 2 * n; ++v) {
        if (vis[v]) continue;
        if (v <= n && load >= cap) continue;
        if (v > n && !vis[v - n]) continue;
        int cost = c[x[k - 1]][v];
        int nlb = rem_lb - min_out[v];
        if (k == 2 * n) { best = min(best, f + cost + c[v][0]); continue; }
        if (f + cost + nlb >= best) continue;
        x[k] = v; vis[v] = true; load += (v <= n ? 1 : -1); f += cost; rem_lb = nlb;
        dfs(k + 1);
        rem_lb += min_out[v]; f -= cost; load -= (v <= n ? 1 : -1); vis[v] = false;
    }
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n >> cap;
    for (int i = 0; i <= 2 * n; ++i) {
        min_out[i] = 1e9;
        for (int j = 0; j <= 2 * n; ++j) {
            cin >> c[i][j];
            if (i != j) min_out[i] = min(min_out[i], c[i][j]);
        }
    }
    for (int i = 1; i <= 2 * n; ++i) rem_lb += min_out[i];
    x[0] = 0; dfs(1);
    cout << best << "\n";
    return 0;
}

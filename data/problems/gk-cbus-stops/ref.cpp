#include <bits/stdc++.h>
using namespace std;

int n, cap, c[30][30], allow[15], x[30], pos[30], min_out[30];
bool vis[30];
int load = 0, f = 0, best = 1e9, rem_lb = 0;

void Try(int k) {
    int forced = -1;
    for (int i = 1; i <= n; i++) if (vis[i] && !vis[i + n]) {
        int ddl = pos[i] + allow[i] + 1;
        if (ddl < k || (ddl == k && forced != -1)) return;
        if (ddl == k) forced = i + n;
    }
    int lo = (forced != -1 ? forced : 1), hi = (forced != -1 ? forced : 2 * n);
    for (int v = lo; v <= hi; v++) {
        if (vis[v] || (v <= n && load >= cap) || (v > n && !vis[v - n])) continue;
        int cost = c[x[k - 1]][v], nlb = rem_lb - min_out[v];
        if (k == 2 * n) { best = min(best, f + cost + c[v][0]); continue; }
        if (f + cost + nlb >= best) continue;

        x[k] = v; pos[v] = k; vis[v] = true;
        load += (v <= n ? 1 : -1); f += cost; rem_lb = nlb;
        Try(k + 1);
        rem_lb += min_out[v]; f -= cost; load -= (v <= n ? 1 : -1);
        vis[v] = false;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> cap;
    for (int i = 0; i <= 2 * n; i++) {
        min_out[i] = 1e9;
        for (int j = 0; j <= 2 * n; j++) {
            cin >> c[i][j];
            if (i != j) min_out[i] = min(min_out[i], c[i][j]);
        }
        if (i >= 1) rem_lb += min_out[i];
    }
    for (int i = 1; i <= n; i++) cin >> allow[i];
    Try(1);
    cout << (best == 1e9 ? -1 : best) << '\n';
    return 0;
}

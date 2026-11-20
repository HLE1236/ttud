#include <bits/stdc++.h>
using namespace std;

int m, n;
bool canTeach[15][35], conf[35][35];
int crd[35], LB[15], loadT[15], cntT[15];
vector<int> order, teachers[35], assigned[15];
int best = 1e9;

void Try(int idx, int curMax, int need) {
    if (n - idx < need) return;
    if (idx == n) {
        best = min(best, curMax);
        return;
    }
    int c = order[idx];
    for (int t : teachers[c]) {
        if (loadT[t] + crd[c] >= best) continue;
        bool ok = true;
        for (int prev : assigned[t]) {
            if (conf[c][prev]) { ok = false; break; }
        }
        if (!ok) continue;

        int nNeed = need - (cntT[t] < LB[t] ? 1 : 0);
        if (n - 1 - idx < nNeed) continue;

        assigned[t].push_back(c);
        loadT[t] += crd[c];
        cntT[t]++;
        Try(idx + 1, max(curMax, loadT[t]), nNeed);
        cntT[t]--;
        loadT[t] -= crd[c];
        assigned[t].pop_back();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> m >> n;
    for (int t = 1; t <= m; t++) {
        int k; cin >> k;
        while (k--) {
            int c; cin >> c;
            canTeach[t][c] = true;
        }
    }
    for (int c = 1; c <= n; c++) cin >> crd[c];
    int need = 0;
    for (int t = 1; t <= m; t++) { cin >> LB[t]; need += LB[t]; }
    int k; cin >> k;
    while (k--) {
        int u, v; cin >> u >> v;
        conf[u][v] = conf[v][u] = true;
    }
    for (int c = 1; c <= n; c++) {
        for (int t = 1; t <= m; t++) if (canTeach[t][c]) teachers[c].push_back(t);
        if (teachers[c].empty()) { cout << -1 << '\n'; return 0; }
        order.push_back(c);
    }
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (teachers[a].size() != teachers[b].size())
            return teachers[a].size() < teachers[b].size();
        return crd[a] > crd[b];
    });
    Try(0, 0, need);
    cout << (best >= 1e9 ? -1 : best) << '\n';
    return 0;
}

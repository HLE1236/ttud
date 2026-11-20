#include <bits/stdc++.h>
using namespace std;
int m, n, D;
int c[15][25], load[15];
int order[25], sufMin[25];
long long best = 1e18;

void dfs(int idx, long long cur) {
    if (cur + sufMin[idx] >= best) return;
    if (idx == n) {
        best = cur;
        return;
    }
    int t = order[idx];
    // Try staff ordered by cheapest cost for task t
    int staff_ord[15];
    for (int i = 0; i < m; ++i) staff_ord[i] = i;
    sort(staff_ord, staff_ord + m, [&](int a, int b) {
        return c[a][t] < c[b][t];
    });
    for (int k = 0; k < m; ++k) {
        int i = staff_ord[k];
        if (load[i] < D) {
            if (cur + c[i][t] + sufMin[idx + 1] >= best) continue;
            load[i]++;
            dfs(idx + 1, cur + c[i][t]);
            load[i]--;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> m >> n >> D;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            cin >> c[i][j];
    if (n > m * D) {
        cout << -1 << "\n";
        return 0;
    }
    vector<int> min1(n, 1e9), gap(n, 0);
    for (int j = 0; j < n; ++j) {
        int m1 = 1e9, m2 = 1e9;
        for (int i = 0; i < m; ++i) {
            if (c[i][j] < m1) { m2 = m1; m1 = c[i][j]; }
            else if (c[i][j] < m2) m2 = c[i][j];
        }
        min1[j] = m1;
        gap[j] = (m > 1 ? m2 - m1 : 0);
        order[j] = j;
    }
    sort(order, order + n, [&](int a, int b) { return gap[a] > gap[b]; });
    sufMin[n] = 0;
    for (int i = n - 1; i >= 0; --i) sufMin[i] = sufMin[i + 1] + min1[order[i]];
    dfs(0, 0);
    cout << (best >= 1e17 ? -1 : best) << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n;
    long long P;
    cin >> m >> n >> P;
    vector<long long> r(n);
    for (int i = 0; i < n; ++i) cin >> r[i];
    vector<long long> q(m), c(m);
    for (int j = 0; j < m; ++j) cin >> q[j] >> c[j];

    vector<int> ord_o(n);
    iota(ord_o.begin(), ord_o.end(), 0);
    sort(ord_o.begin(), ord_o.end(), [&](int a, int b){ return r[a] > r[b]; });

    vector<int> ord_t(m);
    iota(ord_t.begin(), ord_t.end(), 0);
    sort(ord_t.begin(), ord_t.end(), [&](int a, int b){
        return P * q[a] - c[a] > P * q[b] - c[b];
    });

    vector<int> assign_t(n, -1);
    vector<long long> rem_cap = q;
    vector<bool> used_truck(m, false);

    // Greedy fill trucks by decreasing profitability and FFD on used trucks
    for (int oi : ord_o) {
        int chosen = -1;
        for (int tj : ord_t) {
            if (used_truck[tj] && rem_cap[tj] >= r[oi]) {
                chosen = tj; break;
            }
        }
        if (chosen != -1) {
            assign_t[oi] = chosen + 1;
            rem_cap[chosen] -= r[oi];
        } else {
            // Try opening an unused truck if batch of remaining fitting orders covers c[tj]
            int best_t = -1;
            long long best_gain = 0;
            for (int tj : ord_t) {
                if (used_truck[tj] || q[tj] < r[oi]) continue;
                long long cap_left = q[tj], load = 0;
                for (int ok : ord_o) {
                    if (assign_t[ok] == -1 && r[ok] <= cap_left) {
                        cap_left -= r[ok];
                        load += r[ok];
                    }
                }
                long long gain = P * load - c[tj];
                if (gain > best_gain) { best_gain = gain; best_t = tj; }
            }
            if (best_t != -1) {
                used_truck[best_t] = true;
                assign_t[oi] = best_t + 1;
                rem_cap[best_t] -= r[oi];
            }
        }
    }

    cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << assign_t[i];
    }
    cout << "\n";
    return 0;
}

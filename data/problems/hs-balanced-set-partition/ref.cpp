#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<long long> w(n);
    for (int i = 0; i < n; ++i) cin >> w[i];
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b){ return w[a] > w[b]; });
    vector<long long> load(m + 1, 0);
    vector<int> x(n);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int j = 1; j <= m; ++j) pq.push({0, j});
    for (int i : ord) {
        auto [cur, b] = pq.top(); pq.pop();
        x[i] = b;
        load[b] += w[i];
        pq.push({load[b], b});
    }
    // Simple local search: try moving an item from max bin to min bin
    for (int iter = 0; iter < 500 && m > 1; ++iter) {
        int bmax = 1, bmin = 1;
        for (int j = 2; j <= m; ++j) {
            if (load[j] > load[bmax]) bmax = j;
            if (load[j] < load[bmin]) bmin = j;
        }
        long long diff = load[bmax] - load[bmin];
        if (diff <= 1) break;
        bool improved = false;
        for (int i = 0; i < n; ++i) {
            if (x[i] == bmax && w[i] < diff) {
                load[bmax] -= w[i];
                load[bmin] += w[i];
                x[i] = bmin;
                improved = true;
                break;
            }
        }
        if (!improved) break;
    }
    cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << x[i];
    }
    cout << "\n";
    return 0;
}

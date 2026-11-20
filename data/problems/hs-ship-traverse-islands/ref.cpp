#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; long long D;
    cin >> n >> D;
    vector<long long> x(n + 1), y(n + 1);
    for (int i = 1; i <= n; ++i) cin >> x[i] >> y[i];
    if (n == 1) { cout << 0 << "\n"; return 0; }
    long long D2 = D * D;
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 1);
    sort(ord.begin(), ord.end(), [&](int a, int b){ return x[a] < x[b]; });
    vector<long long> sx(n);
    for (int i = 0; i < n; ++i) sx[i] = x[ord[i]];

    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == n) { cout << dist[n] << "\n"; return 0; }
        int l = (int)(lower_bound(sx.begin(), sx.end(), x[u] - D) - sx.begin());
        int r = (int)(upper_bound(sx.begin(), sx.end(), x[u] + D) - sx.begin());
        for (int idx = l; idx < r; ++idx) {
            int v = ord[idx];
            if (dist[v] != -1) continue;
            long long dx = x[u] - x[v], dy = y[u] - y[v];
            if (dx * dx + dy * dy <= D2) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    cout << -1 << "\n";
    return 0;
}

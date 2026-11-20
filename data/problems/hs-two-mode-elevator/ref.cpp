#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, a, b, X, Y;
    cin >> n >> a >> b >> X >> Y;
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[X] = 0;
    q.push(X);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == Y) { cout << dist[u] << "\n"; return 0; }
        int up = u + a, dn = u - b;
        if (up <= n && dist[up] == -1) { dist[up] = dist[u] + 1; q.push(up); }
        if (dn >= 1 && dist[dn] == -1) { dist[dn] = dist[u] + 1; q.push(dn); }
    }
    cout << -1 << "\n";
    return 0;
}

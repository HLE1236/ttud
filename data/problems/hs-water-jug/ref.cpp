#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int a, b, c;
    cin >> a >> b >> c;
    if (c > max(a, b) || c % std::gcd(a, b) != 0) {
        cout << -1 << '\n';
        return 0;
    }
    vector<vector<int>> dist(a + 1, vector<int>(b + 1, -1));
    queue<pair<int, int>> q;
    dist[0][0] = 0;
    q.push({0, 0});
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == c || y == c) {
            cout << dist[x][y] << '\n';
            return 0;
        }
        int d = dist[x][y] + 1;
        auto push_st = [&](int nx, int ny) {
            if (dist[nx][ny] == -1) {
                dist[nx][ny] = d;
                q.push({nx, ny});
            }
        };
        push_st(a, y);
        push_st(x, b);
        push_st(0, y);
        push_st(x, 0);
        int p1 = min(x, b - y);
        push_st(x - p1, y + p1);
        int p2 = min(y, a - x);
        push_st(x + p2, y - p2);
    }
    cout << -1 << '\n';
    return 0;
}

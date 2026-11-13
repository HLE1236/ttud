#include <bits/stdc++.h>
using namespace std;
struct Point {
    long long x, y, c;
    bool operator<(const Point& o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<Point> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i].x >> p[i].y >> p[i].c;
    sort(p.begin(), p.end());
    vector<long long> dp(n);
    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        dp[i] = p[i].c;
        for (int j = 0; j < i; ++j) {
            if (p[j].y <= p[i].y) {
                dp[i] = max(dp[i], dp[j] + p[i].c);
            }
        }
        ans = max(ans, dp[i]);
    }
    cout << ans << "\n";
    return 0;
}

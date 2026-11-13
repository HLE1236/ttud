#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, x;
    if (!(cin >> n >> x)) return 0;
    vector<int> d(n);
    for (int i = 0; i < n; ++i) cin >> d[i];
    const int INF = 1e9;
    vector<int> dp(x + 1, INF);
    dp[0] = 0;
    for (int coin : d) {
        for (int s = coin; s <= x; ++s) {
            if (dp[s - coin] + 1 < dp[s]) dp[s] = dp[s - coin] + 1;
        }
    }
    cout << (dp[x] >= INF ? -1 : dp[x]) << "\n";
    return 0;
}

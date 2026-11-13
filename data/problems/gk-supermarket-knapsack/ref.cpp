#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, W;
    if (!(cin >> n >> W)) return 0;
    vector<int> cost(n), val(n);
    for (int i = 0; i < n; ++i) cin >> cost[i];
    for (int i = 0; i < n; ++i) cin >> val[i];
    vector<long long> dp(W + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (int w = cost[i]; w <= W; ++w) {
            dp[w] = max(dp[w], dp[w - cost[i]] + val[i]);
        }
    }
    cout << dp[W] << "\n";
    return 0;
}

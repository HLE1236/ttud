#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<pair<int, int>> seg(n);
    for (int i = 0; i < n; ++i) cin >> seg[i].first >> seg[i].second;
    sort(seg.begin(), seg.end(), [](const auto& A, const auto& B) {
        if (A.second != B.second) return A.second < B.second;
        return A.first < B.first;
    });
    vector<int> ends(n);
    for (int i = 0; i < n; ++i) ends[i] = seg[i].second;
    vector<long long> dp(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int a = seg[i - 1].first, b = seg[i - 1].second;
        int j = (int)(lower_bound(ends.begin(), ends.begin() + i - 1, a) - ends.begin());
        dp[i] = max(dp[i - 1], (long long)(b - a) + dp[j]);
    }
    cout << dp[n] << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m, n;
    cin >> m >> n;
    vector<vector<int>> a(m, vector<int>(n));
    for (int r = 0; r < m; ++r)
        for (int c = 0; c < n; ++c) cin >> a[r][c];
    vector<int> masks;
    for (int mask = 0; mask < (1 << m); ++mask)
        if ((mask & (mask << 1)) == 0) masks.push_back(mask);
    int K = (int)masks.size();
    vector<vector<int>> compat(K);
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            if ((masks[i] & masks[j]) == 0) compat[i].push_back(j);

    vector<long long> dp(K, 0), nxt(K, 0);
    for (int c = 0; c < n; ++c) {
        fill(nxt.begin(), nxt.end(), 0);
        for (int i = 0; i < K; ++i) {
            long long s = 0;
            for (int r = 0; r < m; ++r)
                if ((masks[i] >> r) & 1) s += a[r][c];
            long long best_prev = 0;
            for (int pj : compat[i]) best_prev = max(best_prev, dp[pj]);
            nxt[i] = best_prev + s;
        }
        dp.swap(nxt);
    }
    cout << *max_element(dp.begin(), dp.end()) << "\n";
    return 0;
}

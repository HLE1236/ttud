#include <bits/stdc++.h>
using namespace std;
int main() {
    int n; string s; cin >> n >> s;
    int m = (int)s.size();
    vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    vector<long long> dp(m, 0);
    dp[0] = 1;
    for (int step = 0; step < n; ++step) {
        vector<long long> ndp(m, 0);
        for (int matched = 0; matched < m; ++matched) {
            if (!dp[matched]) continue;
            for (char ch : {'0', '1'}) {
                int j = matched;
                while (j > 0 && s[j] != ch) j = pi[j - 1];
                if (s[j] == ch) j++;
                if (j < m) ndp[j] += dp[matched];
            }
        }
        dp.swap(ndp);
    }
    long long ans = 0;
    for (long long v : dp) ans += v;
    cout << ans << "\n";
    return 0;
}

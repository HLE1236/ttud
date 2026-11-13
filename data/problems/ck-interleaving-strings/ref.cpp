#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string A, B, C;
    if (!(cin >> A >> B >> C)) return 0;
    int n = (int)A.size(), m = (int)B.size(), k = (int)C.size();
    if (n + m != k) {
        cout << "F\n";
        return 0;
    }
    vector<bool> dp(m + 1, false);
    dp[0] = true;
    for (int j = 1; j <= m; ++j) dp[j] = dp[j - 1] && (B[j - 1] == C[j - 1]);
    for (int i = 1; i <= n; ++i) {
        dp[0] = dp[0] && (A[i - 1] == C[i - 1]);
        for (int j = 1; j <= m; ++j) {
            bool takeA = (A[i - 1] == C[i + j - 1]) && dp[j];
            bool takeB = (B[j - 1] == C[i + j - 1]) && dp[j - 1];
            dp[j] = takeA || takeB;
        }
    }
    cout << (dp[m] ? "T" : "F") << "\n";
    return 0;
}

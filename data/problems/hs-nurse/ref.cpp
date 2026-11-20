#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K1, K2;
    cin >> N >> K1 >> K2;
    const long long MOD = 1000000007LL;
    vector<long long> dp0(N + 1, 0), dp1(N + 1, 0);
    dp0[0] = 1;
    dp1[0] = 1;
    for (int i = 1; i <= N; ++i) {
        dp0[i] = dp1[i - 1] % MOD;
        long long s = 0;
        for (int k = K1; k <= K2; ++k) {
            if (i - k >= 0) s = (s + dp0[i - k]) % MOD;
        }
        dp1[i] = s;
    }
    cout << (dp0[N] + dp1[N]) % MOD << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
int C[1005][1005];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int k, n;
    if (!(cin >> k >> n)) return 0;
    if (k > n) { cout << 0 << '\n'; return 0; }
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) {
            C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
        }
    }
    cout << C[n][k] << '\n';
    return 0;
}

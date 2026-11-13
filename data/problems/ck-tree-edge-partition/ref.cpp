#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        deg[u]++;
        deg[v]++;
    }
    const int MOD = 10000;
    vector<int> pow2(n + 1, 1);
    for (int i = 1; i <= n; i++) pow2[i] = (pow2[i - 1] * 2) % MOD;
    long long ans = 0;
    for (int u = 1; u <= n; u++) {
        if (deg[u] >= 2) {
            ans = (ans + pow2[deg[u] - 1] - 1 + MOD) % MOD;
        }
    }
    cout << ans << "\n";
    return 0;
}

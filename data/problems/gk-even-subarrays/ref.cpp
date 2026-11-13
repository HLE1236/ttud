#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    long long even_cnt = 1, odd_cnt = 0, sum = 0;
    const long long MOD = 1000000007LL;
    for (int i = 0; i < n; i++) {
        long long x; cin >> x;
        sum += x;
        if (sum % 2 == 0) even_cnt++;
        else odd_cnt++;
    }
    long long ans = (even_cnt * (even_cnt - 1) / 2 + odd_cnt * (odd_cnt - 1) / 2) % MOD;
    cout << ans << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
static const long long MOD = 1000000007LL;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> cnt(1000001, 0);
    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        int x; cin >> x;
        ans = (ans + cnt[x]) % MOD;
        cnt[x]++;
    }
    cout << ans << "\n";
    return 0;
}

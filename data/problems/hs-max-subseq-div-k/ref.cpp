#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, K;
    if (!(cin >> n >> K)) return 0;
    const long long INF = 4e18;
    vector<long long> min_pref(K, INF);
    min_pref[0] = 0;
    long long pref = 0, ans = -INF;
    for (int i = 0; i < n; ++i) {
        long long x; cin >> x;
        pref += x;
        int r = (int)(((pref % K) + K) % K);
        if (min_pref[r] < INF / 2) ans = max(ans, pref - min_pref[r]);
        min_pref[r] = min(min_pref[r], pref);
    }
    if (ans < -INF / 2) cout << "NOT_FOUND\n";
    else cout << ans << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    const long long INF = 4e18;
    long long min_pref[2] = {0, INF};
    long long pref = 0, ans = -INF;
    for (int i = 0; i < n; ++i) {
        long long x; cin >> x;
        pref += x;
        int p = (int)((pref % 2 + 2) % 2);
        if (min_pref[p] < INF / 2) ans = max(ans, pref - min_pref[p]);
        min_pref[p] = min(min_pref[p], pref);
    }
    if (ans < -INF / 2) cout << "NOT_FOUND\n";
    else cout << ans << "\n";
    return 0;
}

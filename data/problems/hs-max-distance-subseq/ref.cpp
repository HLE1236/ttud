#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int N, C; cin >> N >> C;
        vector<long long> x(N);
        for (int i = 0; i < N; ++i) cin >> x[i];
        sort(x.begin(), x.end());
        auto ok = [&](long long D) {
            int cnt = 1; long long last = x[0];
            for (int i = 1; i < N; ++i) {
                if (x[i] - last >= D) { cnt++; last = x[i]; if (cnt >= C) return true; }
            }
            return cnt >= C;
        };
        long long lo = 0, hi = x.back() - x.front(), ans = 0;
        while (lo <= hi) {
            long long mid = lo + (hi - lo) / 2;
            if (ok(mid)) { ans = mid; lo = mid + 1; } else hi = mid - 1;
        }
        cout << ans << "\n";
    }
    return 0;
}

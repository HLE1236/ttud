#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, K;
    cin >> n >> K;
    vector<pair<long long, long long>> items(n);
    for (int i = 0; i < n; ++i) cin >> items[i].first >> items[i].second;
    sort(items.begin(), items.end());
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) pref[i] = pref[i - 1] + items[i - 1].second;
    long long total = pref[n];
    if (K == 1) {
        cout << 0 << "\n";
        return 0;
    }

    auto feasible = [&](long long L, long long R) -> bool {
        if (L > R) return false;
        vector<int> can(n + 1, 0), psum(n + 2, 0);
        can[0] = 1;
        for (int k = 1; k <= K; ++k) {
            psum[0] = 0;
            for (int i = 0; i <= n; ++i) psum[i + 1] = psum[i] + can[i];
            fill(can.begin(), can.end(), 0);
            int l_ptr = 0, r_ptr = -1;
            for (int i = 1; i <= n; ++i) {
                while (r_ptr + 1 < i && pref[i] - pref[r_ptr + 1] >= L) r_ptr++;
                while (l_ptr < i && pref[i] - pref[l_ptr] > R) l_ptr++;
                if (l_ptr <= r_ptr && (psum[r_ptr + 1] - psum[l_ptr] > 0)) {
                    can[i] = 1;
                }
            }
        }
        return can[n] == 1;
    };

    vector<long long> candL;
    long long maxL = total / K;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            long long s = pref[j] - pref[i];
            if (s <= maxL) candL.push_back(s);
        }
    }
    sort(candL.rbegin(), candL.rend());
    candL.erase(unique(candL.begin(), candL.end()), candL.end());

    long long best = total;
    for (long long L : candL) {
        long long min_possible_max = (total - L + K - 2) / (K - 1);
        if (min_possible_max - L >= best) break;
        long long hi_bound = min(total, L + best - 1);
        if (!feasible(L, hi_bound)) continue;
        long long lo = max(L, min_possible_max), hi = hi_bound, ansR = hi_bound;
        while (lo <= hi) {
            long long mid = lo + (hi - lo) / 2;
            if (feasible(L, mid)) {
                ansR = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        best = min(best, ansR - L);
        if (best == 0) break;
    }
    cout << best << "\n";
    return 0;
}

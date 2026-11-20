#include <bits/stdc++.h>
using namespace std;
int n;
long long a[25], suf[25], s[3], best = 1e18;
int cnt[3];

void dfs(int idx) {
    if (idx == n) {
        if (cnt[0] == 0 || cnt[1] == 0 || cnt[2] == 0) return;
        long long mx = max({s[0], s[1], s[2]});
        long long mn = min({s[0], s[1], s[2]});
        best = min(best, mx - mn);
        return;
    }
    int empty_groups = (cnt[0] == 0) + (cnt[1] == 0) + (cnt[2] == 0);
    if (n - idx < empty_groups) return;

    long long mx = max({s[0], s[1], s[2]});
    long long mn = min({s[0], s[1], s[2]});
    if (mx - (mn + suf[idx]) >= best) return;
    long long total = s[0] + s[1] + s[2] + suf[idx];
    // Lower bound: max group >= mx, average of other two <= (total - mx)/2 -> min <= (total - mx)/2
    long long lb = mx - (total - mx) / 2;
    if (lb >= best) return;

    for (int g = 0; g < 3; ++g) {
        if (g > 0 && cnt[g] == cnt[g - 1] && s[g] == s[g - 1]) continue;
        s[g] += a[idx];
        cnt[g]++;
        dfs(idx + 1);
        cnt[g]--;
        s[g] -= a[idx];
        if (cnt[g] == 0) break; // symmetry breaking for empty groups
        if (best == 0) return;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a, a + n, greater<long long>());
    suf[n] = 0;
    for (int i = n - 1; i >= 0; --i) suf[i] = suf[i + 1] + a[i];
    dfs(0);
    cout << best << "\n";
    return 0;
}

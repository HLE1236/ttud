#include <bits/stdc++.h>
using namespace std;
int n;
long long total = 0, best = 1e18;
vector<long long> a;
void dfs(int idx, long long s1) {
    long long diff = llabs(total - 2 * s1);
    if (diff < best) best = diff;
    if (best == 0 || idx == n) return;
    if (2 * s1 - total >= best) return;
    dfs(idx + 1, s1 + a[idx]);
    dfs(idx + 1, s1);
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> n)) return 0;
    a.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        total += a[i];
    }
    sort(a.rbegin(), a.rend());
    dfs(1, a[0]);
    cout << best << "\n";
    return 0;
}

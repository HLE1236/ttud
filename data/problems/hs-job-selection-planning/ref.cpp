#include <bits/stdc++.h>
using namespace std;
int par[100005];
int find_slot(int t) { return par[t] == t ? t : par[t] = find_slot(par[t]); }
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<pair<long long, int>> jobs(n);
    int maxd = n;
    for (int i = 0; i < n; ++i) {
        cin >> jobs[i].second >> jobs[i].first;
        jobs[i].second = min(jobs[i].second, n);
    }
    iota(par, par + maxd + 1, 0);
    sort(jobs.rbegin(), jobs.rend());
    long long ans = 0;
    for (auto &[p, d] : jobs) {
        int s = find_slot(d);
        if (s > 0) {
            ans += p;
            par[s] = find_slot(s - 1);
        }
    }
    cout << ans << "\n";
    return 0;
}

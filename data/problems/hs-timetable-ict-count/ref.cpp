#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> d(n), v(n);
    for (int i = 0; i < n; ++i) {
        string code; cin >> code >> d[i] >> v[i];
    }
    int m; cin >> m;
    int max_t = 0;
    struct Room { int cap; vector<pair<int,int>> pers; };
    vector<Room> rooms(m);
    for (int r = 0; r < m; ++r) {
        string code; int k;
        cin >> code >> rooms[r].cap >> k;
        for (int p = 0; p < k; ++p) {
            int s, l; cin >> s >> l;
            rooms[r].pers.push_back({s, l});
            max_t = max(max_t, s + l + 2);
        }
    }
    vector<vector<pair<int, int>>> cands(n); // (start_t, room_count)
    for (int i = 0; i < n; ++i) {
        vector<int> cnt(max_t + 1, 0);
        for (int r = 0; r < m; ++r) {
            if (rooms[r].cap < v[i]) continue;
            for (auto [s, l] : rooms[r].pers) {
                for (int st = s; st + d[i] <= s + l; ++st) cnt[st]++;
            }
        }
        for (int st = 0; st <= max_t; ++st)
            if (cnt[st] > 0) cands[i].push_back({st, cnt[st]});
    }
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b){
        return cands[a].size() < cands[b].size();
    });

    vector<char> occ(max_t + 1, 0);
    unsigned long long ans = 0;
    auto dfs = [&](auto& self, int pos, unsigned long long prod) -> void {
        if (pos == n) { ans += prod; return; }
        int i = ord[pos];
        for (auto [st, w] : cands[i]) {
            bool ok = true;
            for (int t = st; t < st + d[i]; ++t) {
                if (occ[t]) { ok = false; break; }
            }
            if (!ok) continue;
            for (int t = st; t < st + d[i]; ++t) occ[t] = 1;
            self(self, pos + 1, prod * (unsigned long long)w);
            for (int t = st; t < st + d[i]; ++t) occ[t] = 0;
        }
    };
    dfs(dfs, 0, 1);
    cout << ans << "\n";
    return 0;
}

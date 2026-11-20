#include <bits/stdc++.h>
using namespace std;
struct Cls { int d, v; };
struct Room { int cap; vector<pair<int,int>> pers; };
int n, m, max_t = 0, lb = 0, best = 1e9;
vector<Cls> cls;
vector<Room> rooms;
vector<vector<pair<int, int>>> cands; // (r, st)
vector<char> occ;
vector<int> room_load;

void dfs(int idx, int cur_max) {
    if (cur_max >= best || best == lb) return;
    if (idx == n) {
        best = cur_max;
        return;
    }
    int d = cls[idx].d;
    vector<tuple<int, int, int>> choices; // (new_load, r, st)
    for (auto [r, st] : cands[idx]) {
        int nload = room_load[r] + d;
        if (nload >= best) continue;
        bool ok = true;
        for (int t = st; t < st + d; ++t) if (occ[t]) { ok = false; break; }
        if (ok) choices.push_back({nload, r, st});
    }
    sort(choices.begin(), choices.end());
    for (auto [nload, r, st] : choices) {
        if (nload >= best || best == lb) break;
        for (int t = st; t < st + d; ++t) occ[t] = 1;
        room_load[r] = nload;
        dfs(idx + 1, max(cur_max, nload));
        room_load[r] -= d;
        for (int t = st; t < st + d; ++t) occ[t] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    cls.resize(n);
    int sum_d = 0, max_d = 0;
    for (int i = 0; i < n; ++i) {
        string code; cin >> code >> cls[i].d >> cls[i].v;
        sum_d += cls[i].d;
        max_d = max(max_d, cls[i].d);
    }
    sort(cls.begin(), cls.end(), [](const Cls& a, const Cls& b){
        if (a.v != b.v) return a.v > b.v;
        return a.d > b.d;
    });
    cin >> m;
    lb = max(max_d, (sum_d + m - 1) / m);
    rooms.resize(m);
    for (int r = 0; r < m; ++r) {
        string code; int k;
        cin >> code >> rooms[r].cap >> k;
        for (int p = 0; p < k; ++p) {
            int s, l; cin >> s >> l;
            rooms[r].pers.push_back({s, l});
            max_t = max(max_t, s + l + 2);
        }
    }
    cands.resize(n);
    for (int i = 0; i < n; ++i) {
        for (int r = 0; r < m; ++r) {
            if (rooms[r].cap < cls[i].v) continue;
            for (auto [s, l] : rooms[r].pers)
                for (int st = s; st + cls[i].d <= s + l; ++st)
                    cands[i].push_back({r, st});
        }
    }
    occ.assign(max_t + 1, 0);
    room_load.assign(m, 0);
    dfs(0, 0);
    cout << (best >= 1e8 ? -1 : best) << "\n";
    return 0;
}

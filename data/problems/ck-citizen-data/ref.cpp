#include <bits/stdc++.h>
using namespace std;

struct Person { int dad = 0, mom = 0, alive = 0; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Person> P(1);
    unordered_map<string, int> id_of;
    auto get_id = [&](const string& s) {
        if (s == "0000000") return 0;
        auto [it, ok] = id_of.emplace(s, (int)P.size());
        if (ok) P.push_back({});
        return it->second;
    };

    unordered_map<string, int> born_at;
    vector<string> dates;
    string code, dob, dad, mom, alive, reg;
    while (cin >> code && code != "*") {
        cin >> dob >> dad >> mom >> alive >> reg;
        int u = get_id(code), f = get_id(dad), m = get_id(mom);
        P[u] = {f, m, alive == "Y"};
        born_at[dob]++;
        dates.push_back(dob);
    }
    sort(dates.begin(), dates.end());
    int n = (int)P.size() - 1;

    // Đồ thị cha/mẹ - con là đồ thị 2 phía -> Tập độc lập lớn nhất = n - MaxMatching
    int cached_mis = -1;
    auto solve_mis = [&]() {
        if (cached_mis != -1) return cached_mis;
        vector<vector<int>> adj(n + 1);
        for (int u = 1; u <= n; u++) {
            for (int p : {P[u].dad, P[u].mom}) if (p) {
                adj[u].push_back(p);
                adj[p].push_back(u);
            }
        }
        vector<int> col(n + 1, -1), L, matchR(n + 1, 0), vis(n + 1, 0);
        for (int i = 1; i <= n; i++) if (col[i] == -1) {
            queue<int> q; q.push(i); col[i] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                if (!col[u]) L.push_back(u);
                for (int v : adj[u]) if (col[v] == -1) col[v] = col[u] ^ 1, q.push(v);
            }
        }
        auto dfs = [&](auto& self, int u, int step) -> bool {
            if (vis[u] == step) return false;
            vis[u] = step;
            for (int v : adj[u])
                if (!matchR[v] || self(self, matchR[v], step))
                    return matchR[v] = u, true;
            return false;
        };
        int matched = 0, step = 0;
        for (int u : L) if (dfs(dfs, u, ++step)) matched++;
        return cached_mis = n - matched;
    };

    string q;
    while (cin >> q && q != "***") {
        if (q == "NUMBER_PEOPLE") cout << (int)dates.size() << '\n';
        else if (q == "NUMBER_PEOPLE_BORN_AT") { string d; cin >> d; cout << born_at[d] << '\n'; }
        else if (q == "NUMBER_PEOPLE_BORN_BETWEEN") {
            string d1, d2; cin >> d1 >> d2;
            cout << upper_bound(dates.begin(), dates.end(), d2) - lower_bound(dates.begin(), dates.end(), d1) << '\n';
        } else if (q == "MOST_ALIVE_ANCESTOR") {
            string c; cin >> c;
            int u = id_of[c], ans = 0;
            queue<pair<int, int>> bfs;
            for (int p : {P[u].dad, P[u].mom}) if (p) bfs.push({p, 1});
            while (!bfs.empty()) {
                auto [x, d] = bfs.front(); bfs.pop();
                if (P[x].alive) ans = max(ans, d);
                for (int p : {P[x].dad, P[x].mom}) if (p) bfs.push({p, d + 1});
            }
            cout << ans << '\n';
        } else if (q == "MAX_UNRELATED_PEOPLE") cout << solve_mis() << '\n';
    }
    return 0;
}

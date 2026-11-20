#include <bits/stdc++.h>
using namespace std;

int toSec(const string& t) {
    return ((t[0] - '0') * 10 + t[1] - '0') * 3600
         + ((t[3] - '0') * 10 + t[4] - '0') * 60
         + ((t[6] - '0') * 10 + t[7] - '0');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, int> id_of;
    vector<vector<int>> adj;
    auto get_id = [&](const string& s) {
        auto [it, ok] = id_of.emplace(s, (int)adj.size());
        if (ok) adj.emplace_back();
        return it->second;
    };

    long long total_money = 0;
    unordered_map<string, long long> from_money;
    const int N = 86400;
    vector<int> seg(2 * N, 0);

    string u, v, tm, atm; int money;
    while (cin >> u && u != "#") {
        cin >> v >> money >> tm >> atm;
        total_money += money;
        from_money[u] += money;
        int s = toSec(tm) + N;
        seg[s] = max(seg[s], money);
        int iu = get_id(u), iv = get_id(v);
        adj[iu].push_back(iv);
    }
    for (auto& vec : adj) {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }
    // Cây Segment Tree lặp (Iterative Segment Tree) chỉ dài 6 dòng
    for (int i = N - 1; i > 0; i--) seg[i] = max(seg[i << 1], seg[i << 1 | 1]);
    auto query_max = [&](int l, int r) {
        int res = 0;
        for (l += N, r += N + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = max(res, seg[l++]);
            if (r & 1) res = max(res, seg[--r]);
        }
        return res;
    };

    vector<int> vis(adj.size(), 0);
    auto dfs = [&](auto& self, int cur, int st, int rem) -> bool {
        if (rem == 1) return binary_search(adj[cur].begin(), adj[cur].end(), st);
        for (int nxt : adj[cur]) if (!vis[nxt]) {
            vis[nxt] = 1;
            if (self(self, nxt, st, rem - 1)) return vis[nxt] = 0, true;
            vis[nxt] = 0;
        }
        return false;
    };

    string q;
    while (cin >> q && q != "#") {
        if (q == "?total_money_transaction") cout << total_money << '\n';
        else if (q == "?total_money_transaction_from") { string a; cin >> a; cout << from_money[a] << '\n'; }
        else if (q == "?max_money_period") {
            string t1, t2; cin >> t1 >> t2;
            cout << query_max(toSec(t1), toSec(t2)) << '\n';
        } else if (q == "?inspect_cycle") {
            string a; int k; cin >> a >> k;
            if (!id_of.count(a)) cout << 0 << '\n';
            else {
                int st = id_of[a];
                vis[st] = 1;
                cout << dfs(dfs, st, st, k) << '\n';
                vis[st] = 0;
            }
        }
    }
    return 0;
}

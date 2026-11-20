#include <bits/stdc++.h>
using namespace std;

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

    long long cnt = 0, total = 0;
    unordered_map<string, long long> out_sum;
    set<string> accs;
    string u, v, tm, atm; long long money;
    while (cin >> u && u != "#") {
        cin >> v >> money >> tm >> atm;
        cnt++; total += money; out_sum[u] += money;
        accs.insert(u); accs.insert(v);
        int iu = get_id(u), iv = get_id(v);
        adj[iu].push_back(iv);
    }
    for (auto& vec : adj) {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }

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
        if (q == "?number_transactions") cout << cnt << '\n';
        else if (q == "?total_money_transaction") cout << total << '\n';
        else if (q == "?list_sorted_accounts") {
            bool first = true;
            for (auto& s : accs) cout << (first ? "" : " ") << s, first = false;
            cout << '\n';
        } else if (q == "?total_money_transaction_from") {
            string a; cin >> a; cout << out_sum[a] << '\n';
        } else if (q == "?inspect_cycle") {
            string a; int k; cin >> a >> k;
            if (!id_of.count(a) || k <= 0) cout << 0 << '\n';
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

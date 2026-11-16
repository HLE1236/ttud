#include <bits/stdc++.h>
using namespace std;

unordered_map<string, int> idMap;
int getId(const string& s) {
    auto it = idMap.find(s);
    if (it != idMap.end()) return it->second;
    int id = (int)idMap.size();
    return idMap[s] = id;
}

vector<vector<int>> adj;
vector<char> vis;

bool dfs(int u, int start, int rem) {
    if (rem == 1) {
        for (int v : adj[u]) if (v == start) return true;
        return false;
    }
    for (int v : adj[u]) {
        if (!vis[v] && v != start) {
            vis[v] = 1;
            if (dfs(v, start, rem - 1)) return true;
            vis[v] = 0;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long cnt = 0, total = 0;
    unordered_map<string, long long> outSum;
    set<string> accs;
    vector<pair<int, int>> edges;
    string u, v, tm, atm;
    long long money;
    while (cin >> u && u != "#") {
        cin >> v >> money >> tm >> atm;
        cnt++;
        total += money;
        outSum[u] += money;
        accs.insert(u);
        accs.insert(v);
        edges.push_back({getId(u), getId(v)});
    }
    adj.resize(idMap.size());
    for (auto [a, b] : edges) adj[a].push_back(b);
    for (auto& vec : adj) {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }
    vis.assign(idMap.size(), 0);

    string q;
    while (cin >> q && q != "#") {
        if (q == "?number_transactions") cout << cnt << '\n';
        else if (q == "?total_money_transaction") cout << total << '\n';
        else if (q == "?list_sorted_accounts") {
            bool first = true;
            for (const string& s : accs) {
                if (!first) cout << ' ';
                cout << s;
                first = false;
            }
            cout << '\n';
        } else if (q == "?total_money_transaction_from") {
            string acc; cin >> acc;
            auto it = outSum.find(acc);
            cout << (it == outSum.end() ? 0 : it->second) << '\n';
        } else if (q == "?inspect_cycle") {
            string acc; int k; cin >> acc >> k;
            auto it = idMap.find(acc);
            if (it == idMap.end() || k <= 0) cout << 0 << '\n';
            else {
                fill(vis.begin(), vis.end(), 0);
                int st = it->second;
                vis[st] = 1;
                cout << (dfs(st, st, k) ? 1 : 0) << '\n';
            }
        }
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int toSec(const string& t) {
    int h = (t[0]-'0')*10 + (t[1]-'0');
    int m = (t[3]-'0')*10 + (t[4]-'0');
    int s = (t[6]-'0')*10 + (t[7]-'0');
    return h*3600 + m*60 + s;
}

unordered_map<string, int> id_map;
int get_id(const string& s) {
    auto it = id_map.find(s);
    if (it != id_map.end()) return it->second;
    int id = id_map.size();
    return id_map[s] = id;
}

vector<vector<int>> adj;
vector<int> vis;

bool dfs_cycle(int u, int start, int rem) {
    if (rem == 1) {
        for (int v : adj[u]) if (v == start) return true;
        return false;
    }
    for (int v : adj[u]) {
        if (!vis[v] && v != start) {
            vis[v] = 1;
            if (dfs_cycle(v, start, rem - 1)) return true;
            vis[v] = 0;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long total_money = 0;
    unordered_map<string, long long> from_money;
    // Bucket max money per second of day [0..86399] + Segment Tree
    vector<int> sec_max(86400, 0);
    string u, v, tm, atm;
    int money;
    vector<pair<int,int>> edges;
    while (cin >> u && u != "#") {
        cin >> v >> money >> tm >> atm;
        total_money += money;
        from_money[u] += money;
        int s = toSec(tm);
        sec_max[s] = max(sec_max[s], money);
        int iu = get_id(u), iv = get_id(v);
        edges.push_back({iu, iv});
    }
    adj.resize(id_map.size());
    for (auto &e : edges) adj[e.first].push_back(e.second);
    for (auto &vec : adj) {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }
    // Build segment tree on 86400 seconds
    int SZ = 86400;
    vector<int> seg(4 * SZ, 0);
    auto build = [&](auto& self, int node, int l, int r) -> void {
        if (l == r) { seg[node] = sec_max[l]; return; }
        int mid = (l + r) / 2;
        self(self, node*2, l, mid);
        self(self, node*2+1, mid+1, r);
        seg[node] = max(seg[node*2], seg[node*2+1]);
    };
    build(build, 1, 0, SZ - 1);
    auto query = [&](auto& self, int node, int l, int r, int ql, int qr) -> int {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return seg[node];
        int mid = (l + r) / 2;
        return max(self(self, node*2, l, mid, ql, qr), self(self, node*2+1, mid+1, r, ql, qr));
    };

    vis.assign(id_map.size(), 0);
    string q;
    while (cin >> q && q != "#") {
        if (q == "?total_money_transaction") {
            cout << total_money << "\n";
        } else if (q == "?total_money_transaction_from") {
            string acc; cin >> acc;
            cout << from_money[acc] << "\n";
        } else if (q == "?inspect_cycle") {
            string acc; int k; cin >> acc >> k;
            if (id_map.find(acc) == id_map.end()) {
                cout << 0 << "\n";
            } else {
                int start = id_map[acc];
                fill(vis.begin(), vis.end(), 0);
                vis[start] = 1;
                cout << (dfs_cycle(start, start, k) ? 1 : 0) << "\n";
            }
        } else if (q == "?max_money_period") {
            string t1, t2; cin >> t1 >> t2;
            int l = toSec(t1), r = toSec(t2);
            cout << query(query, 1, 0, SZ - 1, l, r) << "\n";
        }
    }
    return 0;
}

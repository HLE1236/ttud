#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<int>> cap(n + 1, vector<int>(n + 1, 0)), adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, c; cin >> u >> v >> c;
        if (!cap[u][v]) adj[u].push_back(v);
        cap[u][v] += c;
    }
    vector<pair<int, int>> req(k);
    for (auto& [s, d] : req) cin >> s >> d;

    auto bfs = [&](int s, int d) -> vector<int> {
        vector<int> par(n + 1, 0);
        queue<int> q;
        q.push(s); par[s] = -1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == d) break;
            for (int v : adj[u]) if (cap[u][v] > 0 && !par[v])
                par[v] = u, q.push(v);
        }
        if (!par[d]) return {};
        vector<int> p;
        for (int v = d; v != -1; v = par[v]) p.push_back(v);
        reverse(p.begin(), p.end());
        return p;
    };

    // Ưu tiên các yêu cầu có đường đi ngắn nhất trước để tiết kiệm băng chuyền
    vector<int> dist(k, 1e9), ord(k);
    for (int i = 0; i < k; i++) {
        auto p = bfs(req[i].first, req[i].second);
        if (!p.empty()) dist[i] = (int)p.size();
        ord[i] = i;
    }
    sort(ord.begin(), ord.end(), [&](int a, int b) { return dist[a] < dist[b]; });

    vector<vector<int>> ans(k);
    for (int i : ord) {
        auto p = bfs(req[i].first, req[i].second);
        if (!p.empty()) {
            for (int j = 0; j + 1 < p.size(); j++) cap[p[j]][p[j + 1]]--;
            ans[i] = move(p);
        }
    }

    for (int i = 0; i < k; i++) {
        if (ans[i].empty()) cout << -1 << '\n';
        else {
            for (int j = 0; j < ans[i].size(); j++)
                cout << ans[i][j] << (j + 1 == ans[i].size() ? '\n' : ' ');
        }
    }
    return 0;
}

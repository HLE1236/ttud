#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, K;
    if (!(cin >> N >> M >> K)) return 0;
    vector<vector<int>> cap(N + 1, vector<int>(N + 1, 0));
    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < M; i++) {
        int u, v, c; cin >> u >> v >> c;
        if (cap[u][v] == 0) adj[u].push_back(v);
        cap[u][v] += c;
    }
    vector<pair<int, int>> req(K);
    for (int i = 0; i < K; i++) cin >> req[i].first >> req[i].second;

    auto findPath = [&](int s, int d) -> vector<int> {
        vector<int> par(N + 1, -1);
        queue<int> q;
        par[s] = s; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == d) break;
            for (int v : adj[u]) {
                if (cap[u][v] > 0 && par[v] == -1) {
                    par[v] = u;
                    q.push(v);
                }
            }
        }
        if (par[d] == -1) return {};
        vector<int> path;
        for (int v = d; v != s; v = par[v]) path.push_back(v);
        path.push_back(s);
        reverse(path.begin(), path.end());
        return path;
    };

    // Sort requests by initial shortest hop distance ascending
    vector<int> initDist(K, 1e9);
    for (int i = 0; i < K; i++) {
        auto p = findPath(req[i].first, req[i].second);
        if (!p.empty()) initDist[i] = (int)p.size();
    }
    vector<int> order(K);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return initDist[a] < initDist[b];
    });

    vector<vector<int>> ans(K);
    vector<vector<int>> edgeUsers((N + 1) * (N + 1) + 5);
    auto eid = [&](int u, int v) { return u * (N + 1) + v; };

    auto applyPath = [&](int id, const vector<int>& p, int sign) {
        for (size_t i = 0; i + 1 < p.size(); i++) {
            cap[p[i]][p[i + 1]] -= sign;
            if (sign > 0) edgeUsers[eid(p[i], p[i + 1])].push_back(id);
        }
    };

    for (int idx : order) {
        auto [s, d] = req[idx];
        auto p = findPath(s, d);
        if (!p.empty()) {
            ans[idx] = p;
            applyPath(idx, p, 1);
        } else {
            // Try 1-step reroute: find saturated edges reachable from s
            vector<int> par(N + 1, -1);
            queue<int> q;
            par[s] = s; q.push(s);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (cap[u][v] > 0 && par[v] == -1) {
                        par[v] = u; q.push(v);
                    }
                }
            }
            bool ok = false;
            for (int u = 1; u <= N && !ok; u++) {
                if (par[u] == -1) continue;
                for (int v : adj[u]) {
                    if (cap[u][v] == 0) {
                        // check if v can reach d
                        auto p2 = findPath(v, d);
                        if (p2.empty()) continue;
                        // check if any user of (u, v) can reroute without (u, v)
                        auto& users = edgeUsers[eid(u, v)];
                        for (int ui = (int)users.size() - 1; ui >= 0; ui--) {
                            int r2 = users[ui];
                            if (ans[r2].empty()) continue;
                            // verify r2 still uses (u,v)
                            bool uses = false;
                            for (size_t j = 0; j + 1 < ans[r2].size(); j++)
                                if (ans[r2][j] == u && ans[r2][j + 1] == v) { uses = true; break; }
                            if (!uses) continue;
                            // temporarily remove r2's path, disallow (u,v), try rerouting r2
                            for (size_t j = 0; j + 1 < ans[r2].size(); j++) cap[ans[r2][j]][ans[r2][j + 1]] += 1;
                            int saved = cap[u][v];
                            cap[u][v] = 0;
                            auto alt = findPath(req[r2].first, req[r2].second);
                            cap[u][v] = saved;
                            if (!alt.empty()) {
                                ans[r2] = alt;
                                applyPath(r2, alt, 1);
                                auto pNew = findPath(s, d);
                                if (!pNew.empty()) {
                                    ans[idx] = pNew;
                                    applyPath(idx, pNew, 1);
                                    ok = true;
                                    break;
                                }
                            } else {
                                for (size_t j = 0; j + 1 < ans[r2].size(); j++) cap[ans[r2][j]][ans[r2][j + 1]] -= 1;
                            }
                        }
                    }
                    if (ok) break;
                }
            }
        }
    }

    for (int i = 0; i < K; i++) {
        if (ans[i].empty()) cout << -1 << '\n';
        else {
            for (size_t j = 0; j < ans[i].size(); j++) {
                if (j) cout << ' ';
                cout << ans[i][j];
            }
            cout << '\n';
        }
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

struct Edge { int to, rev; long long cap; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<vector<int>> a(N, vector<int>(M));
    int S = -1, T = 2 * N * M;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) cin >> a[i][j];

    int V = T + 1;
    vector<vector<Edge>> g(V);
    auto addEdge = [&](int u, int v, long long cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    };
    const long long INF = 1e12;
    auto inId = [&](int r, int c) { return 2 * (r * M + c); };
    auto outId = [&](int r, int c) { return 2 * (r * M + c) + 1; };
    int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (a[i][j] == -1) {
                S = inId(i, j);
                addEdge(inId(i, j), outId(i, j), INF);
            } else if (a[i][j] == -2) {
                addEdge(inId(i, j), outId(i, j), INF);
                addEdge(outId(i, j), T, INF);
            } else if (a[i][j] > 0) {
                addEdge(inId(i, j), outId(i, j), a[i][j]);
            }
            if (a[i][j] != 0 && a[i][j] != -2) {
                for (int k = 0; k < 4; k++) {
                    int ni = i + dr[k], nj = j + dc[k];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < M && a[ni][nj] != 0 && a[ni][nj] != -1) {
                        addEdge(outId(i, j), inId(ni, nj), INF);
                    }
                }
            }
        }
    }

    vector<int> lvl(V), ptr(V);
    auto bfs = [&]() -> bool {
        fill(lvl.begin(), lvl.end(), -1);
        queue<int> q;
        lvl[S] = 0; q.push(S);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge& e : g[u]) {
                if (e.cap > 0 && lvl[e.to] == -1) {
                    lvl[e.to] = lvl[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return lvl[T] != -1;
    };
    auto dfs = [&](auto& self, int u, long long pushed) -> long long {
        if (pushed == 0 || u == T) return pushed;
        for (int& cid = ptr[u]; cid < (int)g[u].size(); cid++) {
            Edge& e = g[u][cid];
            if (e.cap <= 0 || lvl[e.to] != lvl[u] + 1) continue;
            long long tr = self(self, e.to, min(pushed, e.cap));
            if (tr == 0) continue;
            e.cap -= tr;
            g[e.to][e.rev].cap += tr;
            return tr;
        }
        return 0;
    };
    long long flow = 0;
    while (bfs()) {
        fill(ptr.begin(), ptr.end(), 0);
        while (long long pushed = dfs(dfs, S, INF)) flow += pushed;
    }
    cout << flow << '\n';
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
struct Edge {
    int to;
    long long dt, cost, openT, closeT;
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<vector<Edge>> adj(N);
    for (int i = 0; i < M; ++i) {
        int u, v, type;
        long long d, t, toll, op, cl;
        cin >> u >> v >> d >> t >> type >> toll >> op >> cl;
        long long dt = d * t;
        long long cost = dt + toll;
        adj[u].push_back({v, dt, cost, op, cl});
        if (type == 2) adj[v].push_back({u, dt, cost, op, cl});
    }
    int S, T; cin >> S >> T;
    // Pop by smallest cost (then smallest time).
    // Once a state arrives at u with (cost, time), any later state at u has >= cost,
    // so it is only useful if its time is strictly smaller than min_time[u]!
    using State = tuple<long long, long long, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    vector<long long> min_time(N, (long long)4e18);
    pq.push({0, 0, S});
    long long ans = -1;
    while (!pq.empty()) {
        auto [c, tm, u] = pq.top(); pq.pop();
        if (tm >= min_time[u]) continue;
        min_time[u] = tm;
        if (u == T) {
            ans = c;
            break;
        }
        for (const auto& e : adj[u]) {
            long long depart = max(tm, e.openT);
            long long arrive = depart + e.dt;
            if (arrive <= e.closeT && arrive < min_time[e.to]) {
                pq.push({c + e.cost, arrive, e.to});
            }
        }
    }
    cout << ans << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    struct Person {
        int father = 0, mother = 0;
        bool alive = false;
    };
    vector<Person> P(1); // 1-indexed
    unordered_map<string, int> code2id;
    auto get_id = [&](const string& code) -> int {
        if (code == "0000000") return 0;
        auto it = code2id.find(code);
        if (it != code2id.end()) return it->second;
        int id = P.size();
        P.push_back(Person());
        return code2id[code] = id;
    };

    unordered_map<string, int> born_cnt;
    vector<string> all_dates;
    int total_people = 0;

    string code, dob, fcode, mcode, alive_str, region;
    while (cin >> code && code != "*") {
        cin >> dob >> fcode >> mcode >> alive_str >> region;
        total_people++;
        int id = get_id(code);
        int fid = get_id(fcode);
        int mid = get_id(mcode);
        P[id].father = fid;
        P[id].mother = mid;
        P[id].alive = (alive_str == "Y");
        born_cnt[dob]++;
        all_dates.push_back(dob);
    }

    sort(all_dates.begin(), all_dates.end());

    // Build undirected graph for MAX_UNRELATED_PEOPLE (bipartite / forest of parent-child edges)
    int N = P.size() - 1;
    vector<vector<int>> g(N + 1);
    for (int i = 1; i <= N; i++) {
        if (P[i].father) {
            g[i].push_back(P[i].father);
            g[P[i].father].push_back(i);
        }
        if (P[i].mother) {
            g[i].push_back(P[i].mother);
            g[P[i].mother].push_back(i);
        }
    }

    int max_unrelated_cached = -1;
    auto calc_max_unrelated = [&]() -> int {
        if (max_unrelated_cached != -1) return max_unrelated_cached;
        // Parent-child graph has no odd cycles (generations alternate) -> Tree DP if tree, or check connected components!
        // Note: In a general bipartite graph with small components or tree-like structure:
        // Let's check if any component has cycles; actually let's solve exact MIS per component:
        // Tree DP if acyclic, else Hopcroft-Karp Maximum Matching (Konig's theorem: MIS = V - MaxMatching on bipartite graph!)
        vector<int> color(N + 1, -1);
        vector<int> left_nodes;
        for (int i = 1; i <= N; i++) {
            if (color[i] == -1) {
                queue<int> q;
                q.push(i);
                color[i] = 0;
                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    if (color[u] == 0) left_nodes.push_back(u);
                    for (int v : g[u]) {
                        if (color[v] == -1) {
                            color[v] = color[u] ^ 1;
                            q.push(v);
                        }
                    }
                }
            }
        }
        // Hopcroft-Karp on left_nodes
        vector<int> pairU(N + 1, 0), pairV(N + 1, 0), dist(N + 1, 0);
        const int INF = 1e9;
        auto bfs = [&]() -> bool {
            queue<int> q;
            for (int u : left_nodes) {
                if (pairU[u] == 0) { dist[u] = 0; q.push(u); }
                else dist[u] = INF;
            }
            int d_nil = INF;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                if (dist[u] < d_nil) {
                    for (int v : g[u]) {
                        int pu = pairV[v];
                        if (pu == 0) d_nil = dist[u] + 1;
                        else if (dist[pu] == INF) {
                            dist[pu] = dist[u] + 1;
                            q.push(pu);
                        }
                    }
                }
            }
            return d_nil != INF;
        };
        auto dfs = [&](auto& self, int u, int d_nil) -> bool {
            for (int v : g[u]) {
                int pu = pairV[v];
                if (pu == 0) {
                    if (dist[u] + 1 <= d_nil) {
                        pairV[v] = u; pairU[u] = v;
                        return true;
                    }
                } else if (dist[pu] == dist[u] + 1) {
                    if (self(self, pu, d_nil)) {
                        pairV[v] = u; pairU[u] = v;
                        return true;
                    }
                }
            }
            dist[u] = INF;
            return false;
        };
        int matching = 0;
        while (bfs()) {
            for (int u : left_nodes) {
                if (pairU[u] == 0 && dfs(dfs, u, INF)) matching++;
            }
        }
        return max_unrelated_cached = N - matching;
    };

    string q;
    while (cin >> q && q != "***") {
        if (q == "NUMBER_PEOPLE") {
            cout << total_people << "\n";
        } else if (q == "NUMBER_PEOPLE_BORN_AT") {
            string d; cin >> d;
            cout << born_cnt[d] << "\n";
        } else if (q == "NUMBER_PEOPLE_BORN_BETWEEN") {
            string d1, d2; cin >> d1 >> d2;
            auto it1 = lower_bound(all_dates.begin(), all_dates.end(), d1);
            auto it2 = upper_bound(all_dates.begin(), all_dates.end(), d2);
            cout << (it2 - it1) << "\n";
        } else if (q == "MOST_ALIVE_ANCESTOR") {
            string c; cin >> c;
            int id = code2id[c];
            int best_gen = 0;
            queue<pair<int,int>> bfsq;
            if (P[id].father) bfsq.push({P[id].father, 1});
            if (P[id].mother) bfsq.push({P[id].mother, 1});
            while (!bfsq.empty()) {
                auto [u, gen] = bfsq.front(); bfsq.pop();
                if (P[u].alive) best_gen = max(best_gen, gen);
                if (P[u].father) bfsq.push({P[u].father, gen + 1});
                if (P[u].mother) bfsq.push({P[u].mother, gen + 1});
            }
            cout << best_gen << "\n";
        } else if (q == "MAX_UNRELATED_PEOPLE") {
            cout << calc_max_unrelated() << "\n";
        }
    }
    return 0;
}

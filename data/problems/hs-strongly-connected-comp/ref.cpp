#include <bits/stdc++.h>
using namespace std;

int n, m, timer_dfs = 0, scc_count = 0;
vector<vector<int>> adj;
vector<int> num, low, in_stack, st;

void dfs(int u) {
    num[u] = low[u] = ++timer_dfs;
    st.push_back(u);
    in_stack[u] = 1;

    for (int v : adj[u]) {
        if (num[v] == 0) {
            dfs(v);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) {
            low[u] = min(low[u], num[v]);
        }
    }

    if (low[u] == num[u]) {
        scc_count++;
        while (true) {
            int x = st.back();
            st.pop_back();
            in_stack[x] = 0;
            if (x == u) break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    adj.resize(n + 1);
    num.assign(n + 1, 0);
    low.assign(n + 1, 0);
    in_stack.assign(n + 1, 0);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    for (int i = 1; i <= n; i++) {
        if (num[i] == 0) dfs(i);
    }
    cout << scc_count << '\n';
    return 0;
}

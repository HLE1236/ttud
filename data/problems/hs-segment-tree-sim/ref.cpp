#include <bits/stdc++.h>
using namespace std;
int n; vector<int> a, st;
void build(int id, int l, int r) {
    if (l == r) { st[id] = a[l]; return; }
    int mid = (l + r) >> 1;
    build(id << 1, l, mid); build(id << 1 | 1, mid + 1, r);
    st[id] = max(st[id << 1], st[id << 1 | 1]);
}
void upd(int id, int l, int r, int pos, int val) {
    if (l == r) { st[id] = val; return; }
    int mid = (l + r) >> 1;
    if (pos <= mid) upd(id << 1, l, mid, pos, val);
    else upd(id << 1 | 1, mid + 1, r, pos, val);
    st[id] = max(st[id << 1], st[id << 1 | 1]);
}
int qry(int id, int l, int r, int u, int v) {
    if (v < l || r < u) return -1e9;
    if (u <= l && r <= v) return st[id];
    int mid = (l + r) >> 1;
    return max(qry(id << 1, l, mid, u, v), qry(id << 1 | 1, mid + 1, r, u, v));
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n;
    a.resize(n + 1); st.resize(4 * n + 5);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    int m; cin >> m;
    while (m--) {
        string cmd; cin >> cmd;
        if (cmd == "update") { int i, v; cin >> i >> v; upd(1, 1, n, i, v); }
        else { int i, j; cin >> i >> j; cout << qry(1, 1, n, i, j) << "\n"; }
    }
    return 0;
}

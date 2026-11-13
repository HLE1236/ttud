#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<bool> has(1000001, false);
    for (int i = 0; i < n; ++i) { int x; cin >> x; has[x] = true; }
    int ans = 0;
    for (int i = 0; i < m; ++i) { int y; cin >> y; if (has[y]) { ans++; has[y] = false; } }
    cout << ans << "\n";
    return 0;
}

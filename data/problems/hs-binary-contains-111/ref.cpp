#include <bits/stdc++.h>
using namespace std;
int n; string s;
void dfs(int i, int c1, bool has) {
    if (!has && c1 + (n - i) < 3) return;
    if (i == n) { cout << s << "\n"; return; }
    s[i] = '0'; dfs(i + 1, 0, has);
    s[i] = '1'; dfs(i + 1, c1 + 1, has || (c1 + 1 >= 3));
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    if (!(cin >> n)) return 0;
    s.assign(n, '0'); dfs(0, 0, false);
    return 0;
}

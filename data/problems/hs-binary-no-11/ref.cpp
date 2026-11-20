#include <bits/stdc++.h>
using namespace std;
int n; string s;
void dfs(int i) {
    if (i == n) { cout << s << "\n"; return; }
    s[i] = '0'; dfs(i + 1);
    if (i == 0 || s[i - 1] == '0') { s[i] = '1'; dfs(i + 1); }
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n;
    s.assign(n, '0'); dfs(0);
    return 0;
}

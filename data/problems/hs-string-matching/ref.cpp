#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string p, t;
    if (!getline(cin, p)) return 0;
    getline(cin, t);
    if (!p.empty() && p.back() == '\r') p.pop_back();
    if (!t.empty() && t.back() == '\r') t.pop_back();
    int m = (int)p.size(), n = (int)t.size();
    if (m == 0 || n < m) { cout << 0 << '\n'; return 0; }
    vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && p[i] != p[j]) j = pi[j - 1];
        if (p[i] == p[j]) j++;
        pi[i] = j;
    }
    long long ans = 0;
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && t[i] != p[j]) j = pi[j - 1];
        if (t[i] == p[j]) j++;
        if (j == m) {
            ans++;
            j = pi[j - 1];
        }
    }
    cout << ans << '\n';
    return 0;
}

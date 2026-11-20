#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];
    if (m > n) { cout << 0 << '\n'; return 0; }
    vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && b[i] != b[j]) j = pi[j - 1];
        if (b[i] == b[j]) j++;
        pi[i] = j;
    }
    long long ans = 0;
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && a[i] != b[j]) j = pi[j - 1];
        if (a[i] == b[j]) j++;
        if (j == m) {
            ans++;
            j = pi[j - 1];
        }
    }
    cout << ans << '\n';
    return 0;
}

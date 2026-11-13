#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> a(n), b(m);
    for (int &x : a) cin >> x;
    for (int &y : b) cin >> y;
    if (n < m) { swap(a, b); swap(n, m); }
    vector<int> prev(m + 1, 0), cur(m + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i] == b[j - 1]) cur[j] = prev[j - 1] + 1;
            else cur[j] = max(prev[j], cur[j - 1]);
        }
        swap(prev, cur);
    }
    cout << prev[m] << "\n";
    return 0;
}

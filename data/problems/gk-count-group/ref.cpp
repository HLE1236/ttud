#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    int cnt[5] = {0};
    for (int i = 0; i < n; ++i) {
        int x; cin >> x;
        if (x >= 1 && x <= 4) cnt[x]++;
    }
    int t; cin >> t;
    cout << cnt[t] << "\n";
    return 0;
}

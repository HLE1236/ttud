#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long c;
    cin >> n >> c;
    long long x, ans = 0;
    for (int i = 0; i < n; ++i) {
        cin >> x;
        if (x > c) ++ans;
    }
    cout << ans << "\n";
    return 0;
}

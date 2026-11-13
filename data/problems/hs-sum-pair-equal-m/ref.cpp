#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long M; if (!(cin >> n >> M)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    int l = 0, r = n - 1; long long ans = 0;
    while (l < r) {
        long long s = a[l] + a[r];
        if (s == M) { ans++; l++; r--; }
        else if (s < M) l++; else r--;
    }
    cout << ans << "\n";
    return 0;
}

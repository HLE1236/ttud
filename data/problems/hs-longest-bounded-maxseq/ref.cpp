#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; long long Q; cin >> n >> Q;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    long long sum = 0; int L = 0, best = 0;
    for (int R = 0; R < n; ++R) {
        sum += a[R];
        while (sum > Q && L <= R) sum -= a[L++];
        if (sum <= Q) best = max(best, R - L + 1);
    }
    cout << (best == 0 ? -1 : best) << "\n";
    return 0;
}

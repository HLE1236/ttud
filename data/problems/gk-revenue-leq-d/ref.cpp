#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long D;
    if (!(cin >> n >> D)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    long long ans = 0, sum = 0;
    int L = 0;
    for (int R = 0; R < n; R++) {
        sum += a[R];
        while (sum > D) {
            sum -= a[L++];
        }
        ans += (R - L + 1);
    }
    cout << ans << "\n";
    return 0;
}

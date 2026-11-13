#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    long long max_sum = 0, cur = 0;
    for (int i = 0; i < n; i++) {
        long long a;
        cin >> a;
        if (a > 0) {
            cur += a;
            max_sum = max(max_sum, cur);
        } else {
            cur = 0;
        }
    }
    cout << max_sum << "\n";
    return 0;
}

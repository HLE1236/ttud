#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    long long prev = 0, x;
    int cur = 0, best = 0;
    for (int i = 0; i < n; ++i) {
        cin >> x;
        if (i == 0 || x <= prev) cur = 1;
        else cur++;
        best = max(best, cur);
        prev = x;
    }
    cout << best << "\n";
    return 0;
}

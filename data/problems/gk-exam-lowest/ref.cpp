#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    int mn = INT_MAX;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        mn = min(mn, x);
    }
    cout << mn << "\n";
    return 0;
}

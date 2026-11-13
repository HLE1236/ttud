#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, cnt = 0, x;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        if (x > 0) cnt++;
    }
    cout << cnt << "\n";
    return 0;
}

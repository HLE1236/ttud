#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, D;
    if (!(cin >> n >> D)) return 0;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if (x >= D) cnt++;
    }
    cout << cnt << "\n";
    return 0;
}

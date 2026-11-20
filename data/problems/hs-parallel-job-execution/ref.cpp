#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<long long> d(n);
    for (int i = 0; i < n; ++i) cin >> d[i];
    sort(d.rbegin(), d.rend());
    long long T = 0;
    for (int i = 0; i < n; ++i) T = max(T, i + d[i]);
    cout << T << "\n";
    return 0;
}

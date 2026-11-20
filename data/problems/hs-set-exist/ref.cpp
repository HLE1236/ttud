#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    unordered_set<int> seen;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        if (seen.find(x) != seen.end()) cout << 1 << '\n';
        else {
            cout << 0 << '\n';
            seen.insert(x);
        }
    }
    return 0;
}

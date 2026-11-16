#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long m;
    if (!(cin >> n >> m)) return 0;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        long long h = 0;
        for (unsigned char c : s) {
            h = (h * 256 + c) % m;
        }
        cout << h << '\n';
    }
    return 0;
}

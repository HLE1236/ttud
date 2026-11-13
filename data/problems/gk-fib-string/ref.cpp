#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s1, s2;
    long long k;
    cin >> s1 >> s2 >> k;
    vector<long long> len(100);
    len[1] = s1.size();
    len[2] = s2.size();
    int n = 2;
    while (len[n] <= k) {
        n++;
        len[n] = len[n - 1] + len[n - 2];
    }
    while (n > 2) {
        if (k < len[n - 1]) {
            n = n - 1;
        } else {
            k -= len[n - 1];
            n = n - 2;
        }
    }
    if (n == 1) cout << s1[k] << "\n";
    else cout << s2[k] << "\n";
    return 0;
}

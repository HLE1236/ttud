#include <bits/stdc++.h>
using namespace std;
int main() {
    unsigned long long a, b; cin >> a >> b;
    const unsigned long long MOD = 1000000007ULL;
    a %= MOD;
    unsigned long long res = 1;
    while (b > 0) {
        if (b & 1ULL) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    cout << res << "\n";
    return 0;
}

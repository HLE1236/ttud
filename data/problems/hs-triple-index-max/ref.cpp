#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    int mx = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        mx = max(mx, a[i]);
    }
    const long long NEG = -4e18;
    vector<long long> bestL(mx + 1, NEG), bestR(mx + 1, NEG);
    vector<char> seenL(mx + 1, 0), seenR(mx + 1, 0);
    vector<long long> L(n, NEG), R(n, NEG);

    for (int i = 0; i < n; i++) {
        L[i] = bestL[a[i]];
        int x = a[i];
        if (!seenL[x]) {
            seenL[x] = 1;
            long long x2 = 1LL * x * x;
            for (int v = 0; v <= mx; v++) {
                bestL[v] = max(bestL[v], x2 - 1LL * x * v);
            }
        }
    }
    for (int k = n - 1; k >= 0; k--) {
        R[k] = bestR[a[k]];
        int x = a[k];
        if (!seenR[x]) {
            seenR[x] = 1;
            long long x2 = 1LL * x * x;
            for (int v = 0; v <= mx; v++) {
                bestR[v] = max(bestR[v], x2 - 1LL * x * v);
            }
        }
    }
    long long ans = NEG;
    for (int j = 1; j <= n - 2; j++) {
        ans = max(ans, L[j] + R[j]);
    }
    cout << ans << '\n';
    return 0;
}

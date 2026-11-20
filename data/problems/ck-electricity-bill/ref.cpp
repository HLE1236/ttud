#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int K, N;
    cin >> K >> N;
    long long price[24] = {0};
    for (int i = 0; i < K; ++i) {
        int s, e; long long p;
        cin >> s >> e >> p;
        for (int h = max(0, s); h < min(24, e); ++h) price[h] = p;
    }
    long long ans = 0;
    for (int i = 0; i < N; ++i) {
        for (int h = 0; h < 24; ++h) {
            long long kwh; cin >> kwh;
            ans += kwh * price[h];
        }
    }
    cout << ans << "\n";
    return 0;
}

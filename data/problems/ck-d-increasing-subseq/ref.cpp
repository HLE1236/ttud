#include <bits/stdc++.h>
using namespace std;
static const int MAXV = 1000000;
int bit[MAXV + 5];
void upd(int i, int val) {
    for (; i <= MAXV; i += i & -i) bit[i] = max(bit[i], val);
}
int qry(int i) {
    int r = 0;
    for (; i > 0; i -= i & -i) r = max(r, bit[i]);
    return r;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, D;
    if (!(cin >> n >> D)) return 0;
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        int x; cin >> x;
        int best_prev = (x > D ? qry(x - D) : 0);
        int cur = best_prev + 1;
        ans = max(ans, cur);
        upd(x, cur);
    }
    cout << ans << "\n";
    return 0;
}

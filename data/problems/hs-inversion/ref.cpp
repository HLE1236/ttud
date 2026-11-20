#include <bits/stdc++.h>
using namespace std;
static const long long MOD = 1000000007LL;
long long inv_cnt = 0;
void msort(vector<int>& a, vector<int>& tmp, int l, int r) {
    if (l >= r) return;
    int m = (l + r) >> 1;
    msort(a, tmp, l, m); msort(a, tmp, m + 1, r);
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else { inv_cnt = (inv_cnt + (m - i + 1)) % MOD; tmp[k++] = a[j++]; }
    }
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int p = l; p <= r; ++p) a[p] = tmp[p];
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n), tmp(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    msort(a, tmp, 0, n - 1);
    cout << inv_cnt << "\n";
    return 0;
}

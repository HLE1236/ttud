#include <bits/stdc++.h>
using namespace std;

int rowM[9], colM[9], boxM[9];
vector<pair<int, int>> empties;
long long ans = 0;

void Try(int done) {
    if (done == (int)empties.size()) {
        ans++;
        return;
    }
    int bestIdx = done, bestCnt = 10, bestMask = 0;
    for (int i = done; i < (int)empties.size(); i++) {
        int r = empties[i].first, c = empties[i].second;
        int b = (r / 3) * 3 + (c / 3);
        int mask = (~(rowM[r] | colM[c] | boxM[b])) & 0x3FE;
        int cnt = __builtin_popcount(mask);
        if (cnt < bestCnt) {
            bestCnt = cnt;
            bestIdx = i;
            bestMask = mask;
            if (cnt <= 1) break;
        }
    }
    if (bestCnt == 0) return;
    swap(empties[done], empties[bestIdx]);
    int r = empties[done].first, c = empties[done].second;
    int b = (r / 3) * 3 + (c / 3);
    int m = bestMask;
    while (m) {
        int bit = m & -m;
        m -= bit;
        rowM[r] |= bit; colM[c] |= bit; boxM[b] |= bit;
        Try(done + 1);
        rowM[r] ^= bit; colM[c] ^= bit; boxM[b] ^= bit;
    }
    swap(empties[done], empties[bestIdx]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int v;
            if (!(cin >> v)) return 0;
            if (v == 0) empties.push_back({r, c});
            else {
                int bit = 1 << v;
                int b = (r / 3) * 3 + (c / 3);
                if ((rowM[r] & bit) || (colM[c] & bit) || (boxM[b] & bit)) {
                    cout << 0 << '\n';
                    return 0;
                }
                rowM[r] |= bit; colM[c] |= bit; boxM[b] |= bit;
            }
        }
    }
    Try(0);
    cout << ans << '\n';
    return 0;
}

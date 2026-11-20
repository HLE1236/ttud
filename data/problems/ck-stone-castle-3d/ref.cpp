#include <bits/stdc++.h>
using namespace std;
static int dp[101][101][101];
inline int getdp(int x, int y, int z) {
    if (x > y) swap(x, y);
    if (y > z) swap(y, z);
    if (x > y) swap(x, y);
    return dp[x][y][z];
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int L, W, H, N;
        cin >> L >> W >> H >> N;
        int dims[3] = {L, W, H};
        sort(dims, dims + 3);
        int A = dims[0], B = dims[1], C = dims[2];
        for (int a = 1; a <= C; ++a)
            for (int b = a; b <= C; ++b)
                for (int c = b; c <= C; ++c)
                    dp[a][b][c] = a * b * c;

        for (int i = 0; i < N; ++i) {
            int d[3]; cin >> d[0] >> d[1] >> d[2];
            sort(d, d + 3);
            if (d[2] <= C) dp[d[0]][d[1]][d[2]] = 0;
        }

        for (int a = 1; a <= C; ++a) {
            for (int b = a; b <= C; ++b) {
                for (int c = b; c <= C; ++c) {
                    int best = dp[a][b][c];
                    if (best == 0) continue;
                    for (int x = 1; x <= a / 2; ++x)
                        best = min(best, getdp(x, b, c) + getdp(a - x, b, c));
                    for (int y = 1; y <= b / 2; ++y)
                        best = min(best, getdp(a, y, c) + getdp(a, b - y, c));
                    for (int z = 1; z <= c / 2; ++z)
                        best = min(best, getdp(a, b, z) + getdp(a, b, c - z));
                    dp[a][b][c] = best;
                }
            }
        }
        cout << dp[A][B][C] << "\n";
    }
    return 0;
}

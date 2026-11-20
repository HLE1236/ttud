#include <bits/stdc++.h>
using namespace std;
int M, N, xs, ys, xt, yt;
int w[20][20];
bool vis[20][20];
long long best = 0, total_pos = 0;
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

void dfs(int x, int y, long long cur, long long rem) {
    if (cur + rem <= best) return;
    if (x == xt && y == yt) {
        best = max(best, cur);
        return;
    }
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx >= 1 && nx <= M && ny >= 1 && ny <= N && !vis[nx][ny] && w[nx][ny] > 0) {
            vis[nx][ny] = true;
            dfs(nx, ny, cur + w[nx][ny], rem - w[nx][ny]);
            vis[nx][ny] = false;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> M >> N >> xs >> ys >> xt >> yt;
    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j) {
            cin >> w[i][j];
            if (w[i][j] > 0) total_pos += w[i][j];
        }
    if (w[xs][ys] > 0 && w[xt][yt] > 0) {
        vis[xs][ys] = true;
        dfs(xs, ys, w[xs][ys], total_pos - w[xs][ys]);
    }
    cout << best << "\n";
    return 0;
}

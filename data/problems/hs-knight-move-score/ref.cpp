#include <bits/stdc++.h>
using namespace std;

int N, sr, sc;
long long G[12][12];
bool vis[12][12];
long long best = -4e18;
int dr[8] = {-2,-2,-1,-1,1,1,2,2};
int dc[8] = {-1,1,-2,2,-2,2,-1,1};

void dfs(int r, int c, long long sum, long long posRem) {
    if (sum > best) best = sum;
    if (sum + posRem <= best) return;
    for (int k = 0; k < 8; k++) {
        int nr = r + dr[k], nc = c + dc[k];
        if (nr >= 1 && nr <= N && nc >= 1 && nc <= N && !vis[nr][nc]) {
            vis[nr][nc] = true;
            long long d = G[nr][nc] > 0 ? G[nr][nc] : 0;
            dfs(nr, nc, sum + G[nr][nc], posRem - d);
            vis[nr][nc] = false;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> N >> sr >> sc)) return 0;
    long long totalPos = 0;
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++) {
            cin >> G[i][j];
            if (G[i][j] > 0) totalPos += G[i][j];
        }
    vis[sr][sc] = true;
    dfs(sr, sc, G[sr][sc], totalPos - (G[sr][sc] > 0 ? G[sr][sc] : 0));
    cout << best << '\n';
    return 0;
}

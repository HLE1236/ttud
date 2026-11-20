#include <bits/stdc++.h>
using namespace std;
int H, W, n;
pair<int, int> rect[12];
bool grid[12][12];
bool found = false;

bool can_place(int r, int c, int h, int w) {
    if (r + h > H || c + w > W) return false;
    for (int i = r; i < r + h; ++i)
        for (int j = c; j < c + w; ++j)
            if (grid[i][j]) return false;
    return true;
}
void set_place(int r, int c, int h, int w, bool val) {
    for (int i = r; i < r + h; ++i)
        for (int j = c; j < c + w; ++j) grid[i][j] = val;
}
void dfs(int k) {
    if (found) return;
    if (k == n) { found = true; return; }
    for (int rot = 0; rot < 2; ++rot) {
        int h = rot ? rect[k].second : rect[k].first;
        int w = rot ? rect[k].first : rect[k].second;
        if (rot == 1 && h == w) continue;
        for (int r = 0; r + h <= H; ++r) {
            for (int c = 0; c + w <= W; ++c) {
                if (can_place(r, c, h, w)) {
                    set_place(r, c, h, w, true);
                    dfs(k + 1);
                    set_place(r, c, h, w, false);
                    if (found) return;
                }
            }
        }
    }
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> H >> W >> n;
    int area = 0;
    for (int i = 0; i < n; ++i) {
        cin >> rect[i].first >> rect[i].second;
        area += rect[i].first * rect[i].second;
    }
    if (area <= H * W) {
        sort(rect, rect + n, [](auto a, auto b){ return a.first * a.second > b.first * b.second; });
        dfs(0);
    }
    cout << (found ? 1 : 0) << "\n";
    return 0;
}

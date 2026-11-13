#include <bits/stdc++.h>
using namespace std;
struct Pt { long long x, y; };
long long best_d2 = (1LL << 62);
vector<Pt> p, tmp;

void upd(const Pt& a, const Pt& b) {
    long long dx = a.x - b.x, dy = a.y - b.y;
    long long d2 = dx * dx + dy * dy;
    if (d2 < best_d2) best_d2 = d2;
}

void solve(int l, int r) {
    if (r - l <= 3) {
        for (int i = l; i < r; ++i)
            for (int j = i + 1; j < r; ++j) upd(p[i], p[j]);
        sort(p.begin() + l, p.begin() + r, [](const Pt& a, const Pt& b){ return a.y < b.y; });
        return;
    }
    int mid = (l + r) / 2;
    long long mid_x = p[mid].x;
    solve(l, mid);
    solve(mid, r);
    merge(p.begin() + l, p.begin() + mid, p.begin() + mid, p.begin() + r, tmp.begin(),
          [](const Pt& a, const Pt& b){ return a.y < b.y; });
    copy(tmp.begin(), tmp.begin() + (r - l), p.begin() + l);
    vector<Pt> strip;
    for (int i = l; i < r; ++i) {
        long long dx = p[i].x - mid_x;
        if (dx * dx < best_d2) strip.push_back(p[i]);
    }
    for (int i = 0; i < (int)strip.size(); ++i) {
        for (int j = i + 1; j < (int)strip.size(); ++j) {
            long long dy = strip[j].y - strip[i].y;
            if (dy * dy >= best_d2) break;
            upd(strip[i], strip[j]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    p.resize(n);
    tmp.resize(n);
    for (int i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;
    sort(p.begin(), p.end(), [](const Pt& a, const Pt& b){
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    solve(0, n);
    cout << fixed << setprecision(6) << sqrt((double)best_d2) << "\n";
    return 0;
}

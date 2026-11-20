#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    mt19937 rng(1337);
    set<uint64_t> seen;

    vector<int> p(n + 1), d1(2 * n + 2), d2(2 * n + 2);
    auto add = [&](int c, int v) { d1[c - p[c] + n] += v; d2[c + p[c]] += v; };
    auto conf = [&](int c) { return d1[c - p[c] + n] + d2[c + p[c]] - 2; };

    while ((int)seen.size() < k) {
        iota(p.begin() + 1, p.end(), 1);
        shuffle(p.begin() + 1, p.end(), rng);
        fill(d1.begin(), d1.end(), 0);
        fill(d2.begin(), d2.end(), 0);
        for (int i = 1; i <= n; i++) add(i, 1);

        vector<int> bad;
        for (int step = 0; step < 25 * n; step++) {
            if (step % 40 == 0 || bad.empty()) {
                bad.clear();
                for (int i = 1; i <= n; i++) if (conf(i) > 0) bad.push_back(i);
                if (bad.empty()) break;
            }
            int a = bad[rng() % bad.size()];
            if (!conf(a)) continue;
            int best_b = -1, best_gain = -1e9;
            for (int t = 0; t < min(n, 30); t++) {
                int b = rng() % n + 1;
                if (a == b) continue;
                int before = conf(a) + conf(b);
                add(a, -1); add(b, -1); swap(p[a], p[b]); add(a, 1); add(b, 1);
                int gain = before - (conf(a) + conf(b));
                add(a, -1); add(b, -1); swap(p[a], p[b]); add(a, 1); add(b, 1);
                if (gain > best_gain) best_gain = gain, best_b = b;
            }
            if (best_b != -1 && (best_gain >= 0 || rng() % 5 == 0)) {
                add(a, -1); add(best_b, -1); swap(p[a], p[best_b]); add(a, 1); add(best_b, 1);
                bad.clear();
            }
        }
        bool ok = true;
        uint64_t h = 1469598103934665603ULL;
        for (int i = 1; i <= n; i++) {
            if (conf(i) > 0) ok = false;
            h = (h ^ p[i]) * 1099511628211ULL;
        }
        if (ok && seen.insert(h).second) {
            cout << n << '\n';
            for (int i = 1; i <= n; i++) cout << p[i] << (i == n ? '\n' : ' ');
        }
    }
    return 0;
}

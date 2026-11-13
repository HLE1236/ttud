#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    mt19937_64 rng(421337);
    set<uint64_t> seen;
    int found = 0;

    auto solve_one = [&]() {
        vector<int> p(n + 1), d1(2 * n + 5, 0), d2(2 * n + 5, 0);
        while (true) {
            for (int i = 1; i <= n; ++i) p[i] = i;
            shuffle(p.begin() + 1, p.end(), rng);
            fill(d1.begin(), d1.end(), 0);
            fill(d2.begin(), d2.end(), 0);
            for (int i = 1; i <= n; ++i) {
                d1[i - p[i] + n]++;
                d2[i + p[i]]++;
            }
            auto conf_at = [&](int c) {
                return (d1[c - p[c] + n] - 1) + (d2[c + p[c]] - 1);
            };
            auto delta_swap = [&](int a, int b) {
                if (a == b) return 0;
                int old_v = (d1[a - p[a] + n] - 1) + (d2[a + p[a]] - 1)
                          + (d1[b - p[b] + n] - 1) + (d2[b + p[b]] - 1);
                // fast update simulation
                d1[a - p[a] + n]--; d2[a + p[a]]--;
                d1[b - p[b] + n]--; d2[b + p[b]]--;
                int na = p[b], nb = p[a];
                int new_v = d1[a - na + n] + d2[a + na] + d1[b - nb + n] + d2[b + nb];
                if (a - na == b - nb) new_v += 1;
                if (a + na == b + nb) new_v += 1;
                d1[a - p[a] + n]++; d2[a + p[a]]++;
                d1[b - p[b] + n]++; d2[b + p[b]]++;
                return new_v - old_v;
            };
            auto do_swap = [&](int a, int b) {
                d1[a - p[a] + n]--; d2[a + p[a]]--;
                d1[b - p[b] + n]--; d2[b + p[b]]--;
                swap(p[a], p[b]);
                d1[a - p[a] + n]++; d2[a + p[a]]++;
                d1[b - p[b] + n]++; d2[b + p[b]]++;
            };
            vector<int> bad;
            for (int steps = 0; steps < 25 * n; ++steps) {
                if (steps % 50 == 0 || bad.empty()) {
                    bad.clear();
                    for (int i = 1; i <= n; ++i)
                        if (conf_at(i) > 0) bad.push_back(i);
                    if (bad.empty()) break;
                }
                int a = bad[rng() % bad.size()];
                if (conf_at(a) == 0) { bad.clear(); continue; }
                int best_b = -1, best_d = 1e9;
                int trials = min(n, 30);
                for (int t = 0; t < trials; ++t) {
                    int b = (int)(rng() % n) + 1;
                    if (b == a) continue;
                    int d = delta_swap(a, b);
                    if (d < best_d) { best_d = d; best_b = b; }
                }
                if (best_b != -1 && (best_d <= 0 || (rng() % 5 == 0))) {
                    do_swap(a, best_b);
                    bad.clear();
                }
            }
            bool ok = true;
            for (int i = 1; i <= n; ++i) if (conf_at(i) > 0) { ok = false; break; }
            if (ok) return p;
        }
    };

    while (found < k) {
        vector<int> sol = solve_one();
        uint64_t h = 1469598103934665603ULL;
        for (int i = 1; i <= n; ++i) {
            h ^= (uint64_t)sol[i];
            h *= 1099511628211ULL;
        }
        if (seen.count(h)) continue;
        seen.insert(h);
        found++;
        cout << n << "\n";
        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << ' ';
            cout << sol[i];
        }
        cout << "\n";
    }
    return 0;
}

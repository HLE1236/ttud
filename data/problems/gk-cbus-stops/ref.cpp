#include <bits/stdc++.h>
using namespace std;
int N, cap, cmin = 1e9;
int A[30][30], stopAllowed[15], x[30], idx[30], min_out[30];
bool appear[30];
int load = 0, f = 0, f_best = 1e9, rem_lb = 0;

void Try(int k) {
    // Forced drop-off check: any onboard passenger whose deadline == k must be dropped now
    int forced = -1;
    for (int i = 1; i <= N; ++i) {
        if (appear[i] && !appear[i + N]) {
            int ddl = idx[i] + stopAllowed[i] + 1;
            if (ddl < k) return;
            if (ddl == k) {
                if (forced != -1) return; // two deadlines at the same step -> impossible
                forced = i + N;
            }
        }
    }

    int lo = (forced != -1 ? forced : 1);
    int hi = (forced != -1 ? forced : 2 * N);
    for (int v = lo; v <= hi; ++v) {
        if (appear[v]) continue;
        if (v <= N) {
            if (load + 1 > cap) continue;
        } else {
            if (!appear[v - N]) continue;
        }
        int cost = A[x[k - 1]][v];
        int next_rem_lb = rem_lb - min_out[v];
        if (k == 2 * N) {
            f_best = min(f_best, f + cost + A[v][0]);
            continue;
        }
        if (f + cost + next_rem_lb >= f_best) continue;

        x[k] = v;
        idx[v] = k;
        f += cost;
        load += (v <= N ? 1 : -1);
        appear[v] = true;
        rem_lb = next_rem_lb;

        Try(k + 1);

        rem_lb += min_out[v];
        appear[v] = false;
        load -= (v <= N ? 1 : -1);
        f -= cost;
        idx[v] = -1;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N >> cap;
    for (int i = 0; i <= 2 * N; i++) {
        min_out[i] = 1e9;
        for (int j = 0; j <= 2 * N; j++) {
            cin >> A[i][j];
            if (i != j) {
                cmin = min(cmin, A[i][j]);
                min_out[i] = min(min_out[i], A[i][j]);
            }
        }
    }
    for (int i = 1; i <= N; i++) cin >> stopAllowed[i];
    // rem_lb = sum of min incoming/outgoing edge to finish from unvisited vertices + return to 0
    rem_lb = min_out[0];
    for (int i = 1; i <= 2 * N; ++i) rem_lb += min_out[i];
    // Actually from x[k-1] we pay A[x[k-1]][v], plus 2N-k edges from {v} union unvisited back to 0.
    // Using min_out[u] (min cost LEAVING u) for u in {0..2N} minus min_out[0] before start:
    // Sum of min_out for all visited-to-be nodes (1..2N) works because each node leaves once!
    rem_lb = 0;
    for (int i = 1; i <= 2 * N; ++i) rem_lb += min_out[i];

    x[0] = 0; idx[0] = 0;
    Try(1);
    cout << (f_best == 1e9 ? -1 : f_best) << "\n";
    return 0;
}

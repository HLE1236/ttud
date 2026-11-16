#include <bits/stdc++.h>
using namespace std;

int n;
int p[12];
bool used[12];

void Try(int k) {
    if (k == n) {
        for (int i = 0; i < n; i++) {
            if (i) cout << ' ';
            cout << p[i];
        }
        cout << '\n';
        return;
    }
    for (int v = 1; v <= n; v++) {
        if (!used[v]) {
            used[v] = true;
            p[k] = v;
            Try(k + 1);
            used[v] = false;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> n)) return 0;
    Try(0);
    return 0;
}

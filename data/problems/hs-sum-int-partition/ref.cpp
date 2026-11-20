#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> cur;

void Try(int rem, int minVal) {
    if (rem == 0) {
        for (int i = 0; i < cur.size(); i++) {
            if (i) cout << ' ';
            cout << cur[i];
        }
        cout << '\n';
        return;
    }
    for (int v = minVal; v <= rem; v++) {
        cur.push_back(v);
        Try(rem - v, v);
        cur.pop_back();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N;
    Try(N, 1);
    return 0;
}

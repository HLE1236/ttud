#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    priority_queue<int> pq;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        pq.push(x);
    }
    string cmd;
    while (cin >> cmd && cmd != "*") {
        if (cmd == "insert") {
            int x; cin >> x;
            pq.push(x);
        } else if (cmd == "delete-max") {
            if (!pq.empty()) {
                cout << pq.top() << "\n";
                pq.pop();
            }
        }
    }
    return 0;
}

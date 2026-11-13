#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    priority_queue<int, vector<int>, greater<int>> pq;
    string cmd;
    while (cin >> cmd && cmd != "#") {
        if (cmd == "PUSH") { int e; cin >> e; pq.push(e); }
        else if (cmd == "POP" && !pq.empty()) {
            cout << pq.top() << "\n"; pq.pop();
        }
    }
    return 0;
}

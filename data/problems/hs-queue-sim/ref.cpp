#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    queue<int> q; string cmd;
    while (cin >> cmd && cmd != "#") {
        if (cmd == "PUSH") { int v; cin >> v; q.push(v); }
        else if (cmd == "POP") {
            if (q.empty()) cout << "NULL\n";
            else { cout << q.front() << "\n"; q.pop(); }
        }
    }
    return 0;
}

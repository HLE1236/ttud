#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    stack<int> st; string cmd;
    while (cin >> cmd && cmd != "#") {
        if (cmd == "PUSH") { int v; cin >> v; st.push(v); }
        else if (cmd == "POP") {
            if (st.empty()) cout << "NULL\n";
            else { cout << st.top() << "\n"; st.pop(); }
        }
    }
    return 0;
}

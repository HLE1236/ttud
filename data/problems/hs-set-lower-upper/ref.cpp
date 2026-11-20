#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    set<int> s;
    for (int i = 0; i < n; ++i) { int x; cin >> x; s.insert(x); }
    string cmd;
    while (cin >> cmd && cmd != "#") {
        int v; cin >> v;
        if (cmd == "min_greater_equal") {
            auto it = s.lower_bound(v);
            if (it == s.end()) cout << "NULL\n"; else cout << *it << "\n";
        } else if (cmd == "min_greater") {
            auto it = s.upper_bound(v);
            if (it == s.end()) cout << "NULL\n"; else cout << *it << "\n";
        } else if (cmd == "insert") s.insert(v);
        else if (cmd == "remove") s.erase(v);
    }
    return 0;
}

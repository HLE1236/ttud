#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    unordered_set<string> db;
    string s;
    while (cin >> s && s != "*") {
        db.insert(s);
    }
    string cmd, k;
    while (cin >> cmd && cmd != "***") {
        cin >> k;
        if (cmd == "find") {
            cout << (db.count(k) ? 1 : 0) << '\n';
        } else {
            cout << (db.insert(k).second ? 1 : 0) << '\n';
        }
    }
    return 0;
}

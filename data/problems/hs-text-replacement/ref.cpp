#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string p1, p2, t;
    if (!getline(cin, p1)) return 0;
    getline(cin, p2);
    getline(cin, t);
    auto strip_cr = [](string& s) {
        if (!s.empty() && s.back() == '\r') s.pop_back();
    };
    strip_cr(p1); strip_cr(p2); strip_cr(t);
    if (p1.empty()) {
        cout << t << '\n';
        return 0;
    }
    string res;
    size_t pos = 0;
    while (true) {
        size_t found = t.find(p1, pos);
        if (found == string::npos) {
            res.append(t, pos, string::npos);
            break;
        }
        res.append(t, pos, found - pos);
        res.append(p2);
        pos = found + p1.size();
    }
    cout << res << '\n';
    return 0;
}

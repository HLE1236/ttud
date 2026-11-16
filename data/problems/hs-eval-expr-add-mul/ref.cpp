#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    getline(cin, s);
    if (!s.empty() && s.back() == '\r') s.pop_back();

    struct Tok { int type; long long val; char op; }; // type 0: num, 1: op
    vector<Tok> toks;
    int n = (int)s.size();
    for (int i = 0; i < n; ) {
        if (s[i] == ' ' || s[i] == '\t') { i++; continue; }
        if (isdigit((unsigned char)s[i])) {
            long long v = 0;
            while (i < n && isdigit((unsigned char)s[i])) {
                v = (v * 10 + (s[i] - '0')) % MOD;
                i++;
            }
            toks.push_back({0, v, 0});
        } else if (s[i] == '+' || s[i] == '*') {
            toks.push_back({1, 0, s[i]});
            i++;
        } else {
            cout << "NOT_CORRECT\n";
            return 0;
        }
    }

    if (toks.empty() || toks.size() % 2 == 0) {
        cout << "NOT_CORRECT\n";
        return 0;
    }
    for (size_t i = 0; i < toks.size(); i++) {
        if ((i % 2 == 0 && toks[i].type != 0) || (i % 2 == 1 && toks[i].type != 1)) {
            cout << "NOT_CORRECT\n";
            return 0;
        }
    }

    long long ans = 0;
    long long term = toks[0].val;
    for (size_t i = 1; i < toks.size(); i += 2) {
        if (toks[i].op == '*') {
            term = (term * toks[i + 1].val) % MOD;
        } else {
            ans = (ans + term) % MOD;
            term = toks[i + 1].val;
        }
    }
    ans = (ans + term) % MOD;
    cout << ans << '\n';
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

struct Tok {
    int type; // 0: num, 1: +, 2: *, 3: (, 4: )
    long long val;
};

vector<Tok> toks;
size_t ptr = 0;
bool ok = true;

long long parseExpr();

long long parseFactor() {
    if (!ok || ptr >= toks.size()) { ok = false; return 0; }
    if (toks[ptr].type == 0) {
        return toks[ptr++].val;
    }
    if (toks[ptr].type == 3) {
        ptr++;
        long long v = parseExpr();
        if (!ok || ptr >= toks.size() || toks[ptr].type != 4) {
            ok = false;
            return 0;
        }
        ptr++;
        return v;
    }
    ok = false;
    return 0;
}

long long parseTerm() {
    long long res = parseFactor();
    while (ok && ptr < toks.size() && toks[ptr].type == 2) {
        ptr++;
        long long rhs = parseFactor();
        res = (res * rhs) % MOD;
    }
    return res;
}

long long parseExpr() {
    long long res = parseTerm();
    while (ok && ptr < toks.size() && toks[ptr].type == 1) {
        ptr++;
        long long rhs = parseTerm();
        res = (res + rhs) % MOD;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    getline(cin, s);
    if (!s.empty() && s.back() == '\r') s.pop_back();
    int n = (int)s.size();
    for (int i = 0; i < n; ) {
        if (s[i] == ' ' || s[i] == '\t') { i++; continue; }
        if (isdigit((unsigned char)s[i])) {
            long long v = 0;
            while (i < n && isdigit((unsigned char)s[i])) {
                v = (v * 10 + (s[i] - '0')) % MOD;
                i++;
            }
            toks.push_back({0, v});
        } else if (s[i] == '+') { toks.push_back({1, 0}); i++; }
        else if (s[i] == '*') { toks.push_back({2, 0}); i++; }
        else if (s[i] == '(') { toks.push_back({3, 0}); i++; }
        else if (s[i] == ')') { toks.push_back({4, 0}); i++; }
        else { cout << "NOT_CORRECT\n"; return 0; }
    }
    if (toks.empty()) { cout << "NOT_CORRECT\n"; return 0; }
    long long ans = parseExpr();
    if (!ok || ptr != toks.size()) cout << "NOT_CORRECT\n";
    else cout << ans << '\n';
    return 0;
}

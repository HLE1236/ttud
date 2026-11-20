#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007;
string s;
int pos = 0, n;
bool ok = true;

void skip_spaces() {
    while (pos < n && s[pos] == ' ') pos++;
}

long long parse_expr();

long long parse_factor() {
    skip_spaces();
    if (!ok || pos >= n) {
        ok = false;
        return 0;
    }
    if (s[pos] == '(') {
        pos++;
        long long val = parse_expr();
        skip_spaces();
        if (pos >= n || s[pos] != ')') {
            ok = false;
            return 0;
        }
        pos++;
        return val;
    }
    if (!isdigit(s[pos])) {
        ok = false;
        return 0;
    }
    long long val = 0;
    while (pos < n && isdigit(s[pos])) {
        val = (val * 10 + (s[pos] - '0')) % MOD;
        pos++;
    }
    return val;
}

long long parse_term() {
    long long val = parse_factor();
    while (ok) {
        skip_spaces();
        if (pos < n && s[pos] == '*') {
            pos++;
            val = (val * parse_factor()) % MOD;
        } else {
            break;
        }
    }
    return val;
}

long long parse_expr() {
    long long val = parse_term();
    while (ok) {
        skip_spaces();
        if (pos < n && s[pos] == '+') {
            pos++;
            val = (val + parse_term()) % MOD;
        } else {
            break;
        }
    }
    return val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    getline(cin, s);
    if (!s.empty() && s.back() == '\r') s.pop_back();
    n = (int)s.size();

    long long ans = parse_expr();
    skip_spaces();

    if (!ok || pos != n) cout << "NOT_CORRECT\n";
    else cout << ans << '\n';
    return 0;
}

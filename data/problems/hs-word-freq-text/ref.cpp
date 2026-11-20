#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    map<string, int> mp;
    string cur; char ch;
    while (cin.get(ch)) {
        if (isalnum(ch)) cur.push_back(ch);
        else if (!cur.empty()) { mp[cur]++; cur.clear(); }
    }
    if (!cur.empty()) mp[cur]++;
    for (auto &[w, c] : mp) cout << w << " " << c << "\n";
    return 0;
}

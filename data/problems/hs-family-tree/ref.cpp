#include <bits/stdc++.h>
using namespace std;

unordered_map<string, vector<string>> ch;
unordered_map<string, int> memoDesc, memoGen;

int getDesc(const string& u) {
    auto it = memoDesc.find(u);
    if (it != memoDesc.end()) return it->second;
    int res = 0;
    auto cit = ch.find(u);
    if (cit != ch.end()) {
        for (const string& v : cit->second) {
            res += 1 + getDesc(v);
        }
    }
    return memoDesc[u] = res;
}

int getGen(const string& u) {
    auto it = memoGen.find(u);
    if (it != memoGen.end()) return it->second;
    int res = 0;
    auto cit = ch.find(u);
    if (cit != ch.end()) {
        for (const string& v : cit->second) {
            res = max(res, 1 + getGen(v));
        }
    }
    return memoGen[u] = res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string a, b;
    while (cin >> a && a != "***") {
        cin >> b;
        ch[b].push_back(a);
    }
    string cmd, name;
    while (cin >> cmd && cmd != "***") {
        cin >> name;
        if (cmd == "descendants") cout << getDesc(name) << '\n';
        else cout << getGen(name) << '\n';
    }
    return 0;
}

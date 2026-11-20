#include <bits/stdc++.h>
using namespace std;

int root = -1;
unordered_map<int, vector<int>> ch;
unordered_set<int> has;
vector<int> res;

void pre(int u) { res.push_back(u); for (int v : ch[u]) pre(v); }
void post(int u) { for (int v : ch[u]) post(v); res.push_back(u); }
void in(int u) {
    auto& c = ch[u];
    if (!c.empty()) in(c[0]);
    res.push_back(u);
    for (int i = 1; i < c.size(); i++) in(c[i]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string cmd;
    while (cin >> cmd && cmd != "*") {
        if (cmd == "MakeRoot") cin >> root, has.insert(root);
        else if (cmd == "Insert") {
            int u, v; cin >> u >> v;
            if (has.count(v) && !has.count(u)) ch[v].push_back(u), has.insert(u);
        } else {
            res.clear();
            if (root != -1) {
                if (cmd == "PreOrder") pre(root);
                else if (cmd == "InOrder") in(root);
                else post(root);
            }
            for (int i = 0; i < res.size(); i++)
                cout << res[i] << (i + 1 == res.size() ? "" : " ");
            cout << '\n';
        }
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int root = -1;
unordered_map<int, vector<int>> ch;
unordered_set<int> ex;

void preOrder(int u, vector<int>& out) {
    out.push_back(u);
    for (int v : ch[u]) preOrder(v, out);
}

void inOrder(int u, vector<int>& out) {
    auto& c = ch[u];
    if (!c.empty()) inOrder(c[0], out);
    out.push_back(u);
    for (size_t i = 1; i < c.size(); i++) inOrder(c[i], out);
}

void postOrder(int u, vector<int>& out) {
    for (int v : ch[u]) postOrder(v, out);
    out.push_back(u);
}

void printVec(const vector<int>& v) {
    for (size_t i = 0; i < v.size(); i++) {
        if (i) cout << ' ';
        cout << v[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string cmd;
    while (cin >> cmd && cmd != "*") {
        if (cmd == "MakeRoot") {
            cin >> root;
            ex.insert(root);
        } else if (cmd == "Insert") {
            int u, v; cin >> u >> v;
            if (ex.count(v) && !ex.count(u)) {
                ch[v].push_back(u);
                ex.insert(u);
            }
        } else if (cmd == "PreOrder") {
            vector<int> out;
            if (root != -1) preOrder(root, out);
            printVec(out);
        } else if (cmd == "InOrder") {
            vector<int> out;
            if (root != -1) inOrder(root, out);
            printVec(out);
        } else if (cmd == "PostOrder") {
            vector<int> out;
            if (root != -1) postOrder(root, out);
            printVec(out);
        }
    }
    return 0;
}

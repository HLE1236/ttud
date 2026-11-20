#include <bits/stdc++.h>
using namespace std;

struct Node { int k; Node *l = nullptr, *r = nullptr; };

Node* ins(Node* u, int k) {
    if (!u) return new Node{k};
    if (k < u->k) u->l = ins(u->l, k);
    else if (k > u->k) u->r = ins(u->r, k);
    return u;
}

void pre(Node* u, bool& first) {
    if (!u) return;
    cout << (first ? "" : " ") << u->k;
    first = false;
    pre(u->l, first);
    pre(u->r, first);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Node* root = nullptr;
    string cmd; int k;
    while (cin >> cmd && cmd != "#") cin >> k, root = ins(root, k);
    bool first = true;
    pre(root, first);
    cout << '\n';
    return 0;
}

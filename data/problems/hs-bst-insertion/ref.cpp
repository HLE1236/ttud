#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

Node* insertNode(Node* r, int k) {
    if (!r) return new Node(k);
    if (k < r->key) r->left = insertNode(r->left, k);
    else if (k > r->key) r->right = insertNode(r->right, k);
    return r;
}

vector<int> ans;
void preorder(Node* r) {
    if (!r) return;
    ans.push_back(r->key);
    preorder(r->left);
    preorder(r->right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Node* root = nullptr;
    string cmd;
    while (cin >> cmd && cmd != "#") {
        int k; cin >> k;
        root = insertNode(root, k);
    }
    preorder(root);
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}

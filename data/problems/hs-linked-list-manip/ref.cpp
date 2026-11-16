#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    Node* next;
    Node(int k) : key(k), next(nullptr) {}
};

Node* head = nullptr;

bool exists(int k) {
    for (Node* p = head; p; p = p->next)
        if (p->key == k) return true;
    return false;
}

void addLast(int k) {
    if (exists(k)) return;
    Node* nn = new Node(k);
    if (!head) { head = nn; return; }
    Node* p = head;
    while (p->next) p = p->next;
    p->next = nn;
}

void addFirst(int k) {
    if (exists(k)) return;
    Node* nn = new Node(k);
    nn->next = head;
    head = nn;
}

void addAfter(int u, int v) {
    if (exists(u)) return;
    for (Node* p = head; p; p = p->next) {
        if (p->key == v) {
            Node* nn = new Node(u);
            nn->next = p->next;
            p->next = nn;
            return;
        }
    }
}

void addBefore(int u, int v) {
    if (exists(u) || !head) return;
    if (head->key == v) {
        addFirst(u);
        return;
    }
    for (Node* p = head; p->next; p = p->next) {
        if (p->next->key == v) {
            Node* nn = new Node(u);
            nn->next = p->next;
            p->next = nn;
            return;
        }
    }
}

void removeKey(int k) {
    if (!head) return;
    if (head->key == k) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
        return;
    }
    for (Node* p = head; p->next; p = p->next) {
        if (p->next->key == k) {
            Node* tmp = p->next;
            p->next = tmp->next;
            delete tmp;
            return;
        }
    }
}

void reverseList() {
    Node *prev = nullptr, *cur = head;
    while (cur) {
        Node* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    head = prev;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    Node* tail = nullptr;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        Node* nn = new Node(x);
        if (!head) head = tail = nn;
        else { tail->next = nn; tail = nn; }
    }
    string cmd;
    while (cin >> cmd && cmd != "#") {
        if (cmd == "addlast") { int k; cin >> k; addLast(k); }
        else if (cmd == "addfirst") { int k; cin >> k; addFirst(k); }
        else if (cmd == "addafter") { int u, v; cin >> u >> v; addAfter(u, v); }
        else if (cmd == "addbefore") { int u, v; cin >> u >> v; addBefore(u, v); }
        else if (cmd == "remove") { int k; cin >> k; removeKey(k); }
        else if (cmd == "reverse") { reverseList(); }
    }
    bool first = true;
    for (Node* p = head; p; p = p->next) {
        if (!first) cout << ' ';
        cout << p->key;
        first = false;
    }
    cout << '\n';
    return 0;
}

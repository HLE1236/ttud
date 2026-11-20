#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a;
    auto has = [&](int k) { return find(a.begin(), a.end(), k) != a.end(); };
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        if (!has(x)) a.push_back(x);
    }

    string cmd;
    while (cin >> cmd && cmd != "#") {
        if (cmd == "addlast") {
            int k; cin >> k;
            if (!has(k)) a.push_back(k);
        } else if (cmd == "addfirst") {
            int k; cin >> k;
            if (!has(k)) a.insert(a.begin(), k);
        } else if (cmd == "addafter") {
            int u, v; cin >> u >> v;
            auto it = find(a.begin(), a.end(), v);
            if (!has(u) && it != a.end()) a.insert(it + 1, u);
        } else if (cmd == "addbefore") {
            int u, v; cin >> u >> v;
            auto it = find(a.begin(), a.end(), v);
            if (!has(u) && it != a.end()) a.insert(it, u);
        } else if (cmd == "remove") {
            int k; cin >> k;
            auto it = find(a.begin(), a.end(), k);
            if (it != a.end()) a.erase(it);
        } else if (cmd == "reverse") {
            reverse(a.begin(), a.end());
        }
    }

    for (int i = 0; i < (int)a.size(); i++)
        cout << a[i] << (i + 1 == (int)a.size() ? '\n' : ' ');
    return 0;
}

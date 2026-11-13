#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<pair<int,int>> a(n);
    for (int i = 0; i < n; i++) cin >> a[i].first >> a[i].second;
    sort(a.begin(), a.end());
    priority_queue<int, vector<int>, greater<int>> pq;
    for (auto &m : a) {
        if (!pq.empty() && pq.top() <= m.first) pq.pop();
        pq.push(m.second);
    }
    cout << pq.size() << "\n";
    return 0;
}

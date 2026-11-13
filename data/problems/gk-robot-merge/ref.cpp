#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    priority_queue<long long, vector<long long>, greater<long long>> pq;
    for (int i = 0; i < N; i++) {
        long long x;
        cin >> x;
        pq.push(x);
    }
    long long sum_cost = 0;
    while (pq.size() > 1) {
        long long x = pq.top(); pq.pop();
        long long y = pq.top(); pq.pop();
        long long s = x + y;
        sum_cost += s;
        pq.push(s);
    }
    // 5% of sum_cost = sum_cost * 5 / 100 = sum_cost / 20
    cout << (sum_cost / 20) << "\n";
    return 0;
}

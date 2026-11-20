#include <bits/stdc++.h>
using namespace std;

int toSec(const string& t) {
    return ((t[0] - '0') * 10 + t[1] - '0') * 3600
         + ((t[3] - '0') * 10 + t[4] - '0') * 60
         + ((t[6] - '0') * 10 + t[7] - '0');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, long long> day_id;
    string s; int idx = 0;
    while (cin >> s && s != "*") day_id[s] = idx++;

    auto toTS = [&](const string& d, const string& t) {
        return day_id[d] * 86400LL + toSec(t);
    };

    long long total_qty = 0;
    unordered_map<string, long long> cust_qty;
    unordered_map<string, pair<long long, long long>> span;
    map<long long, long long> time_qty;

    string trip, cust, dt, tm; long long qty;
    while (cin >> trip && trip != "***") {
        cin >> cust >> dt >> tm >> qty;
        long long ts = toTS(dt, tm);
        total_qty += qty;
        cust_qty[cust] += qty;
        time_qty[ts] += qty;
        if (!span.count(trip)) span[trip] = {ts, ts};
        else {
            span[trip].first = min(span[trip].first, ts);
            span[trip].second = max(span[trip].second, ts);
        }
    }

    int M = (int)time_qty.size(), K = __lg(max(1, M)) + 1;
    vector<long long> ts_list;
    vector<vector<long long>> st(K, vector<long long>(max(1, M)));
    int i = 0;
    for (auto [t, q] : time_qty) ts_list.push_back(t), st[0][i++] = q;
    for (int j = 1; j < K; j++)
        for (int l = 0; l + (1 << j) <= M; l++)
            st[j][l] = max(st[j - 1][l], st[j - 1][l + (1 << (j - 1))]);

    auto query_max = [&](int l, int r) -> long long {
        if (l > r) return 0;
        int j = __lg(r - l + 1);
        return max(st[j][l], st[j][r - (1 << j) + 1]);
    };

    string q;
    while (cin >> q && q != "***") {
        if (q == "TOTAL_QTY") cout << total_qty << '\n';
        else if (q == "QTY_CUSTOMER") { string c; cin >> c; cout << cust_qty[c] << '\n'; }
        else if (q == "TOTAL_TRIPS") cout << span.size() << '\n';
        else if (q == "TRAVEL_TIME_TRIP") {
            string tr; cin >> tr;
            cout << (span.count(tr) ? span[tr].second - span[tr].first : 0) << '\n';
        } else if (q == "QTY_MAX_PERIOD") {
            string d1, t1, d2, t2; cin >> d1 >> t1 >> d2 >> t2;
            int l = lower_bound(ts_list.begin(), ts_list.end(), toTS(d1, t1)) - ts_list.begin();
            int r = upper_bound(ts_list.begin(), ts_list.end(), toTS(d2, t2)) - ts_list.begin() - 1;
            cout << query_max(l, r) << '\n';
        } else if (q == "MAX_CONFLICT_TRIPS") {
            vector<pair<long long, int>> ev;
            for (auto& [tr, p] : span) ev.push_back({p.first, 1}), ev.push_back({p.second + 1, -1});
            sort(ev.begin(), ev.end());
            int cur = 0, mx = 0;
            for (auto [t, d] : ev) cur += d, mx = max(mx, cur);
            cout << mx << '\n';
        }
    }
    return 0;
}

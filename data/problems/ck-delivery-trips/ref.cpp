#include <bits/stdc++.h>
using namespace std;

int daysInMonth(int y, int m) {
    static int dm[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)) return 29;
    return dm[m];
}

long long parseDate(const string& d) {
    int y = stoi(d.substr(0, 4)), m = stoi(d.substr(5, 2)), day = stoi(d.substr(8, 2));
    // Days from 1970
    long long days = 0;
    for (int yy = 1970; yy < y; yy++) {
        days += ((yy % 4 == 0 && yy % 100 != 0) || yy % 400 == 0) ? 366 : 365;
    }
    for (int mm = 1; mm < m; mm++) days += daysInMonth(y, mm);
    days += day - 1;
    return days;
}

long long parseDT(const string& d, const string& t, unordered_map<string, long long>& day_cache) {
    long long dy;
    auto it = day_cache.find(d);
    if (it != day_cache.end()) dy = it->second;
    else dy = day_cache[d] = parseDate(d);
    int hh = (t[0]-'0')*10 + (t[1]-'0');
    int mm = (t[3]-'0')*10 + (t[4]-'0');
    int ss = (t[6]-'0')*10 + (t[7]-'0');
    return dy * 86400LL + hh * 3600LL + mm * 60LL + ss;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    unordered_map<string, long long> day_cache;
    int day_idx = 0;
    while (cin >> s && s != "*") {
        day_cache[s] = day_idx++;
    }
    long long total_qty = 0;
    unordered_map<string, long long> cust_qty;
    unordered_map<string, pair<long long, long long>> trip_span;
    unordered_map<long long, long long> time_qty;

    string trip, cust, dt, tm;
    long long qty;
    while (cin >> trip && trip != "***") {
        cin >> cust >> dt >> tm >> qty;
        total_qty += qty;
        cust_qty[cust] += qty;
        long long ts = parseDT(dt, tm, day_cache);
        time_qty[ts] += qty;
        auto it = trip_span.find(trip);
        if (it == trip_span.end()) trip_span[trip] = {ts, ts};
        else {
            it->second.first = min(it->second.first, ts);
            it->second.second = max(it->second.second, ts);
        }
    }

    vector<pair<long long, long long>> tq(time_qty.begin(), time_qty.end());
    sort(tq.begin(), tq.end());
    int M = tq.size();
    vector<long long> times(M);
    for (int i = 0; i < M; i++) times[i] = tq[i].first;
    // Sparse table for RMQ max on tq[i].second
    int K = 1;
    while ((1 << K) <= max(1, M)) K++;
    vector<vector<long long>> st(K, vector<long long>(max(1, M), 0));
    for (int i = 0; i < M; i++) st[0][i] = tq[i].second;
    for (int j = 1; j < K; j++) {
        for (int i = 0; i + (1 << j) <= M; i++) {
            st[j][i] = max(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
        }
    }
    auto rmq = [&](int l, int r) -> long long {
        if (l > r) return 0;
        int j = 31 - __builtin_clz(r - l + 1);
        return max(st[j][l], st[j][r - (1 << j) + 1]);
    };

    string q;
    while (cin >> q && q != "***") {
        if (q == "TOTAL_QTY") {
            cout << total_qty << "\n";
        } else if (q == "QTY_CUSTOMER") {
            string c; cin >> c;
            cout << cust_qty[c] << "\n";
        } else if (q == "QTY_MAX_PERIOD") {
            string d1, t1, d2, t2;
            cin >> d1 >> t1 >> d2 >> t2;
            long long s1 = parseDT(d1, t1, day_cache);
            long long s2 = parseDT(d2, t2, day_cache);
            int l = lower_bound(times.begin(), times.end(), s1) - times.begin();
            int r = upper_bound(times.begin(), times.end(), s2) - times.begin() - 1;
            cout << rmq(l, r) << "\n";
        } else if (q == "TOTAL_TRIPS") {
            cout << trip_span.size() << "\n";
        } else if (q == "TRAVEL_TIME_TRIP") {
            string tr; cin >> tr;
            auto it = trip_span.find(tr);
            if (it == trip_span.end()) cout << 0 << "\n";
            else cout << (it->second.second - it->second.first) << "\n";
        } else if (q == "MAX_CONFLICT_TRIPS") {
            // Maximum clique in interval graph = max number of overlapping intervals at any point!
            vector<pair<long long, int>> ev;
            for (auto &kv : trip_span) {
                ev.push_back({kv.second.first, 1});
                ev.push_back({kv.second.second + 1, -1}); // closed interval [L, R]
            }
            sort(ev.begin(), ev.end());
            int cur = 0, mx = 0;
            for (auto &e : ev) {
                cur += e.second;
                mx = max(mx, cur);
            }
            cout << mx << "\n";
        }
    }
    return 0;
}

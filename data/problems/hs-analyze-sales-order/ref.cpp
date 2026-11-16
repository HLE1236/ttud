#include <bits/stdc++.h>
using namespace std;

int toSec(const string& t) {
    return ((t[0]-'0')*10 + (t[1]-'0'))*3600 + ((t[3]-'0')*10 + (t[4]-'0'))*60 + ((t[6]-'0')*10 + (t[7]-'0'));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long totalOrders = 0, totalRev = 0;
    unordered_map<string, long long> shopRev, custShop;
    shopRev.reserve(65536);
    custShop.reserve(131072);
    vector<long long> pref(86401, 0);

    string cust, prod, shop, tm;
    long long price;
    while (cin >> cust && cust != "#") {
        cin >> prod >> price >> shop >> tm;
        totalOrders++;
        totalRev += price;
        shopRev[shop] += price;
        custShop[cust + "#" + shop] += price;
        pref[toSec(tm) + 1] += price;
    }
    for (int i = 1; i <= 86400; i++) pref[i] += pref[i - 1];

    string q;
    while (cin >> q && q != "#") {
        if (q == "?total_number_orders") cout << totalOrders << '\n';
        else if (q == "?total_revenue") cout << totalRev << '\n';
        else if (q == "?revenue_of_shop") {
            string s; cin >> s;
            auto it = shopRev.find(s);
            cout << (it == shopRev.end() ? 0 : it->second) << '\n';
        } else if (q == "?total_consume_of_customer_shop") {
            string c, s; cin >> c >> s;
            auto it = custShop.find(c + "#" + s);
            cout << (it == custShop.end() ? 0 : it->second) << '\n';
        } else if (q == "?total_revenue_in_period") {
            string t1, t2; cin >> t1 >> t2;
            int l = toSec(t1), r = toSec(t2);
            cout << (l <= r ? pref[r + 1] - pref[l] : 0) << '\n';
        }
    }
    return 0;
}

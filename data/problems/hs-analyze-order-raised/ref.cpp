#include <bits/stdc++.h>
using namespace std;
int to_sec(const string& t) {
    int h = stoi(t.substr(0, 2)), m = stoi(t.substr(3, 2)), s = stoi(t.substr(6, 2));
    return h * 3600 + m * 60 + s;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    const int MAXS = 86400;
    vector<int> cnt(MAXS, 0), pref(MAXS, 0);
    int total = 0; string id;
    while (cin >> id && id != "#") {
        string tm; cin >> tm;
        cnt[to_sec(tm)]++;
        total++;
    }
    pref[0] = cnt[0];
    for (int i = 1; i < MAXS; ++i) pref[i] = pref[i - 1] + cnt[i];
    string q;
    while (cin >> q && q != "###") {
        if (q == "?number_orders") cout << total << "\n";
        else if (q == "?number_orders_at_time") {
            string tm; cin >> tm; cout << cnt[to_sec(tm)] << "\n";
        } else if (q == "?number_orders_in_period") {
            string t1, t2; cin >> t1 >> t2;
            int s1 = to_sec(t1), s2 = to_sec(t2);
            cout << pref[s2] - (s1 > 0 ? pref[s1 - 1] : 0) << "\n";
        }
    }
    return 0;
}

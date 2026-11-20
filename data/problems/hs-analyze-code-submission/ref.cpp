#include <bits/stdc++.h>
using namespace std;

int toSec(const string& t) {
    return ((t[0]-'0')*10 + (t[1]-'0'))*3600 + ((t[3]-'0')*10 + (t[4]-'0'))*60 + ((t[6]-'0')*10 + (t[7]-'0'));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int totalSub = 0, errSub = 0;
    unordered_map<string, int> userErr, userTotal, bestProb;
    vector<int> pref(86401, 0);

    string uid, pid, tm, st;
    int pt;
    while (cin >> uid && uid != "#") {
        cin >> pid >> tm >> st >> pt;
        totalSub++;
        if (st == "ERR") {
            errSub++;
            userErr[uid]++;
        }
        string k = uid + "#" + pid;
        if (pt > bestProb[k]) {
            userTotal[uid] += pt - bestProb[k];
            bestProb[k] = pt;
        }
        pref[toSec(tm) + 1]++;
    }
    for (int i = 1; i <= 86400; i++) pref[i] += pref[i - 1];

    string q;
    while (cin >> q && q != "#") {
        if (q == "?total_number_submissions") cout << totalSub << '\n';
        else if (q == "?number_error_submision") cout << errSub << '\n';
        else if (q == "?number_error_submision_of_user") {
            string u; cin >> u;
            auto it = userErr.find(u);
            cout << (it == userErr.end() ? 0 : it->second) << '\n';
        } else if (q == "?total_point_of_user") {
            string u; cin >> u;
            auto it = userTotal.find(u);
            cout << (it == userTotal.end() ? 0 : it->second) << '\n';
        } else if (q == "?number_submission_period") {
            string t1, t2; cin >> t1 >> t2;
            int l = toSec(t1), r = toSec(t2);
            cout << (l <= r ? pref[r + 1] - pref[l] : 0) << '\n';
        }
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    unordered_map<string, int> qPoint;
    unordered_map<string, pair<string, int>> ansInfo;
    string qid, aid, uid, tid, tm;
    int pt, tf;

    while (cin >> qid && qid != "#") {
        cin >> pt;
        qPoint[qid] = pt;
    }
    while (cin >> aid && aid != "#") {
        cin >> qid >> tf;
        ansInfo[aid] = {qid, tf};
    }

    unordered_map<string, pair<string, int>> latest;
    while (cin >> uid && uid != "#") {
        cin >> tid >> aid >> tm;
        auto it = ansInfo.find(aid);
        if (it == ansInfo.end()) continue;
        const string& q = it->second.first;
        int ok = it->second.second;
        string key = uid + "#" + tid + "#" + q;
        auto lit = latest.find(key);
        if (lit == latest.end() || tm >= lit->second.first) {
            latest[key] = {tm, ok ? qPoint[q] : 0};
        }
    }

    unordered_map<string, long long> testScore;
    for (const auto& kv : latest) {
        if (kv.second.second == 0) continue;
        size_t p1 = kv.first.find('#');
        size_t p2 = kv.first.find('#', p1 + 1);
        testScore[kv.first.substr(0, p2)] += kv.second.second;
    }

    string cmd;
    while (cin >> cmd && cmd != "###") {
        cin >> uid >> tid;
        auto it = testScore.find(uid + "#" + tid);
        cout << (it == testScore.end() ? 0 : it->second) << '\n';
    }
    return 0;
}

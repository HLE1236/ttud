#include <bits/stdc++.h>
using namespace std;
int main() {
    string a, b;
    if (!(cin >> a >> b)) return 0;
    string res;
    int i = (int)a.size() - 1, j = (int)b.size() - 1, carry = 0;
    while (i >= 0 || j >= 0 || carry) {
        int s = carry + (i >= 0 ? a[i--] - '0' : 0) + (j >= 0 ? b[j--] - '0' : 0);
        res.push_back(char('0' + s % 10));
        carry = s / 10;
    }
    reverse(res.begin(), res.end());
    cout << res << "\n";
    return 0;
}

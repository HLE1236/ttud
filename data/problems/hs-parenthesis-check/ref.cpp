#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    vector<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push_back(c);
        } else {
            if (st.empty()) { cout << 0 << '\n'; return 0; }
            char top = st.back();
            if ((c == ')' && top == '(') || (c == ']' && top == '[') || (c == '}' && top == '{')) {
                st.pop_back();
            } else {
                cout << 0 << '\n'; return 0;
            }
        }
    }
    cout << (st.empty() ? 1 : 0) << '\n';
    return 0;
}

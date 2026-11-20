#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> h(m + 1, 0);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int x; cin >> x;
            h[j] = x ? (h[j] + 1) : 0;
        }
        h[m] = 0;
        vector<int> st;
        for (int j = 0; j <= m; j++) {
            while (!st.empty() && h[st.back()] >= h[j]) {
                int height = h[st.back()];
                st.pop_back();
                int width = st.empty() ? j : (j - st.back() - 1);
                ans = max(ans, height * width);
            }
            st.push_back(j);
        }
    }
    cout << ans << '\n';
    return 0;
}

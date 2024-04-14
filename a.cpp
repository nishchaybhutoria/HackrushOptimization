#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx,avx2,sse,sse2,popcnt")
 
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
 
using namespace __gnu_pbds;
using namespace std;
 
#define ll long long
#define ld long double
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define mp make_pair
#define F first
#define S second
#define eb emplace_back
#define pb push_back
#define random mt19937 rng(chrono::steady_clock::now().time_since_epoch().count())

inline void solve() {
    int n, m;
    cin >> n >> m;
    vector <vector <int>> s(n + 1, vector <int> (m + 1));
    vector <vector <int>> events(m + 1, vector <int> (3));
    vector <pair <ld, pair <int, int>>> score;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cin >> s[i][j];
        }
    }
    for (int i = 1; i <= m; ++i) {
        cin >> events[i][0] >> events[i][1] >> events[i][2];
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            ld val = ((ld) s[i][j] * s[i][j] * s[i][j] * s[i][j] * s[i][j] * s[i][j] * s[i][j] * s[i][j] * s[i][j]) / ((ld)(events[j][1] - events[j][0] + 1));
            score.eb(mp(val, mp(i, j)));
        }
    }
    sort(rall(score));
    vector <int> current_cnt(m + 1);
    vector <vector <int>> ans(n + 1);
    vector <vector <int>> overlap(m + 1, vector <int> (m + 1));
    for (int i = 1; i <= m; ++i) {
        for (int j = i; j <= m; ++j) {
            int a = events[i][0], b = events[i][1];
            int c = events[j][0], d = events[j][1];
            // events [a, b], [c, d] dont overlap iff (a, b) <= (c, d) or (c, d) <= (a, b)
            bool not_overlap = (b < c) || (d < a);
            if (!not_overlap) {
                overlap[i][j] = 1;
                overlap[j][i] = 1;
            }
        }
    }
    for (auto &p : score) {
        auto [i, j] = p.S;
        bool chk = (current_cnt[j] < events[j][2]);
        for (int event : ans[i]) {
            chk &= (!overlap[event][j]);
        }
        if (chk) {
            ans[i].eb(j);
            ++current_cnt[j];
        }
    }
    int lines = 0;
    int numbers = 1;
    for (int i = 1; i <= n; ++i) {
        cout << ans[i].size();
        ++lines;
        int number = 0;
        if (ans[i].empty()) {
            cout << '\n';
            continue;
        } else {
            cout << " ";
        }
        for (int j = 0; j < (int)ans[i].size(); ++j) {
            cout << ans[i][j] << " \n"[j == (int)ans[i].size() - 1];
            ++number;
        }
        numbers &= (number == (int)ans[i].size());
    }
    assert(lines == n && numbers);
}
 
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int tests = 1;
    // cin >> tests;
    for (int _ = 0; _ < tests; ++_) {
        solve();
    }
}

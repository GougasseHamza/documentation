/*
 * Problem it Solves: Computes partition function P(n) using Pentagonal Number Theorem.
 * Usage: yo(n) returns vector of partitions up to n.
 * Input/Output: n (int) -> vector<int> (mod MOD).
 * Time Complexity: O(N sqrt(N)).
 * Tweaks: Can be optimized to O(N log N) using FFT.
 * Other Use Cases: Partition counting.
 * Warnings: Memory usage.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
using ll = long long;

// ans[i] = number of partitions of i
// O(n sqrt(n))
// takes 1.5s for n = 5e5
vector<int> yo(int n) {
    vector<int> ans(n + 1);
    vector<pair<int, int>> gp; // (sign, generalized pentagonal numbers)
    gp.emplace_back(0, 0);
    for (int i = 1; gp.back().second <= n; i++) {
        gp.emplace_back(i % 2 ? 1 : -1, i * (3 * i - 1) / 2);
        gp.emplace_back(i % 2 ? 1 : -1, i * (3 * i + 1) / 2);
    }
    ans[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (auto it : gp) {
            if (i >= it.second) {
                if (it.first == 1) ans[i] = (ans[i] + ans[i - it.second]) % MOD;
                else ans[i] = (ans[i] - ans[i - it.second] + MOD) % MOD;
            } else break;
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n = 100000;
    auto ans = yo(n);
    for (int i = 0; i < 10; i++) cout << ans[i] << ' '; cout << '\n';
    cerr << 1.0 * (clock()) / CLOCKS_PER_SEC << '\n';
    return 0;
}

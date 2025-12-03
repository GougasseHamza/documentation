/*
 * Problem it Solves: Calculates number of derangements (permutations with no fixed points).
 * Usage: d[n] gives derangements of length n.
 * Input/Output: n (integer) -> number of derangements mod MOD.
 * Time Complexity: O(N) precomputation.
 * Tweaks: Change N and MOD.
 * Other Use Cases: Hat check problem.
 * Warnings: Modulo arithmetic applies.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9, MOD = 1e9 + 7;

int d[N];

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    d[0] = 1; d[1] = 0;
    for (int i = 2; i < N; i++) {
        d[i] = 1LL * (i - 1) * (d[i - 1] + d[i - 2]) % MOD;
    }
    int n; cin >> n;
    cout << d[n] << '\n';
    return 0;
}

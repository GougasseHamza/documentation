/*
 * Problem it Solves: Finds all possible XOR sums of perfect matchings reachable from an initial diagonal matching by 2-opt moves.
 * Usage: Call yo(initial_xor).
 * Input/Output: Matrix a[n][n]. Output all reachable xor sums.
 * Time Complexity: Heuristic, depends on state space connectivity.
 * Tweaks: N is max matrix size.
 * Other Use Cases: Exploring permutation parity or similar state spaces.
 * Warnings: Not guaranteed to find ALL perfect matchings if the state space isn't connected by 2-opts.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1030; // Max XOR value or state size. TWEAK: Adjust based on max possible XOR sum.

int a[N][N], p[N], n;
int vis[N]; // Visited XOR sums

// KEY: Explores states by swapping p[i] and p[j], effectively changing edges (i, p[i]), (j, p[j]) to (i, p[j]), (j, p[i])
// This is a 2-opt move on the matching.
void yo(int cur) {
    if (vis[cur]) return;
    vis[cur] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            // Update XOR sum based on the swap
            // Remove old edges: a[i][p[i]], a[j][p[j]]
            // Add new edges: a[i][p[j]], a[j][p[i]]
            int nxt = cur ^ a[i][p[i]] ^ a[j][p[j]] ^ a[i][p[j]] ^ a[j][p[i]];
            
            swap(p[i], p[j]);
            yo(nxt);
            swap(p[i], p[j]); // Backtrack
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        cin >> n;
        int cur = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cin >> a[i][j];
                if (i == j) {
                    p[i] = i;
                    cur ^= a[i][i]; // Initial matching is the diagonal
                }
            }
        }
        memset(vis, 0, sizeof vis);
        yo(cur);
        for (int i = 0; i < N; i++) {
            if (vis[i]) cout << i << ' ';
        }
        cout << '\n';
    }
    return 0;
}

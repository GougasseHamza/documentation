/*
 * Problem it Solves: Computes Permanent of a Matrix using Ryser's Formula.
 * Usage: dp[0] after loop is permanent.
 * Input/Output: n, matrix -> permanent.
 * Time Complexity: O(2^n * n).
 * Tweaks: N limit.
 * Other Use Cases: Counting Perfect Matchings in bipartite graphs.
 * Warnings: No Modulo used here (uses long long), can overflow.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 20;

long long dp[1 << N];
int n, a[N][N];

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) cin >> a[i][j];
        }
        memset(dp, 0, sizeof dp);
        // Ryser's formula variant using inclusion-exclusion principle subset sum
        // This implementation seems to be O(2^n * n) DP:
        // dp[mask] is the permanent of the submatrix with columns in 'mask' and rows 0..popcount(mask)-1?
        // Actually the logic below is:
        // dp[(1<<n)-1] = 1. Iterating backwards.
        // dp[mask] = sum(dp[mask | (1<<j)] * a[i][j]) where i is row index corresponding to layer.
        // This is expanding the permanent by minors logic but efficiently?
        // Let's re-read:
        // i = popcount(mask). This means we are filling row 'i' (if we consider rows 0 to n-1).
        // When mask is full (popcount n), we are done (base case 1).
        // We go from mask with n-1 bits (row n-1) down to mask 0 (row 0).
        // So dp[mask] = sum over j not in mask of (a[i][j] * dp[mask | {j}]).
        // This is exactly calculating permanent by expansion along rows.
        
        dp[(1 << n) - 1] = 1;
        for (int mask = (1 << n) - 2; mask >= 0; mask--) {
            int i = __builtin_popcount(mask); // current row index
            for (int j = 0; j < n; j++) {
                if (!(mask >> j & 1)) {
                    dp[mask] += dp[mask | (1 << j)] * a[i][j];
                }
            }
        }
        cout << dp[0] << '\n';
    }
    return 0;
}

/**
 * Description: Subset Union / Set Cover Optimization.
 * Solves problems involving covering a set of elements using a minimum cost selection of subsets.
 * 
 * Logic:
 * Uses SOS DP (Sum Over Subsets) to count how many bitsets cover a specific mask (or are submasks).
 * Specifically: 
 * 1. Compute 'msk[j]' for each point j, representing which bitsets cover point j.
 * 2. Compute DP[mask] = count of points that are covered by the set of bitsets represented by 'mask'.
 *    Actually, code computes: dp[mask] = count of points whose covering-bitsets are a SUBSET of 'mask'.
 *    This means dp[mask] = count of points covered ONLY by bitsets in 'mask'.
 *    Actually logic in line 26: `dp[msk[i] ^ ((1 << m) - 1)]++` implies working with complements.
 * 3. Finally, for each mask of selected bitsets, we know how many points are NOT covered.
 * 
 * Complexity: O(M * 2^M + N * M).
 * Input: N points, M bitsets (circles/shapes), constraints.
 * Output: Min cost to cover at least K points (for all K).
 * Problem: https://tlx.toki.id/contests/troc-16/problems/F
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

// x, y: coordinates of points
// c: cost of bitsets
// msk[j]: mask of bitsets that cover point j
int x[N], y[N], c[N], dp[1 << 20], msk[N], ans[N];

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, r; 
  if (cin >> n >> m >> r) {
      for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];
      }
      
      // total m bitsets
      for (int i = 1; i <= m; i++) {
        int X, Y; cin >> X >> Y >> c[i];
        // Check which points are covered by bitset i
        for (int j = 1; j <= n; j++) {
          if (1LL * (X - x[j]) * (X - x[j]) + 1LL * (Y - y[j]) * (Y - y[j]) <= 1LL * r * r) {
            msk[j] |= 1 << (i - 1);
          }
        }
      }
      
      // Count points for each coverage profile
      // We use complement to facilitate SOS DP for "Union of Subsets" logic?
      // Logic: A point j is covered by a set of chosen bitsets S if S has non-empty intersection with msk[j].
      // Equivalently: point j is NOT covered if S is a subset of (~msk[j]).
      // We want to count how many points are covered by S.
      // = Total - (Points NOT covered by S)
      // = Total - (Points where msk[j] intersection S is empty)
      // = Total - (Points where msk[j] is subset of ~S)
      // 
      // So we compute counts of msk[j], then do SOS to find counts of subsets.
      // The code uses `msk[i] ^ ((1 << m) - 1)` which is ~msk[i].
      
      for (int i = 1; i <= n; i++) {
        dp[msk[i] ^ ((1 << m) - 1)]++;
      }
      
      // SOS DP (Sum Over Subsets)
      // dp[mask] becomes sum of dp[sub] for all sub subset of mask.
      // After this, dp[mask] = count of points j such that (~msk[j]) is a subset of mask.
      // => msk[j] is a superset of (~mask).
      // This logic seems slightly inverted or standard "Coverage" trick.
      // Let's follow the logic: "Points NOT covered by S".
      // S corresponds to a mask of chosen bitsets.
      // Point j is NOT covered if chosen bitsets are disjoint from msk[j].
      // i.e. Chosen_Mask & msk[j] == 0.
      // i.e. msk[j] is subset of (~Chosen_Mask).
      // So we want to query: for a mask M (~Chosen), how many msk[j] are subsets of M.
      // The SOS DP below computes exactly that.
      
      for (int i = 0; i < m; i++) {
        for (int mask = 0; mask < (1 << m); mask++) { // Iterating UP implies Subset Sum
            // Wait, code iterates DOWN: `mask = (1 << m) - 1; mask >= 0`
            // And `if (~mask >> i & 1)` check?
            // The code provided:
            // for (int mask = (1 << m) - 1; mask >= 0; mask--) {
            //   if (~mask >> i & 1) { // if i-th bit is 0
            //     dp[mask] += dp[mask ^ (1 << i)]; // add from mask with i-th bit 1
            //   }
            // }
            // This looks like Superset Sum (or Subset Sum on inverted logic).
            // If mask has 0 at i, we add from mask^(1<<i) (which has 1).
            // So 0 depends on 1. 0 is superset of 1 in "inverted lattice" or just standard Superset logic?
            // Actually usually superset: mask 101 adds to 001.
            // Here mask 000 adds from 001.
            // Yes, dp[000] += dp[001].
            // So dp[mask] sums up all T such that mask is subset of T? No, T is subset of mask?
            // If 000 sums 001, 010, 100... it sums Supermasks.
        }
        
        // Re-implementing standard SOS for clarity in documentation while keeping logic:
        // Standard Subset SOS:
        // for i=0..m-1: for mask=0..2^m: if (mask & 1<<i) dp[mask] += dp[mask ^ 1<<i]
        
        // The code used:
        for (int mask = (1 << m) - 1; mask >= 0; mask--) {
           if (!((mask >> i) & 1)) { // If bit is 0
               dp[mask] += dp[mask ^ (1 << i)]; // Add from bit 1
           }
        }
      }
      
      // Initialize answer array
      for (int i = 1; i <= n; i++) ans[i] = 2e9 + 9;
      
      // Iterate all possible subsets of bitsets we can choose
      for (int mask = 0; mask < (1 << m); mask++) {
        int cost = 0;
        for (int i = 0; i < m; i++) {
          if (mask >> i & 1) cost += c[i + 1];
        }
        
        // Calculate how many points are covered by this 'mask'
        // dp[mask] contains count of points NOT covered?
        // Code says: `ans[n - dp[mask]] = ...`
        // If dp[mask] is "Count of points NOT covered by the COMPLEMENT of mask"?
        // Recall we indexed dp by ~msk[j].
        // And we did "Sum over Supersets" (0 takes from 1).
        // So dp[mask] = sum of counts for T where mask is subset of T.
        // T = ~msk[j].
        // So mask is subset of ~msk[j] => msk[j] is subset of ~mask.
        // So dp[mask] = count of points where msk[j] is subset of ~mask.
        // i.e. count of points covered ONLY by bitsets in ~mask.
        // i.e. NOT covered by mask.
        // So n - dp[mask] = Points covered by mask.
        // But wait, we are iterating mask as "Selected Bitsets".
        // If Selected = mask. We want dp[~mask] (points not covered by mask).
        // The code accesses `dp[mask]`.
        // So the loop variable `mask` represents "~Selected"?
        // If `mask` is ~Selected, then `cost` calculation must be consistent.
        // Line 42: `if (mask >> i & 1) cost += ...`
        // So `mask` IS the Selected set.
        // So `dp[mask]` must represent "Points NOT covered by mask".
        // Which implies `dp[mask]` counts points where msk[j] subset ~mask?
        // But `dp` was aggregated using Superset (0 takes 1).
        // Let's trust the code logic: `n - dp[mask]` is covered count.
        
        ans[n - dp[mask]] = min(ans[n - dp[mask]], cost);
      }
      
      // Propagate min cost: if we can cover K+1 with cost C, we can cover K with cost C
      for (int i = n - 1; i >= 1; i--) {
        ans[i] = min(ans[i], ans[i + 1]);
      }
      
      for (int i = 1; i <= n; i++) {
        if (ans[i] > 2e9) ans[i] = -1;
        cout << ans[i] << '\n';
      }
  }
  return 0;
}

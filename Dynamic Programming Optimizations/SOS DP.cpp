/**
 * Description: Sum Over Subsets (SOS) DP.
 * Computes sum of values for all subsets (or supersets) of a mask.
 * 
 * Logic:
 * Iterates through each bit position.
 * For each bit, propagates the sum to the mask with that bit set (for subsets)
 * or from the mask with that bit set (for supersets).
 *
 * Complexity: O(N * 2^N) where N is number of bits.
 * Input: Array A.
 * Output: F (subset sums), G (superset sums), etc.
 * Problem: https://cses.fi/problemset/task/1654
 */

#include<bits/stdc++.h>
using namespace std;

const int B = 20; // Number of bits
int a[1 << B];
int f[1 << B]; // Subset sums
int g[1 << B]; // Superset sums

int32_t main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n; 
  if (cin >> n) {
      for (int i = 1; i <= n; i++) {
        cin >> a[i];
        f[a[i]]++;
        g[a[i]]++;
      }

      // SOS DP: Sum over Subsets
      // f[mask] += f[mask ^ (1<<i)] if (mask & (1<<i))
      for (int i = 0; i < B; i++) {
        for (int mask = 0; mask < (1 << B); mask++) {
          if ((mask & (1 << i)) != 0) {
            f[mask] += f[mask ^ (1 << i)];
          }
        }
      }

      // SOS DP: Sum over Supersets
      // g[mask] += g[mask ^ (1<<i)] if (mask & (1<<i)) == 0
      // We iterate mask downwards or just handle indexing carefully.
      for (int i = 0; i < B; i++) {
        for (int mask = (1 << B) - 1 ; mask >= 0 ; mask--) {
          if ((mask & (1 << i)) == 0) {
             g[mask] += g[mask ^ (1 << i)];
          }
        }
      }
      
      for (int i = 1; i <= n; i++) {
        // 1. Number of j such that a[j] is subset of a[i] -> f[a[i]]
        // 2. Number of j such that a[i] is subset of a[j] -> g[a[i]]
        // 3. Number of j such that a[i] AND a[j] == 0 -> (handled by subset sum on complement)
        //    We need subsets of (~a[i]).
        
        // Note: The mask for complement should be limited to B bits.
        // (1 << B) - 1 is the full mask.
        
        int comp = ((1 << B) - 1) ^ a[i];
        
        cout << f[a[i]] << ' ' << g[a[i]] << ' ' << f[comp] << '\n';
      }
      // 4. Inverse SOS DP (Inclusion-Exclusion)
    // We currently have "Subsets with AND sum containing mask".
    // We want "Subsets with AND sum == mask".
    // We must subtract ways[superset] from ways[mask].
    for (int i = 0; i < BITS; ++i) {
      for (int mask = 0; mask < LIMIT; ++mask) {
          if (!(mask & (1 << i))) {
              // Subtract the superset count to isolate the exact match
              ways[mask] = (ways[mask] - ways[mask | (1 << i)] + MOD) % MOD;
          }
      }
  }
  }
  return 0;
}

/**
 * Description: Connected Component DP (Broken Profile / Permutation Building).
 * Useful for counting permutations or sequences satisfying conditions related to adjacent elements (e.g., difference sums).
 * The state usually tracks: (index, number_of_components, current_sum, number_of_ends_filled).
 *
 * Logic:
 * We insert elements in sorted order (usually).
 * When inserting element i, it can:
 * 1. Create a new component (it is isolated).
 * 2. Merge two existing components (it connects them).
 * 3. Attach to one end of an existing component.
 *
 * Complexity: O(N^2 * K) or O(N^2 * Sum).
 * Input: N, K, array A.
 * Output: Number of valid permutations/arrangements modulo MOD.
 * 
 * Problem: JOI16 Skyscraper (Arrange A such that sum(|A[i] - A[i+1]|) <= K).
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 105, mod = 1e9 + 7;

int n, k, a[N];
// dp[i][c][sum][ends]
// i: number of elements placed so far (sorted 1..N)
// c: number of connected components formed by these elements
// sum: current cost contribution (sum of differences so far).
//      Contribution trick: |x - y| = contribution of x and y to the sum. 
//      When we place a[i], if it starts a component, it contributes -2*a[i] (effectively, will cancel later).
//      Alternatively, standard trick: sum += (a[i] - a[i-1]) * (2 * c - ends).
//      This calculates the cost "gap" between a[i-1] and a[i] for all open boundaries.
// ends: number of "endpoints" of the final permutation that are already filled (0, 1, or 2).
int dp[N][N][1005][3];

int yo(int i, int c, int sum, int ends) { 
  if (sum > k) return 0;
  if (c < 0) return 0; // Safety
  if (i == n + 1) {
    return ends == 2 && c == 1; // Must form 1 component and fill both ends
  }
  
  int &ret = dp[i][c][sum][ends];
  if (ret != -1) return ret;
  
  // Calculate cost added by extending the active components from a[i-1] to a[i]
  // Each component has 2 sides, except those touching the ends of the permutation.
  // Total open sides = 2*c - ends.
  int nsum = sum + (a[i] - a[i - 1]) * (2 * c - ends);
  
  long long ans = 0;

  // Transitions:
  
  // 1. Create a new component
  // It can be placed in any of the (c + 1) slots between/outside existing components.
  // But we must respect 'ends'.
  // Slots available = c + 1 - ends.
  if (c + 1 - ends >= 0)
      ans += 1LL * (c + 1 - ends) * yo(i + 1, c + 1, nsum, ends);
  
  // 2. Merge two components
  // We place a[i] between two components, reducing count by 1.
  // Valid only if c >= 2.
  if (c >= 2) 
      ans += 1LL * (c - 1) * yo(i + 1, c - 1, nsum, ends);
  
  // 3. Attach to one end of a component
  // Does not change component count.
  // Available spots: 2*c - ends.
  if (c >= 1) 
      ans += 1LL * (2 * c - ends) * yo(i + 1, c, nsum, ends);
  
  // 4. New component that acts as a border (fills one of the global ends)
  // Increases ends by 1.
  if (ends < 2) 
      ans += 1LL * (2 - ends) * yo(i + 1, c + 1, nsum, ends + 1);
      
  // 5. Extend a component to act as a border
  // Does not change component count, but increases ends.
  if (ends < 2 && c >= 1) 
      ans += 1LL * (2 - ends) * yo(i + 1, c, nsum, ends + 1);

  ans %= mod;
  return ret = ans;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n >> k;
  for (int i = 1; i <= n; i++) cin >> a[i];
  sort(a + 1, a + n + 1);
  
  // Special case for N=1
  if (n == 1) {
      if (0 <= k) cout << 1 << '\n';
      else cout << 0 << '\n';
      return 0;
  }
  
  memset(dp, -1, sizeof dp);
  // Start with 1st element. 
  // Initial call: index 1, 0 components (will be created inside), sum 0, 0 ends.
  // Actually, the transitions assume we are placing 'i'.
  // Let's manually handle the first step or adjust the DP.
  // The transitions above are for: state at i -> state at i+1.
  // So we start at i=1 with "0 components" is weird because we place a[1].
  // Standard: call yo(1, 0, 0, 0) and inside handle placements for '1'.
  // The code logic inside yo handles placing 'i'.
  // So initially we have 0 components, we place 1.
  
  cout << yo(1, 0, 0, 0) << '\n';
  return 0;
}

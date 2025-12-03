/**
 * Description: 1D/1D Dynamic Programming Optimization.
 * Solves DP of the form: dp[i] = min(dp[j] + w(j, i)) for 0 <= j < i.
 * Requires the cost function w(j, i) to satisfy the Quadrangle Inequality.
 * This allows the optimal transition point opt[i] to be non-decreasing (opt[i] <= opt[i+1]).
 * This implementation uses a stack/deque to maintain the optimal transition points for future states.
 *
 * Complexity: O(N log N) due to binary search for finding the cut-off point.
 * Input: N, specific problem parameters (here s, t, f arrays).
 * Output: dp[n]
 *
 * Problem: https://vjudge.net/problem/OpenJ_Bailian-1180
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 1e4 + 9;

/*
 * Key Logic:
 * We maintain a list of candidates 'v'. Each element in 'v' is a pair (start_index, best_k).
 * (start_index, best_k) means that for any query index >= start_index, 'best_k' is the best previous state to transition from
 * (until the next element in v takes over).
 */

int n, s, t[N], f[N];

// Cost function w(j, i). MODIFIABLE.
// Calculates the cost to transition from j to i.
int w(int j, int i) {
  if (j >= i) return 1e9; // Invalid transition
  // Example cost function:
  return s * (f[n] - f[j]) + t[i] * (f[i] - f[j]);
}

int dp[N];

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  cin >> n >> s;
  t[0] = 0;
  f[0] = 0;
  for (int i = 1; i <= n; i++) {
    cin >> t[i] >> f[i];
    t[i] += t[i - 1]; // Prefix sum if needed
    f[i] += f[i - 1];
  }

  dp[0] = 0;
  // v stores pairs of {start_index, best_candidate_index}
  vector<pair<int, int>> v;
  v.push_back({0, 0}); // From index 0 onwards, candidate 0 is best initially

  for (int i = 1; i <= n; i++) {
    // Find the best candidate for current i
    // We need the candidate whose range covers i.
    // Since v is sorted by start_index, we look for the last element with start_index <= i.
    int k = (--lower_bound(v.begin(), v.end(), make_pair(i + 1, 0)))->second;
    
    dp[i] = dp[k] + w(k, i);

    // Update the candidates for future states (i+1 to n) using current i
    // We try to remove worse candidates from the back of the stack
    while (!v.empty()) {
      int start_pos = v.back().first;
      int old_k = v.back().second;
      
      // Check if current i is better than old_k at start_pos
      // If i is better even at the start of the interval controlled by old_k, 
      // then old_k is completely useless for that interval and beyond.
      if (start_pos > i && dp[i] + w(i, start_pos) < dp[old_k] + w(old_k, start_pos)) {
        v.pop_back();
      } else {
        // Find the intersection point where i becomes better than old_k
        int l = start_pos + 1, r = n + 1;
        int best_pos = n + 1;
        while (l < r) {
          int mid = (l + r) / 2;
          if (dp[i] + w(i, mid) < dp[old_k] + w(old_k, mid)) {
            best_pos = mid;
            r = mid;
          } else {
            l = mid + 1;
          }
        }
        
        if (best_pos != n + 1) {
          v.push_back({best_pos, i});
        }
        break;
      }
    }
    
    if (v.empty()) v.push_back({0, i}); // Should ideally not happen if logic is correct for i=1
  }

  cout << dp[n] << '\n';
  return 0;
}

/**
 * Description: Convex Hull Trick (CHT).
 * Optimization for DP states of the form: dp[i] = min/max(a[i]*b[j] + dp[j]).
 * This effectively queries the minimum/maximum value of a set of lines y = mx + c at a given x.
 *
 * Limitations: 
 * - This implementation assumes lines are added with MONOTONIC slopes.
 * - If slopes are not monotonic, use Dynamic CHT (Li Chao Tree or std::set version).
 *
 * Complexity: 
 * - Add: O(1) amortized.
 * - Query: O(log N) using binary search.
 * Input: Lines (m, b). Query x.
 * Output: min/max value at x.
 */

#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define nl '\n'

struct CHT {
  vector<ll> m, b;
  int ptr = 0;

  // Checks intersection of lines to maintain convex hull.
  // Returns true if l2 is redundant given l1 and l3.
  bool bad(int l1, int l2, int l3) {
    // Intersection of (l1,l2) is (b2-b1)/(m1-m2)
    // Intersection of (l2,l3) is (b3-b2)/(m2-m3)
    // For MIN queries with slopes DECREASING: want intersect(l1,l2) < intersect(l2,l3)
    // Careful with floating point precision or overflow.
    
    // Generic form: (b3-b1)/(m1-m3) vs ...
    // Using __int128 avoids overflow for large coordinates
    return (__int128)(b[l3] - b[l1]) * (m[l1] - m[l2]) <= (__int128)(b[l2] - b[l1]) * (m[l1] - m[l3]);
    
    // Original logic for reference:
    // return 1.0 * (b[l3] - b[l1]) * (m[l1] - m[l2])  <= 1.0 * (b[l2] - b[l1]) * (m[l1] - m[l3]);
  }

  // Add line y = mx + b
  // NOTE: Slopes must be added in monotonic order!
  // For MIN query:
  // - if slopes decreasing: normal hull.
  // - if slopes increasing: add(-m, -b) and query(-x)? Or just change bad() logic.
  void add(ll _m, ll _b) {
    m.push_back(_m);
    b.push_back(_b);
    int s = m.size();
    while(s >= 3 && bad(s - 3, s - 2, s - 1)) {
      m.erase(m.end() - 2);
      b.erase(b.end() - 2);
      s--;
    }
  }

  ll f(int i, ll x) {
    return m[i] * x + b[i];
  }

  // Query for optimal value at x
  // If queries are also monotonic, use the pointer 'ptr' (O(1)).
  ll query(ll x) {
    if (ptr >= m.size()) ptr = m.size() - 1;
    while (ptr < m.size() - 1 && f(ptr + 1, x) < f(ptr, x)) ptr++; 
    return f(ptr, x);
  }

  // Binary search for optimal line at x
  // Use this if queries are NOT monotonic
  ll query_bs(ll x) {
      int l = 0, r = m.size() - 1;
      while (l < r) {
          int mid = (l + r) / 2;
          if (f(mid, x) > f(mid + 1, x)) { // > for min query (checking if next line is better)
              l = mid + 1;
          } else {
              r = mid;
          }
      }
      return f(l, x);
  }
};

ll a[300005], b[300005];
CHT cht;

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n;
  if (!(cin >> n)) return 0;
  for(int i = 0; i < n; i++) cin >> a[i];
  for(int i = 0; i < n; i++) cin >> b[i];
  
  // DP: dp[i] = min(dp[j] + b[j] * a[i]) ? 
  // Example usage
  cht.add(b[0], 0); // Add first line
  ll ans = 0;
  for(int i = 1; i < n; i++) {
    ans = cht.query(a[i]); // or query_bs if a[i] not sorted
    cht.add(b[i], ans);
  }
  cout << ans << nl;
  return 0;
}

/**
 * Description: Knuth's Optimization.
 * Optimizes DP of the form: dp[l][r] = min(dp[l][m] + dp[m][r]) + Cost(l, r)
 * condition: opt[l][r-1] <= opt[l][r] <= opt[l+1][r]
 * This reduces complexity from O(N^3) to O(N^2).
 *
 * Common problem type: Optimal Binary Search Tree, Matrix Chain Multiplication (with specific costs), Cutting Sticks.
 *
 * Complexity: O(N^2).
 * Input: N, array A (positions of cuts).
 * Output: Min cost.
 * Problem: https://vjudge.net/problem/ZOJ-2860 (Breaking strings/sticks)
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 1010;
using ll = long long;

/*
 * dp[l][r]: min cost to solve range (l, r)
 * mid[l][r]: optimal split point 'm' for range (l, r)
 * 
 * Recurrence:
 * dp[l][r] = min_{m} (dp[l][m] + dp[m][r]) + (a[r] - a[l])
 * (a[r] - a[l]) is the cost of the current cut (length of stick).
 */

int n, k;
int a[N], mid[N][N];
ll res[N][N];

ll solve() {
  // s is length of the sub-interval (number of segments inside)
  // logic: we have points a[0]...a[k+1].
  // intervals are between indices.
  // Range length s refers to index difference.
  
  for (int s = 0; s <= k; s++) {          
    for (int l = 0; l + s <= k; l++) {  
      int r = l + s;                  
      
      if (s < 2) {
        res[l][r] = 0;              // Adjacent points, cost is 0 (no cuts needed inside)
        mid[l][r] = l;              
        continue;
      }
      
      int mleft = mid[l][r - 1];
      int mright = mid[l + 1][r];
      
      res[l][r] = 2e18;
      
      // Iterate m only within the optimized bounds
      for (int m = mleft; m <= mright; m++) {    
        ll tmp = res[l][m] + res[m][r] + (a[r] - a[l]);
        if (res[l][r] > tmp) {                 
          res[l][r] = tmp;
          mid[l][r] = m;
        }
      }
    }
  }
  ll ans = res[0][k];
  return ans;
}

int main() {
  // Input: N (length), K (number of cuts), then K cut positions.
  while(cin >> n >> k) {
    for(int i = 1; i <= k; i++) cin >> a[i];
    a[0] = 0;
    a[k + 1] = n;
    k++; // Now we have points 0..k (total k+1 points), indices 0 to k.
    
    cout << solve() << endl;
  }
  return 0;
}

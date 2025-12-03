/**
 * Description: Number of Subsequences Having Product at least K.
 * Uses a Meet-in-the-middle / SQRT Decomposition approach on the DP state.
 * Standard DP: dp[i][p] = count of subsequences of first i elements with product p.
 * Since p can be large, we observe that if p >= SQRT(MAX_VAL), we track K/p instead.
 *
 * Logic:
 * dp1[p]: count of subseqs with product 'p' (where p < SQ).
 * dp2[val]: count of subseqs with product 'P' such that floor(K/P) = val.
 *
 * Complexity: O(N * Sqrt(K)).
 * Input: N, K, Array A.
 * Output: Count of subsequences with product >= K (or < K depending on logic).
 * Problem: https://codeforces.com/group/NOwIbqv33y/contest/307527/problem/J
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 1010, mod = 1e9 + 7;
const int SQ = 32000; // Adjust based on K (approx sqrt(10^9) ~ 31622)

int a[N], k;
int dp1[N][SQ], dp2[N][SQ];

// dp1[i][p]: using suffix i..n, product is exactly p (p < SQ)
// Actually code uses prefix/suffix recursion.
// mul_back(i, p): suffix 1..i, product p (limit check logic)
int mul_back(int i, int p) {
  if (i <= 0) return p >= 1;
  int &ret = dp1[i][p];
  if (ret != -1) return ret;
  ret = mul_back(i - 1, p); // Skip a[i]
  ret += mul_back(i - 1, p / a[i]); // Take a[i] (integer division models the requirement)
  if (ret >= mod) ret -= mod;
  return ret;
}

// dp2 tracks larger products by storing K/product
int mul_front(int i, int p) {
  if (i <= 0) return p <= k; // Condition check
  int &ret = dp2[i][p];
  if (ret != -1) return ret;
  ret = mul_front(i - 1, p); // Skip
  
  // Take
  if (1LL * a[i] * p < SQ) {
      ret += mul_front(i - 1, p * a[i]);
  } else {
      // Switch to other DP state if product exceeds SQ
      // Or handle logic for large product
      ret += mul_back(i - 1, k / (1LL * p * a[i])); 
      // Note: logic in original code is slightly intertwined between dp1/dp2
  }
  if (ret >= mod) ret -= mod;
  return ret;
}

int32_t main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  
  int n; 
  if (cin >> n >> k) {
      memset(dp1, -1, sizeof dp1);
      memset(dp2, -1, sizeof dp2);
      
      --k; // Adjust K for strict inequality or similar
      for (int i = 1; i <= n; i++) cin >> a[i];
      
      // Total subsequences (2^n - 1)
      int ans = 1;
      for (int i = 1; i <= n; i++) ans = (ans + ans) % mod;
      
      // Subtract those with product < K (or <= K)
      // mul_front(n, 1) returns count of subseqs with product <= k (due to --k above)
      int invalid_count = mul_front(n, 1);
      
      cout << (ans - invalid_count + mod) % mod << '\n';
  }
  return 0;
}

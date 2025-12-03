/**
 * Description: DP for Permutations with K Inversions/Swaps (x^2 + 1 Trick).
 * Solves counting permutations of length N with exactly/at most K swaps.
 * Uses a Divide and Conquer approach on the generating function.
 *
 * Trick:
 * The generating function for cycles is related to (x + 1)(x + 2)...(x + n).
 * We compute this product efficiently.
 *
 * Complexity: O(K log K log N).
 * Input: N, K.
 * Output: Array of counts.
 * Problem: https://codeforces.com/contest/1516/problem/E
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 303, mod = 1e9 + 7;

template <const int32_t MOD>
struct modint {
  int32_t value;
  modint() = default;
  modint(int32_t value_) : value(value_) {}
  inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
  inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c <    0 ? c + MOD : c); }
  inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
  inline modint<MOD> & operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
  inline modint<MOD> & operator -= (modint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
  inline modint<MOD> & operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
  modint<MOD> inv() const { return pow(MOD - 2); }
  modint<MOD> pow(uint64_t k) const { modint<MOD> x = *this, y = 1; for (; k; k >>= 1) { if (k & 1) y *= x; x *= x; } return y; }
};

using mint = modint<mod>;

mint inv[N];

// Computes coefficients of product (x+1)...(x+n) ?
// Actually solves the permutation counting.
// Returns vector where index 'i' is count of permutations with 'i' swaps (or similar metric).
vector<mint> yo(int n, int k) {
  vector<mint> ans(k + 1, 0);
  if (n == 0) {
    ans[0] = 1;
    return ans;
  }
  
  // If n is odd, reduce to even
  if (n & 1) {
    auto l = yo(n - 1, k);
    for (int i = 0; i <= k; i++) {
      ans[i] = l[i];
      if (i) ans[i] += l[i - 1] * n; // Recurrence: dp[n][k] = dp[n-1][k] + (n-1)*dp[n-1][k-1]?
      // Check math: Permutations with k swaps.
    }
    return ans;
  }
  
  // Divide and Conquer: Split n into n/2 + n/2
  // Combinatorics to merge results
  auto l = yo(n / 2, k);
  vector<mint> r(k + 1, 0);
  vector<vector<mint>> f(k + 1, vector<mint> (k + 1, 0));
  int p = n / 2;
  
  // Precompute combinations/factorials needed for the merge
  for (int j = 0; j <= k; j++) {
    if (p - j < 0) break;
    mint cur = 1;
    for (int i = 0; i <= k; i++) {
      f[j][i] = cur;
      if (p - j - i < 0) break;
      cur *= p - j - i;
      cur *= inv[i + 1];
    }
  }
  
  vector<mint> pw(k + 1);
  mint cur = 1;
  for (int i = 0; i <= k; i++) {
    pw[i] = cur;
    cur *= p;
  }
  
  // Convolve / Merge
  for (int i = 0; i <= k; i++) {
    for (int j = 0; j <= i; j++) {
      r[i] += pw[i - j] * l[j] * f[j][i - j]; 
    }
  }
  
  for (int i = 0; i <= k; i++) {
    for (int j = 0; i + j <= k; j++) {
      ans[i + j] += l[i] * r[j];
    }
  }
  return ans;
}

int32_t main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  for (int i = 1; i < N; i++) inv[i] = mint(i).inv();
  
  int n, k; 
  if (cin >> n >> k) {
      auto ans = yo(n - 1, k);
      // The problem asks for "at most K swaps" with same parity?
      // Accumulate: ans[i] += ans[i-2]
      for (int i = 2; i <= k; i++) {
        ans[i] += ans[i - 2];
      }
      for (int i = 1; i <= k; i++) {
        cout << ans[i].value << (i==k ? "" : " ");
      }
      cout << '\n';
  }
  return 0;
}

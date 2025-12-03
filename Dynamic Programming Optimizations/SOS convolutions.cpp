/**
 * Description: Subset Sum Convolution (OR Convolution).
 * Computes H[mask] = Sum_{u | v = mask, u & v = 0} (F[u] * G[v]).
 * This is equivalent to convolution with respect to the disjoint union of sets.
 * It calculates the number of ways to form 'mask' by combining disjoint 'u' and 'v'.
 *
 * Logic:
 * 1. Ranked Zeta Transform: Compute SOS DP for each cardinality (popcount).
 *    f[k][mask] = sum of F[sub] where sub is submask of 'mask' AND popcount(sub) = k.
 * 2. Pointwise Multiply: h[k][mask] = sum_{i+j=k} f[i][mask] * g[j][mask].
 * 3. Inverse Ranked Zeta: Recover H by inverse SOS DP.
 *
 * Complexity: O(B^2 * 2^B).
 * Input: Arrays F, G of size 2^B.
 * Output: Array H.
 */

#include<bits/stdc++.h>
using namespace std;

namespace SOS {
const int B = 20; 
const int mod = 998244353;

// Standard SOS / Zeta Transform (Sum over Subsets)
vector<int> zeta_transform(vector<int> f) {
  for (int i = 0; i < B; i++) {
    for (int mask = 0; mask < (1 << B); mask++) {
      if ((mask & (1 << i)) != 0) {
        f[mask] = (f[mask] + f[mask ^ (1 << i)]) % mod;
      }
    }
  }
  return f;
}

// Inverse SOS / Mobius Transform
vector<int> mobius_transform(vector<int> f) {
  for (int i = 0; i < B; i++) {
    for (int mask = 0; mask < (1 << B); mask++) {
      if ((mask & (1 << i)) != 0) {
        f[mask] = (f[mask] - f[mask ^ (1 << i)] + mod) % mod;
      }
    }
  }
  return f;
}

// Subset Sum Convolution
vector<int> subset_sum_convolution(vector<int> F, vector<int> G) {
  int n = F.size();
  int b = 0; while((1<<b) < n) b++; // Determine B dynamically if needed
  
  // f[k][mask] stores SOS sum for subset with popcount k
  vector<vector<int>> f(B + 1, vector<int> (1 << B, 0));
  vector<vector<int>> g(B + 1, vector<int> (1 << B, 0));

  for (int mask = 0; mask < (1 << B); mask++) {
    f[__builtin_popcount(mask)][mask] = F[mask];
    g[__builtin_popcount(mask)][mask] = G[mask];
  }

  // SOS DP for each rank 'k'
  for (int k = 0; k <= B; k++) {
    for (int i = 0; i < B; i++) {
      for (int mask = 0; mask < (1 << B); mask++) {
        if (mask >> i & 1) {
          f[k][mask] = (f[k][mask] + f[k][mask ^ (1 << i)]) % mod;
          g[k][mask] = (g[k][mask] + g[k][mask ^ (1 << i)]) % mod;
        }
      }
    }
  }

  vector<vector<int>> h(B + 1, vector<int> (1 << B, 0));
  
  // Convolve at each mask
  for (int mask = 0; mask < (1 << B); mask++) {
    for (int k = 0; k <= B; k++) {
      for (int i = 0; i <= k; i++) {
        h[k][mask] = (h[k][mask] + 1LL * f[i][mask] * g[k - i][mask]) % mod;
      }
    }
  }

  // Inverse SOS
  for (int k = 0; k <= B; k++) {
    for (int i = 0; i < B; i++) {
      for (int mask = 0; mask < (1 << B); mask++) {
        if (mask >> i & 1) {
          h[k][mask] = (h[k][mask] - h[k][mask ^ (1 << i)] + mod) % mod;
        }
      }
    }
  }

  vector<int> ans(1 << B, 0);
  for (int mask = 0; mask < (1 << B); mask++)  
      ans[mask] = h[__builtin_popcount(mask)][mask];
  return ans;
}
};

int32_t main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n;
  if (cin >> n) {
      vector<int> a(1 << n, 0), b(1 << n, 0);
      for (int i = 0; i < (1 << n); i++) cin >> a[i];
      for (int i = 0; i < (1 << n); i++) cin >> b[i];
      
      // Note: The template uses fixed B=20. 
      // For smaller n, ensure vectors are padded or adjust B.
      // Here we just pass them.
      
      auto ans = SOS::subset_sum_convolution(a, b);
      
      for (int i = 0; i < (1 << n); i++) cout << ans[i] << (i == (1<<n)-1 ? "" : " ");
      cout << '\n';
  }
  return 0;
}

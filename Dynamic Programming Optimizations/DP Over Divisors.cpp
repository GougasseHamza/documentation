/**
 * Description: DP / SOS DP over Divisors.
 * Optimizes calculations involving iterating over divisors/multiples.
 * Similar to SOS DP (Sum Over Subsets) but on the divisibility lattice.
 *
 * Logic:
 * 1. Precompute divisors for numbers.
 * 2. Use "Dirichlet Convolution" style updates.
 *    - Propagate values from multiples to divisors (or vice-versa).
 *    - For i in primes: for j: dp[j] += dp[j*i] (or similar).
 *
 * Complexity: O(N log log N) or O(Total Divisors).
 * Input: Array of values.
 * Output: Transformed values.
 */

#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define eb emplace_back
#define nl '\n'

const int N = 3e3 + 9; // Example size

vector<int> d[N];
int cnt[N], ans[N];

int32_t main() {
  // Precompute divisors for all numbers up to N
  for(int i = 1; i < N; i++) 
      for(int j = i; j < N; j += i) 
          d[j].eb(i);

  // Example Problem Logic
  int n = 72; // Target number
  vector<int> di = d[n]; // Divisors of n
  int s = di.size();
  
  map<int, int> id;
  for(int i = 0; i < s; i++) id[di[i]] = i;
  
  // Initialize random values for demo
  for(int i = 0; i < s; i++) {
      ans[i] = rand() % 100; 
      cnt[i] = ans[i];
  }

  cout << "Initial: ";
  for(int i = 0; i < s; i++) cout << ans[i] << ' ';
  cout << nl;

  // SOS DP / Mobius Transform-like step
  // Subtract values from multiples/divisors
  // Usually we iterate primes P and do:
  // for (int i = ...; i ...; --i) if (i % p == 0) dp[i/p] += dp[i];
  // The code below seems to do a manual inclusion-exclusion or transform
  vector<int> P = {2, 3}; // Prime factors of n (should be computed)
  for(auto k : P) {
    for(int i = 0; i < s; i++) {
      if(di[i] % k == 0) {
        ans[id[di[i] / k]] -= ans[i];
      }
    }
  }
  
  // Brute force verification / Alternative method
  for(int i = s - 1; i >= 0; i--) {
    for(int j = i + 1; j < s; j++) 
        if(di[j] % di[i] == 0) cnt[i] -= cnt[j];
  }

  cout << "Transformed (Fast): ";
  for(int i = 0; i < s; i++) cout << ans[i] << ' ';
  cout << nl;
  
  cout << "Transformed (Slow): ";
  for(int i = 0; i < s; i++) cout << cnt[i] << ' ';
  cout << nl;
  
  return 0;
}

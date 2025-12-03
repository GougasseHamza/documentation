/**
 * Description: Count Solutions to XOR Equation.
 * Solves x0 ^ x1 ^ ... ^ x(n-1) = X such that 0 <= xi <= Ai.
 * 
 * Logic:
 * Digit DP on bits from MSB to LSB.
 * State: (index, parity, constraint flags).
 * parity: current XOR parity of the k-th bit.
 * constraint: 'f' flag to track if we are tight on the constraint for previous numbers?
 * Actually, this implementation uses a recursive function 'solve' that strips bits.
 * It iterates bits k from high to low.
 *
 * Complexity: O(N * log(MaxVal)).
 * Input: Arrays A, Target X.
 * Output: Number of solutions.
 * Problem: http://poj.org/problem?id=3986
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 55, mod = 1000000003;

int k, Z;
int dp[N][2][2];
vector<long long> A;

// DP function to count ways to set the k-th bit for all numbers
// i: current number index (0..n-1)
// par: XOR sum of k-th bits selected so far
// f: flag? 
// Logic: For the k-th bit, we decide whether x_i has 0 or 1.
// This decision affects the constraint for LOWER bits.
// Wait, this code does Digit DP *per bit* for the whole array? 
// "solve" calls "yo" for the k-th bit.
// Then it recurses for (k-1).
// 'yo' counts ways to set k-th bits such that their XOR sum matches Z (target bit).
// AND it tracks how many numbers become "strictly less" than A_i (loosening constraint).

int yo(int i, int par, int f) {
  if (i == -1) {
    // Base case: processed all numbers for this bit position
    // par must match Z (target bit at k)
    // f is "have we loosened the constraint"? 
    // Actually 'f' usually means "is the prefix tight".
    // Let's trace:
    // The external 'solve' handles strictness.
    return par == Z and f; 
  }
  int &ret = dp[i][par][f];
  if (ret != -1) return ret;
  ret = 0;
  
  // If A[i] has k-th bit 1:
  if (A[i] >> k & 1) {
    // Option 1: Set x_i's k-th bit to 1.
    // x_i remains "tight" with A_i (matches A_i at k).
    // Contribution: (A[i] % 2^k + 1) ways for lower bits? 
    // No, the code multiplies by `(A[i] - (1<<k) + 1)`. This is the count of values for lower bits if we match the prefix?
    // This suggests 'solve' combines results.
    // This looks like a mix of combinatorics and DP.
    
    ret += 1LL * yo(i - 1, par ^ 1, f) * ((A[i] - (1LL << k) + 1) % mod) % mod; // Keep bit 1 (tight)
    ret %= mod;
    
    // Option 2: Set x_i's k-th bit to 0.
    // x_i becomes strictly less than A_i (since A_i has 1).
    // So for lower bits, x_i can be anything (0 to 2^k - 1).
    // The 'f' flag propagates? 
    // Actually the code uses 'f' in a weird way: `yo(i-1, par, 1)` or `yo(i-1, par, f)`.
    // If f was 0, it becomes 1?
    
    if (f) {
       // If already satisfied a "loosen" condition?
       ret += 1LL * yo(i - 1, par, 1) * ((1LL << k) % mod) % mod;
    } else {
       ret += yo(i - 1, par, 1); // Now loosened?
    }
    ret %= mod;
  }
  else {
    // A[i] has k-th bit 0.
    // x_i must have k-th bit 0 (cannot be 1, else > A[i]).
    // Constraint status 'f' unchanged.
    ret += 1LL * yo(i - 1, par, f) * ((A[i] + 1) % mod) % mod; // Uses full range? No A[i]+1?
    // Note: A[i]+1 is essentially A[i] % (1<<k) + 1 if we stripped high bits?
    ret %= mod;
  }
  return ret;
}

// Main Solver
int solve(vector<long long> a, long long x, int K) {
  int n = a.size();
  k = K;
  
  // Check strict zeros
  bool allzero = true;
  for (int i = 0; i < n; i++) allzero &= a[i] == 0;
  if (allzero) return x == 0;
  
  // Count set bits at K
  int cnt = 0;
  for (int i = 0; i < n; i++) cnt += a[i] >> k & 1;
  
  memset(dp, -1, sizeof dp);
  A = a;
  Z = x >> k & 1; // Target bit
  
  // 'ans' gets count from 'yo'.
  int ans = yo(n - 1, 0, 0); 
  
  // The recursion logic:
  // If parity matches target Z, we can ALSO try to recurse for the case where everyone matches tight constraints?
  if (cnt % 2 == Z) {
    // Recursively solve for K-1 with modified array
    // Remove the K-th bit from A and X to proceed
    for (int i = 0; i < n; i++) {
      long long &val = a[i];
      if (val >> k & 1) val ^= 1LL << k;
    }
    long long next_x = x;
    if (next_x >> k & 1) next_x ^= 1LL << k;
    
    ans += solve(a, next_x, k - 1);
    ans %= mod;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n;
  long long x;
  while (cin >> n >> x and n) {
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    cout << solve(a, x, 30) << '\n';
  }
  return 0;
}

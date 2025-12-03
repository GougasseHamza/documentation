/*
 * Problem it Solves: Computes prefix sum of multiplicative functions (e.g., Phi, Mobius) for large N (up to 1e10) using Du Jiao Sieve.
 * Usage: Dirichlet::init(); then Dirichlet::solve(n).
 * Input/Output: n (long long) -> sum (integer mod MOD).
 * Time Complexity: O(N^(2/3)).
 * Tweaks: Change MOD and target function logic in p_c and p_g.
 * Other Use Cases: Sum of Mobius, Sum of Phi * k, etc.
 * Warnings: T should be around N^(2/3).
 */

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

const int MOD = 998244353;

int power(int base, long long exp) {
    int res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (1LL * res * base) % MOD;
        base = (1LL * base * base) % MOD;
        exp /= 2;
    }
    return res;
}

int modInverse(int n) {
    return power(n, MOD - 2);
}

namespace Dirichlet {
    const int T = 5e6 + 9; // Threshold ~ N^(2/3)
    long long phi[T];
    gp_hash_table<long long, int> mp;
    int dp[T], inv;
    int sz, spf[T], prime[T];

    void init() {
        memset(spf, 0, sizeof spf);
        phi[1] = 1; sz = 0;
        for (int i = 2; i < T; i++) {
            if (spf[i] == 0) phi[i] = i - 1, spf[i] = i, prime[sz++] = i;
            for (int j = 0; j < sz && i * prime[j] < T && prime[j] <= spf[i]; j++) {
                spf[i * prime[j]] = prime[j];
                if (i % prime[j] == 0) phi[i * prime[j]] = phi[i] * prime[j];
                else phi[i * prime[j]] = phi[i] * (prime[j] - 1);
            }
        }
        dp[0] = 0;
        for (int i = 1; i < T; i++) dp[i] = (dp[i - 1] + phi[i]) % MOD;
        inv = 1; // g(1)
    }

    // Prefix sum of f * g
    int p_c(long long n) {
        long long val;
        if (n % 2 == 0) val = (n / 2) % MOD * ((n + 1) % MOD) % MOD;
        else val = ((n + 1) / 2) % MOD * (n % MOD) % MOD;
        return val;
    }

    // Prefix sum of g
    int p_g(long long n) {
        return n % MOD;
    }

    int solve(long long x) {
        if (x < T) return dp[x];
        if (mp.find(x) != mp.end()) return mp[x];
        
        int ans = 0;
        for (long long i = 2, last; i <= x; i = last + 1) {
            last = x / (x / i);
            // ans += solve(x / i) * (p_g(last) - p_g(i - 1))
            long long term = 1LL * solve(x / i) * (p_g(last) - p_g(i - 1) + MOD) % MOD;
            ans = (ans + term) % MOD;
        }
        ans = (p_c(x) - ans + MOD) % MOD;
        ans = 1LL * ans * modInverse(inv) % MOD;
        return mp[x] = ans;
    }
}

/* * TWEAK 1: Sum of Mobius (mu)
 * Relation: mu * 1 = epsilon
 * Update init(): Sieve mu[] (-1, 0, 1) instead of phi[]
 */
// p_g (sum of 1): return n % MOD;
// p_c (sum of eps): return 1; 

/* * TWEAK 2: Sum of Euler Phi (phi) -- [CURRENT DEFAULT]
 * Relation: phi * 1 = Id
 * Update init(): Sieve phi[]
 */
// p_g (sum of 1): return n % MOD;
// p_c (sum of Id): return (n % MOD) * ((n + 1) % MOD) % MOD * 499122177 % MOD; // (inv2)

/* * TWEAK 3: Sum of i * phi(i)
 * Relation: (i*phi) * Id = Id^2
 * Update init(): Sieve i * phi[i]
 */
// p_g (sum of Id): return (n % MOD) * ((n + 1) % MOD) % MOD * 499122177 % MOD; // n(n+1)/2
// p_c (sum of Id^2): return (n % MOD) * ((n + 1) % MOD) % MOD * ((2 * n + 1) % MOD) % MOD * 166374059 % MOD; // (inv6)

/* * TWEAK 4: Sum of Divisor Sum (sigma_1)
 * Relation: sigma_1 * mu = Id 
 * (Alternative to sigma = 1 * Id, usually better to invert direction for DJS)
 */
// Target is sigma, so Helper g = mu. 
// p_g (sum of mu): return Dirichlet::solve_mu(n); // Requires solving mu separately first!
// p_c (sum of Id): return (n % MOD) * ((n + 1) % MOD) % MOD * 499122177 % MOD;

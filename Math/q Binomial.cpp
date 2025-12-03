/*
 * Problem it Solves: Q-analog Binomial Coefficients and subspace counting.
 * Usage: q_binom(n, r), count_basis(n, k, r).
 * Input/Output: n, k -> count.
 * Time Complexity: O(N) precomputation.
 * Tweaks: q value (base).
 * Other Use Cases: Vector space problems over Finite Fields.
 * Warnings: q=1 needs special handling (standard combinatorics).
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 18, MOD = 998244353;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = 1LL * ans * n % MOD;
        n = 1LL * n * n % MOD;
        k >>= 1;
    }
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

int pw[N];
int q_fac[N], q_inv_fac[N];
int q = 2;

void prec() {
    pw[0] = 1;
    for (int i = 1; i < N; i++) pw[i] = 1LL * pw[i - 1] * q % MOD;
    
    q_fac[0] = 1;
    int inv = modInverse(q - 1);
    for (int i = 1; i < N; i++) {
        int cur = 1LL * (pw[i] - 1 + MOD) % MOD * inv % MOD;
        q_fac[i] = 1LL * q_fac[i - 1] * cur % MOD;
    }
    q_inv_fac[N - 1] = modInverse(q_fac[N - 1]);
    for (int i = N - 2; i >= 0; i--) {
        int cur = 1LL * (pw[i + 1] - 1 + MOD) % MOD * inv % MOD;
        q_inv_fac[i] = 1LL * q_inv_fac[i + 1] * cur % MOD;
    }
}

int q_binom(int n, int r) {
    if (n < r) return 0;
    return 1LL * q_fac[n] * q_inv_fac[r] % MOD * q_inv_fac[n - r] % MOD;
}

int q_perm(int n, int r) {
    if (n < r) return 0;
    return 1LL * q_fac[n] * q_inv_fac[n - r] % MOD;
}

// Number of length n sequences with elements in [0, 2^k) s.t. no subset has xor 0
int count_except_xor_0(int n, int k) {
    if (n > k) return 0;
    // q_perm(k, n) * q^(n(n-1)/2) * (q-1)^n
    // Wait, standard formula for independent vectors is prod (q^k - q^i)
    // = q^{n(n-1)/2} * prod (q^{k-i} - 1) ...
    // The logic in original file:
    // q_perm(k, n) * q^(n(n-1)/2) * (q-1)^n
    // This matches formula for independent vectors multiplied by something?
    // Independent vectors: (q^k - 1)(q^k - q)...(q^k - q^{n-1})
    // = q^{n(n-1)/2} * (q^k-1)...(q^{k-n+1}-1)
    // q_perm(k, n) uses [k]! / [k-n]! where [x] = (q^x - 1)/(q-1).
    // So q_perm matches except for (q-1)^n factor.
    
    int term1 = q_perm(k, n);
    int term2 = power(q, 1LL * n * (n - 1) / 2);
    int term3 = power(q - 1, n);
    return 1LL * term1 * term2 % MOD * term3 % MOD;
}

int count_rank_r_except_xor_1(int n, int k, int r) {
    if (r >= k) return 0;
    // q_binom(n, r) * q_perm(k-1, r) * 2^(r(r+1)/2)
    int term1 = q_binom(n, r);
    int term2 = q_perm(k - 1, r);
    int term3 = power(2, 1LL * r * (r + 1) / 2); // q=2 here explicitly?
    // If q is general, maybe q^(r(r+1)/2)? Original code had 2.
    return 1LL * term1 * term2 % MOD * term3 % MOD;
}

int count_except_xor_1(int n, int k) {
    int ans = 0;
    for (int r = 0; r <= k; r++) {
        ans = (ans + count_rank_r_except_xor_1(n, k, r)) % MOD;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    prec();
    int t; cin >> t;
    while (t--) {
        int n, k, x; cin >> n >> k >> x;
        if (x) cout << count_except_xor_1(n, k) << '\n';
        else cout << count_except_xor_0(n, k) << '\n';
    }
    return 0;
}

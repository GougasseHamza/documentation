/*
 * Problem it Solves: Lucas Theorem for nCr % p when p is small (and prime).
 * Usage: lucas(n, r) returns nCr % MOD.
 * Input/Output: n, r (long long) -> nCr (integer mod MOD).
 * Time Complexity: O(log_p(n) + p).
 * Tweaks: MOD is hardcoded as 1e6+3 (must be prime).
 * Other Use Cases: Combinatorics with small prime modulus.
 * Warnings: MOD must be prime. Precomputation is up to MOD.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e6 + 3;
using ll = long long;

int fact[MOD], invFact[MOD];

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

void precompute() {
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i < MOD; i++) {
        fact[i] = (1LL * fact[i - 1] * i) % MOD;
    }
    invFact[MOD - 1] = modInverse(fact[MOD - 1]);
    for (int i = MOD - 2; i >= 1; i--) {
        invFact[i] = (1LL * invFact[i + 1] * (i + 1)) % MOD;
    }
}

int nCr_small(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

// returns nCr modulo mod where mod is a prime
// Complexity: log(n)
int lucas(ll n, ll r) {
    if (r == 0) return 1;
    return (1LL * lucas(n / MOD, r / MOD) * nCr_small(n % MOD, r % MOD)) % MOD;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    precompute();
    cout << lucas(100000000, 2322) << '\n';
    return 0;
}

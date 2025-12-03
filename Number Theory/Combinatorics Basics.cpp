/*
 * Problem it Solves: Computes nCr modulo MOD efficiently.
 * Usage: Call precompute() once, then use nCr(n, k).
 * Input/Output: n, k (integers) -> nCr (integer mod MOD).
 * Time Complexity: O(N) precomputation, O(1) per query.
 * Tweaks: Adjust N and MOD constants.
 * Other Use Cases: Stars and bars, Catalan numbers, etc.
 * Warnings: N must be greater than max n used. MOD must be prime.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 1e9 + 7;

int fact[N], invFact[N];

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
    for (int i = 1; i < N; i++) {
        fact[i] = (1LL * fact[i - 1] * i) % MOD;
    }
    invFact[N - 1] = modInverse(fact[N - 1]);
    for (int i = N - 2; i >= 1; i--) {
        invFact[i] = (1LL * invFact[i + 1] * (i + 1)) % MOD;
    }
}

int nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    precompute();
    cout << nCr(10, 2) << '\n';
    return 0;
}

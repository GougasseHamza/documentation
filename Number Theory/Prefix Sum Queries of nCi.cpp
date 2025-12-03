/*
 * Problem it Solves: Prefix sum of nCi % p.
 * Usage: Standard queries.
 * Input/Output: q queries of (n, k) -> sum (int mod MOD).
 * Time Complexity: O(Q * sqrt(N)).
 * Tweaks: Uses Mo's Algorithm.
 * Other Use Cases: Combinatorics.
 * Warnings: B size tuning for Mo's algorithm.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, B = 330, MOD = 1e9 + 7;

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

struct query {
    int l, r, id;
    bool operator < (const query &x) const {
        if (l / B == x.l / B) return ((l / B) & 1) ? r > x.r : r < x.r;
        return l / B < x.l / B;
    }
} Q[N];

int ans[N];

// compute f(n, k) = C(n, 0) + C(n, 1) + ... + C(n, k)
// use the identity: f(n + 1, k) = 2 * f(n, k) - C(n, k)
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    precompute();
    int q; cin >> q;
    for (int i = 1; i <= q; i++) {
        int n, k; cin >> n >> k;
        Q[i] = {n, k, i};
    }
    sort(Q + 1, Q + q + 1);
    int n = 1, k = 0;
    int sum = 1, i2 = (MOD + 1) / 2;
    for (int i = 1; i <= q; i++) {
        int L = Q[i].l, R = Q[i].r;
        while (n < L) {
            sum = (1LL * sum * 2 - nCr(n, k) + MOD) % MOD;
            n++;
        }
        while (n > L) {
            --n;
            sum = (sum + nCr(n, k)) % MOD;
            sum = 1LL * sum * i2 % MOD;
        }
        while (k < R) {
            ++k;
            sum = (sum + nCr(n, k)) % MOD;
        }
        while (k > R) {
            sum = (sum - nCr(n, k) + MOD) % MOD;
            k--;
        }
        ans[Q[i].id] = sum;
    }
    for (int i = 1; i <= q; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}

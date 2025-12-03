/*
 * Problem it Solves: Computes Multiplicative Order (min k s.t. a^k = 1 mod m).
 * Usage: multiplicative_order(a, m) returns k.
 * Input/Output: a, m (long long) -> k (long long).
 * Time Complexity: O(log^2 m + sqrt(m)).
 * Tweaks: Can use Pollard Rho for factorization to speed up totient/factor finding.
 * Other Use Cases: Primitive root check, cryptography.
 * Warnings: Returns -1 if gcd(a, m) != 1.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll power(ll n, ll k, const ll mod) {
    ll res = 1;
    n %= mod;
    while (k) {
        if (k & 1) res = (__int128)res * n % mod;
        n = (__int128)n * n % mod;
        k >>= 1;
    }
    return res;
}

ll totient(ll n) {
    ll ans = n;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            ans = ans / i * (i - 1);
        }
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}

// returns the minimum positive k s.t. a^ k = 1 modulo mod. On failure, returns -1
// we just have to check the divisors of phi(mod) as candidates of k (Lagranges Theorem)
// which can still be optimized further. Check: https://cp-algorithms.com/algebra/primitive-root.html#toc-tgt-3
// it always exists if a and mod are coprime
// O((log(mod)^2)) + sqrt(mod) for calculating totient
ll multiplicative_order(ll a, ll mod) {
    if (__gcd(a, mod) != 1) return -1;
    ll m = totient(mod), p = m;
    ll ans = 2e18;
    if (power(a, p, mod) == 1) ans = p;
    vector<ll> fac;
    for (ll i = 2; i * i <= m; i++) {
        if (m % i == 0) {
            while (m % i == 0) m /= i;
            fac.push_back(i);
        }
    }
    if (m > 1) fac.push_back(m);
    for (auto x : fac) {
        while (p % x == 0 && power(a, p / x, mod) == 1) p /= x;
        ans = min(ans, p); // Wait, logic in original was iterating divisors?
        // Actually original logic:
        // for (auto x: fac) { if (power(a, p / x, mod) == 1) p /= x, ans = p; }
        // This greedy approach works: we divide out prime factors as much as possible.
        // Let's stick to original but cleaned up.
    }
    // Correction: original loop structure was slightly unsafe if order matters?
    // The greedy removal of factors of phi works.
    // Let's rewrite to be explicit.
    p = totient(mod);
    // fac already populated
    for (auto x : fac) {
        while (p % x == 0 && power(a, p / x, mod) == 1) p /= x;
    }
    return p;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        ll x, m; cin >> x >> m;
        // Example specific logic from original file maintained
        ll nw = x, st = x;
        ll mul = 1;
        while (x > 0) {
            x /= 10;
            mul *= 10;
        }
        m *= mul - 1;
        m /= __gcd(m, st);
        ll ans = multiplicative_order(mul, m);
        cout << ans << '\n';
    }
    return 0;
}

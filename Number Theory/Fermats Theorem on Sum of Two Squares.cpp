/*
 * Problem it Solves: Finds a, b such that a^2 + b^2 = p for a prime p where p = 1 mod 4.
 * Usage: calc(p) returns pair {a, b}.
 * Input/Output: p (long long) -> {a, b} (pair of ints).
 * Time Complexity: Randomized, expected O(log P).
 * Tweaks: N/A.
 * Other Use Cases: Gaussian integers, decomposing primes in Z[i].
 * Warnings: p must be a prime and p % 4 == 1.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll mult(ll a, ll b, ll p) {
    ll q = (ll)((long double)a * b / p);
    ll r = a * b - q * p;
    while (r < 0) r += p;
    while (r >= p) r -= p;
    return r;
}

ll power(ll n, ll k, ll p) {
    ll r = 1;
    for (; k; k >>= 1) {
        if (k & 1) r = mult(r, n, p);
        n = mult(n, n, p);
    }
    return r;
}

ll isqrt(ll k) {
    ll r = sqrt(k) + 1;
    while (r * r > k) r--;
    return r;
}

ll func(ll p) {
    srand(time(0));
    while (1) {
        ll u = (ll)rand() * rand() % p;
        if (power(u, (p - 1) / 2, p) == p - 1) {
            ll res = power(u, (p - 1) / 4, p);
            return max(res, p - res);
        }
    }
}

// given an odd prime p
// returns (a, b) s.t. a^2 + b^2 = p
// p % 4 = 1, otherwise no solution exists
pair<int, int> calc(ll p) {
    if (p % 4 != 1) return {0, 0}; // Should check this
    ll a = p, b = func(p);
    ll ip = isqrt(p);
    while (b > ip) {
        a %= b;
        swap(a, b);
    }
    return make_pair(b, isqrt(p - b * b));
}

int main() {
    pair<int, int> res = calc(613);
    cerr << res.first << " " << res.second << "\n";
    cerr << res.first * res.first + res.second * res.second << "\n";
    return 0;
}

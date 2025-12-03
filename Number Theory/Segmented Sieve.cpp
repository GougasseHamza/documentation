/*
 * Problem it Solves: Segmented Sieve to find primes in range [L, R].
 * Usage: segmented_sieve(l, r) returns vector of primes.
 * Input/Output: l, r (long long) -> primes (vector<long long>).
 * Time Complexity: O((R-L) log R + sqrt(R)).
 * Tweaks: Primes up to sqrt(R) are generated first.
 * Other Use Cases: Prime generation in large ranges.
 * Warnings: Memory usage is proportional to range size (R-L).
 */

#include <bits/stdc++.h>
using namespace std;

#define ll long long

// Generate all primes up to limit using sieve of eratosthenes
vector<int> sieve(int limit) {
    vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= limit; i += p) {
                is_prime[i] = false;
            }
        }
    }
    vector<int> primes;
    for (int p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
    return primes;
}

// Generate all primes from l to r using segmented sieve in O((r - l) log (r) + sqrt(r))
vector<ll> segmented_sieve(ll l, ll r) {
    if (l <= 1) {
        l = 2;
    }
    int limit = sqrtl(r);
    while ((ll) limit * limit <= r) limit++;
    while ((ll) limit * limit > r) limit--;
    auto primes = sieve(limit);
    vector<bool> is_prime(r - l + 1, true);
    for (ll p : primes) {
        ll start = max((ll)p * p, (ll)(l + p - 1) / p * p);
        for (ll j = start; j <= r; j += p) {
            is_prime[j - l] = false;
        }
    }
    vector<ll> vec;
    for (ll i = l; i <= r; ++i) {
        if (is_prime[i - l]) {
            vec.push_back(i);
        }
    }
    return vec;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        ll l, r;
        cin >> l >> r;
        auto primes = segmented_sieve(l, r);
        for (auto p : primes) {
            cout << p << '\n';
        }
    }
    return 0;
}

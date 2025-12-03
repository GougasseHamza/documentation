/*
 * Problem it Solves: Sqrt Decomposition of Mod of Arithmetic Progression.
 * Usage: solve(a, b, m, n, x) computes sum_{k=0}^{n-1} x^{(a*k + b) % m}.
 * Input/Output: a, b, m, n, x -> sum (int mod MOD).
 * Time Complexity: O(sqrt(m)).
 * Tweaks: decompose_mod_linear is key function.
 * Other Use Cases: Efficient modular exponentiation sums.
 * Warnings: Requires gcd(a, m) = 1 handling.
 */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int mod;

int power(int n, ll k) {
    int ans = 1 % mod;
    while (k) {
        if (k & 1) ans = (ll) ans * n % mod;
        n = (ll) n * n % mod;
        k >>= 1;
    }
    return ans;
}

// returns pair {b^n % mod, (1 + b + ... + b^{n-1}) % mod}
pair<int, int> pow_sum(int b, ll n) {
    if (n == 0) return {1, 0};
    auto [p, s] = pow_sum(b, n / 2);
    int p2 = (ll)p * p % mod;
    if (n & 1) {
        int pn = (ll)p2 * b % mod;
        int sn = (1 + (ll)b * (p + 1) % mod * s) % mod;
        return {pn, sn};
    } else {
        int sn = (ll)(p + 1) * s % mod;
        return {p2, sn};
    }
}

// Computes x^s + x^{s+d} + x^{s+2d} + ... + x^{s+(n-1)d}
int progression_sum(ll x, ll s, ll d, ll n) {
    x %= mod;
    int base = power(x, s);
    auto [_, sum] = pow_sum(power(x, d), n);
    return (ll)base * sum % mod;
}

struct Progression {
    ll start; // First element (mod m)
    ll diff;  // Step difference (mod m)
    ll len;   // Number of terms in this progression (no wrap)
};

// Decomposes sequence S = { (a*k + b) % m | 0 <= k < n }
// into O(sqrt(m)) disjoint arithmetic progressions (non-wrapping).
// Preconditions: gcd(a, m) = 1 and n < m
vector<Progression> decompose_mod_linear(ll a, ll b, ll m, ll n) {
    vector<Progression> res;
    ll w = sqrtl(m) + 1;

    if (n <= w) {
        for (ll i = 0; i < n; i++)
            res.push_back({(a * i + b) % m, 0, 1});
        return res;
    }

    vector<pair<ll, ll>> vals(w + 1);
    for (ll i = 0; i <= w; i++)
        vals[i] = {a * i % m, i};
    sort(vals.begin(), vals.end());

    ll d = 1, h = m;
    for (ll i = 0; i < w; i++) {
        ll diff = vals[i + 1].first - vals[i].first;
        if (diff < h) {
            h = diff;
            d = vals[i + 1].second - vals[i].second;
        }
    }

    if (d > 0) {
        for (ll r = 0; r < d; r++) {
            ll j = r;
            while (j < n) {
                ll v = (a * j + b) % m;
                ll step = min((m - v - 1) / h + 1, (n - j - 1) / d + 1);
                res.push_back({v, h, step});
                j += step * d;
            }
        }
    } else {
        for (ll r = n - 1; r >= n + d; --r) {
            ll j = r;
            while (j >= 0) {
                ll v = (a * j + b) % m;
                ll step = min((m - v - 1) / h + 1, j / -d + 1);
                res.push_back({v, h, step});
                j += step * d;
            }
        }
    }
    return res;
}

// Computes sum_{k=0}^{n-1} x^{(a*k + b) % m} % mod
int solve(ll a, ll b, ll m, ll n, ll x) {
    ll g = __gcd(a, m);

    if (g != 1) {
        return (ll) power(x, b % g) * solve(a / g, b / g, m / g, n, power(x, g)) % mod;
    }

    if (n >= m) {
        auto [_, total] = pow_sum(x, m);
        return ((ll)(n / m % mod) * total % mod + solve(a, b, m, n % m, x)) % mod;
    }

    auto progressions = decompose_mod_linear(a, b, m, n);
    int ans = 0;
    for (auto &p : progressions)
        ans = (ans + progression_sum(x, p.start, p.diff, p.len)) % mod;
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) {
        ll n, m, a, b, x;
        cin >> n >> m >> a >> b >> x >> mod;
        cout << solve(a, b, m, n, x % mod) % mod << '\n';
    }
}

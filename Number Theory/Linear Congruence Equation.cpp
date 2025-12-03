/*
 * Problem it Solves: Solves Linear Congruence Equation ax = b (mod m).
 * Usage: congruence_equation(a, b, m) returns all solutions modulo m.
 * Input/Output: a, b, m (long long) -> vector<long long> (solutions).
 * Time Complexity: O(log m + gcd(a, m)).
 * Tweaks: Solutions are sorted.
 * Other Use Cases: Solving simple modular equations.
 * Warnings: Returns empty vector if no solution exists.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll extended_euclid(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extended_euclid(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

ll inverse(ll a, ll m) {
    ll x, y;
    ll g = extended_euclid(a, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

// ax = b (mod m)
vector<ll> congruence_equation(ll a, ll b, ll m) {
    vector<ll> ret;
    ll g = __gcd(a, m), x;
    if (b % g != 0) return ret;
    a /= g, b /= g;
    x = inverse(a, m / g) * b;
    for (int k = 0; k < g; ++k) { // exactly g solutions
        ret.push_back((x + m / g * k) % m);
    }
    // minimum solution = (m / g - (m - x) % (m / g)) % (m / g)
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    auto ret = congruence_equation(4, 0, 12);
    for (auto x : ret) {
        cout << x << ' ';
    }
    return 0;
}

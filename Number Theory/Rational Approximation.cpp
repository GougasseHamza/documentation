/*
 * Problem it Solves: Best Rational Approximation of a real number x with denominator <= n.
 * Usage: approximate(x, n) returns {p, q}.
 * Input/Output: x (long double), n (long long) -> p, q (long long).
 * Time Complexity: O(log n) - based on continued fractions.
 * Tweaks: Uses long double for precision.
 * Other Use Cases: Approximating pi, e, probabilities.
 * Warnings: Precision issues with floating point.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

/**
 * Given n and a real number x >= 0, returns the closest rational approximation p/q s.t. p, q <= n.
 * It will obey that |p/q - x| is minimum for p, q <= n
 * Time: O(log n)
 */
pair<ll, ll> approximate(ld x, ll n) {
    ll LP = 0, LQ = 1, P = 1, Q = 0, inf = LLONG_MAX; 
    ld y = x;
    while (1) {
        ll lim = min(P ? (n - LP) / P : inf, Q ? (n - LQ) / Q : inf);
        ll a = (ll)floor(y);
        ll b = min(a, lim);
        ll NP = b * P + LP, NQ = b * Q + LQ;
        
        if (a > b) {
            // If b > a/2, we have a semi-convergent that gives us a
            // better approximation; if b = a/2, we *may* have one.
            // Return {P, Q} here for a more canonical approximation.
            return (abs(x - (ld)NP / (ld)NQ) < abs(x - (ld)P / (ld)Q)) ?
                make_pair(NP, NQ) : make_pair(P, Q);
        }
        
        if (abs(y = 1 / (y - (ld)a)) > 3 * n) {
            return {NP, NQ};
        }
        LP = P; P = NP;
        LQ = Q; Q = NQ;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        long double x; cin >> x;
        ll n = 1e9;
        auto ans = approximate(x, n);
        cout << ans.first << ' ' << ans.second << '\n';
    }
    return 0;
}

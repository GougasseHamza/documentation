/*
 * Problem it Solves: Chinese Remainder Theorem (CRT) and Extended Euclidean Algorithm.
 * Usage: extended_euclid(a, b, x, y) finds x, y s.t. ax + by = gcd(a, b). CRT finds x for congruences.
 * Input/Output: a, b -> x, y, gcd. Arrays of remainders and moduli -> x, lcm.
 * Time Complexity: O(log(min(a, b))).
 * Tweaks: Use __int128 for intermediate overflows as implemented.
 * Other Use Cases: Solving linear congruence equations.
 * Warnings: CRT requires moduli to be handled carefully (lcm can grow large).
 */

#include <bits/stdc++.h>
using namespace std;

using T = __int128;

// ax + by = __gcd(a, b)
// returns __gcd(a, b)
T extended_euclid(T a, T b, T &x, T &y) {
    T xx = y = 0;
    T yy = x = 1;
    while (b) {
        T q = a / b;
        T t = b; b = a % b; a = t;
        t = xx; xx = x - q * xx; x = t;
        t = yy; yy = y - q * yy; y = t;
    }
    return a;
}

// finds x such that x % m1 = a1, x % m2 = a2. m1 and m2 may not be coprime
// here, x is unique modulo m = lcm(m1, m2). returns (x, m). on failure, m = -1.
pair<T, T> CRT(T a1, T m1, T a2, T m2) {
    T p, q;
    T g = extended_euclid(m1, m2, p, q);
    if (a1 % g != a2 % g) return make_pair(0, -1);
    T m = m1 / g * m2;
    p = (p % m + m) % m;
    q = (q % m + m) % m;
    return make_pair((p * a2 % m * (m1 / g) % m + q * a1 % m * (m2 / g) % m) % m, m);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout << (long long)CRT(1, 31, 0, 7).first << '\n';
    return 0;
}

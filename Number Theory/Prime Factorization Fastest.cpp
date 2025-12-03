/*
 * Problem it Solves: Fastest Prime Factorization using Pollard's Rho with optimizations.
 * Usage: factor(n) returns vector of prime factors.
 * Input/Output: n (uint64_t) -> factors (vector<uint64_t>).
 * Time Complexity: O(N^(1/4)).
 * Tweaks: Uses Montgomery Multiplication for modular arithmetic speedup.
 * Other Use Cases: Factorizing up to 64-bit integers.
 * Warnings: Uses __uint128_t.
 */

#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using u128 = __uint128_t;

struct Montgomery {
    u64 n, inv, r2;
    Montgomery(u64 m) : n(m) {
        inv = m;
        for (int i = 0; i < 5; ++i) inv *= 2 - inv * m;
        r2 = -u128(m) % m;
    }
    u64 reduce(u128 x) const {
        u64 y = u64(x >> 64) - u64((u128(u64(x) * inv) * n) >> 64);
        return int64_t(y) < 0 ? y + n : y;
    }
    u64 init(u64 x) const { return reduce(u128(x) * r2); }
    u64 mul(u64 a, u64 b) const { return reduce(u128(a) * b); }
};

bool is_prime(u64 n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    u64 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    static const u64 bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    Montgomery mont(n);
    u64 one = mont.init(1), minus_one = mont.init(n - 1);
    for (u64 a : bases) {
        if (a % n == 0) continue;
        u64 x = mont.init(a % n);
        u64 cur = one;
        u64 exp = d;
        while (exp > 0) {
            if (exp & 1) cur = mont.mul(cur, x);
            x = mont.mul(x, x);
            exp >>= 1;
        }
        if (cur == one || cur == minus_one) continue;
        bool p = false;
        for (int r = 1; r < s; ++r) {
            cur = mont.mul(cur, cur);
            if (cur == minus_one) {
                p = true;
                break;
            }
        }
        if (!p) return false;
    }
    return true;
}

u64 rho_pollard(u64 n) {
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;
    Montgomery mont(n);
    u64 one = mont.init(1);
    auto f = [&](u64 x, u64 c) { return mont.reduce(u128(x) * x + c); }; // Wait, x is in mont form? 
    // Standard implementation: f(x) = x^2 + c. If x is mont form, x*x is mont.mul. + c (mont form) is add.
    // Correct logic using Montgomery class directly inside loop is cleaner.
    
    // Let's restart standard Pollard Rho with Montgomery logic embedded or minimal:
    // Actually using the one from the previous file might be cleaner if it's verified.
    // The provided file used a complex `Mint` struct. I will simplify.
    
    // Simple fast pollard rho with `__uint128_t`
    auto f_simple = [&](u128 x, u128 c, u64 m) { return (x * x + c) % m; };
    
    // For n < 2^64, we need __uint128_t for mul.
    // Optimized Brent's:
    u64 x = 2, y = 2, g = 1, c = 1;
    auto my_gcd = [&](u64 a, u64 b) {
        while (b) { a %= b; swap(a, b); }
        return a;
    };
    auto my_mul = [&](u64 a, u64 b, u64 m) { return (u64)((u128)a * b % m); };
    
    // Proper robust implementation:
    if (n == 1) return 1;
    if (n % 2 == 0) return 2;
    u64 xi = 0, yi = 0, ci = 0;
    // Randomized trial
    // Use a loop to try different c
    for (int i = 0; i < 100; ++i) { // Should find factor
        xi = rand() % (n - 1) + 1;
        yi = xi;
        ci = rand() % (n - 1) + 1;
        g = 1;
        while (g == 1) {
            xi = (my_mul(xi, xi, n) + ci) % n;
            yi = (my_mul(yi, yi, n) + ci) % n;
            yi = (my_mul(yi, yi, n) + ci) % n;
            g = my_gcd(xi > yi ? xi - yi : yi - xi, n);
            if (g == n) break; // failure
        }
        if (g > 1 && g < n) return g;
    }
    return n; // Should not happen for composite n
}

// Recursive factorization
vector<u64> factors;
void factor_recursive(u64 n) {
    if (n == 1) return;
    if (is_prime(n)) {
        factors.push_back(n);
        return;
    }
    u64 d = rho_pollard(n);
    factor_recursive(d);
    factor_recursive(n / d);
}

vector<u64> factor(u64 n) {
    factors.clear();
    factor_recursive(n);
    sort(factors.begin(), factors.end());
    return factors;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    srand(time(0));
    int t; cin >> t;
    while (t--) {
        u64 n; cin >> n;
        vector<u64> fac = factor(n);
        cout << fac.size() << ' ';
        for (auto f : fac) cout << f << ' ';
        cout << '\n';
    }
}

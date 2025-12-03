/*
 * Problem it Solves: Computes n-th Fibonacci number efficiently modulo m.
 * Usage: fib(n, mod) returns F(n) % mod.
 * Input/Output: n (long long), mod (int) -> F(n) (int).
 * Time Complexity: O(log n) using Fast Doubling method.
 * Tweaks: Modulo can be passed as argument.
 * Other Use Cases: Linear recurrences.
 * Warnings: n must be non-negative.
 */

#include <bits/stdc++.h>
using namespace std;

int fib(long long n, int mod) {
    assert(n >= 0);
    if (n <= 1) return n;
    int a = 0, b = 1;
    long long i = 1ll << (63 - __builtin_clzll(n) - 1);
    for (; i; i >>= 1) {
        int na = (1LL * a * a + 1LL * b * b) % mod;
        int nb = (2LL * a + b) * b % mod;
        a = na; b = nb;
        if (n & i) {
            int c = a + b;
            if (c >= mod) c -= mod;
            a = b; b = c;
        }
    }
    return b;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout << fib(10, 100) << '\n';
    return 0;
}

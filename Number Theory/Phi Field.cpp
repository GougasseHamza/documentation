/*
 * Problem it Solves: Computes Fibonacci numbers using field extension Q(sqrt(5)).
 * Usage: fib(n) returns F(n).
 * Input/Output: n (long long) -> F(n) (int mod MOD).
 * Time Complexity: O(log n).
 * Tweaks: MOD hardcoded.
 * Other Use Cases: Linear recurrences with roots in quadratic fields.
 * Warnings: 5 must be a quadratic residue mod MOD for this to work directly?
 * Actually, if 5 is not a residue, we work in the extension field directly, which this code does (a + b*sqrt(5)).
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
using ll = long long;

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

struct field { // a . 1 + b . sqrt(5)
    int a, b;
    field(int a = 0, int b = 0) : a(a), b(b) {}
    bool operator == (const field &oth) const {
        return a == oth.a and b == oth.b;
    }
    field operator + (const field &oth) const {
        return field((a + oth.a) % MOD, (b + oth.b) % MOD);
    }
    field operator - (const field &oth) const {
        return field((a - oth.a + MOD) % MOD, (b - oth.b + MOD) % MOD);
    }
    field operator * (const field &oth) const {
        return field((1LL * a * oth.a + 5LL * b * oth.b) % MOD, (1LL * a * oth.b + 1LL * b * oth.a) % MOD);
    }
    field operator * (const int &oth) const {
        return field(1LL * a * oth % MOD, 1LL * b * oth % MOD);
    }
    field inv() const {
        int d = modInverse((1LL * a * a - 5LL * b * b % MOD + MOD) % MOD);
        return field(1LL * a * d % MOD, 1LL * (MOD - b) * d % MOD);
    }
    field operator / (const field &oth) const {
        return (*this) * oth.inv();
    }
    field pow(ll k) const {
        field ans(1, 0), cur = *this;
        while (k) {
            if (k & 1) ans = ans * cur;
            cur = cur * cur;
            k >>= 1;
        }
        return ans;
    }
};

int fib(ll n) {
    // phi = (1 + sqrt(5)) / 2
    int inv2 = modInverse(2);
    field phi(inv2, inv2);
    // (phi^n - (1-phi)^n) / sqrt(5)
    // 1 - phi = 1 - (1/2 + 1/2 sqrt(5)) = 1/2 - 1/2 sqrt(5)
    
    field term1 = phi.pow(n);
    field term2 = (field(1, 0) - phi).pow(n);
    field num = term1 - term2;
    field den(0, 1); // sqrt(5)
    field ans = num / den;
    
    assert(ans.b == 0);
    return ans.a;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    for (int i = 0; i <= 10; i++) {
        cout << fib(i) << '\n';
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using u128 = __uint128_t;

u64 mulmod(u64 a, u64 b, u64 m) {
    return (u128)a * b % m;
}

u64 powmod(u64 a, u64 e, u64 m) {
    u64 r = 1;
    while (e) {
        if (e & 1) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

// Miller-Rabin primality test (deterministic for n < 2^64)
bool isPrime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2,3,5,7,11,13,17,19,23,29,31,37}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n-1, s = 0;
    while ((d & 1) == 0) d >>= 1, s++;
    auto check = [&](u64 a) {
        if (a % n == 0) return true;
        u64 x = powmod(a, d, n);
        if (x == 1 || x == n-1) return true;
        for (u64 r=1; r<s; r++) {
            x = mulmod(x, x, n);
            if (x == n-1) return true;
        }
        return false;
    };
    for (u64 a : {2,325,9375,28178,450775,9780504,1795265022})
        if (!check(a)) return false;
    return true;
}

u64 f_rho(u64 x, u64 c, u64 mod) {
    return (mulmod(x, x, mod) + c) % mod;
}

u64 pollard(u64 n) {
    if (n % 2 == 0) return 2;
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    while (true) {
        u64 x = uniform_int_distribution<u64>(2, n-2)(rng);
        u64 y = x;
        u64 c = uniform_int_distribution<u64>(1, n-1)(rng);
        u64 d = 1;
        while (d == 1) {
            x = f_rho(x, c, n);
            y = f_rho(f_rho(y, c, n), c, n);
            d = gcd<u64>(x>y?x-y:y-x, n);
        }
        if (d != n) return d;
    }
}

vector<u64> factor(u64 n) {
    if (n == 1) return {};
    if (isPrime(n)) return {n};
    u64 d = pollard(n);
    auto L = factor(d);
    auto R = factor(n/d);
    L.insert(L.end(), R.begin(), R.end());
    return L;
}

// Möbius function via Pollard Rho
int mobius(long long n) {
    if (n == 1) return 1;
    auto f = factor(n);
    sort(f.begin(), f.end());
    for (int i = 1; i < (int)f.size(); i++)
        if (f[i] == f[i-1]) return 0; // squared prime factor
    return (f.size() % 2 == 0 ? 1 : -1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;
    cout << mobius(n) << "\n";
}

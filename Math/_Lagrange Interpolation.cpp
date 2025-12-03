/*
 * Problem it Solves: Fast Lagrange Interpolation for equidistant points x=0, 1, ..., n.
 * Usage: Lagrange(p, x) where p contains values at 0, 1, ..., k.
 * Input/Output: p values, x target -> value at x.
 * Time Complexity: O(N).
 * Tweaks: MOD.
 * Other Use Cases: Sum of powers.
 * Warnings: Points must be 0, 1, 2...
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 1e9 + 7;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

// p = first at least n + 1 points: 0, 1, ..., n of the n degree polynomial
// returns f(x)
int Lagrange(const vector<int> &p, int x) {
    int n = p.size() - 1;
    if (x <= n && x >= 0) return p[x];
    
    vector<int> pref(n + 1, 1), suf(n + 1, 1);
    for (int i = 0; i < n; i++) pref[i + 1] = 1LL * pref[i] * (x - i + MOD) % MOD;
    for (int i = n; i > 0; i--) suf[i - 1] = 1LL * suf[i] * (x - i + MOD) % MOD;
    
    vector<int> fact(n + 1, 1), invFact(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = 1LL * fact[i - 1] * i % MOD;
    invFact[n] = modInverse(fact[n]);
    for (int i = n; i >= 1; i--) invFact[i - 1] = 1LL * invFact[i] * i % MOD;
    
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        int num = 1LL * p[i] * pref[i] % MOD * suf[i] % MOD;
        int den = 1LL * invFact[i] * invFact[n - i] % MOD;
        int term = 1LL * num * den % MOD;
        if ((n - i) & 1) ans = (ans - term + MOD) % MOD;
        else ans = (ans + term) % MOD;
    }
    return ans;
}

int main() {
    int n, k; cin >> n >> k;
    vector<int> p;
    int sum = 0; p.push_back(0);
    for (int i = 1; i <= k + 1; i++) {
        sum = (sum + power(i, k)) % MOD;
        p.push_back(sum);
    }
    cout << Lagrange(p, n) << '\n';
    return 0;
}

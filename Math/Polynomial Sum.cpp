/*
 * Problem it Solves: Computes sum_{i=0}^{n-1} a^i * i^k using Lagrange/Polynomial shift.
 * Usage: yo(p, n, a, k) or Lagrange.
 * Input/Output: n, k, a -> sum.
 * Time Complexity: O(K log MOD).
 * Tweaks: MOD.
 * Other Use Cases: Sum of powers.
 * Warnings: Special case a=1.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e7 + 9, MOD = 998244353;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = 1LL * ans * n % MOD;
        n = 1LL * n * n % MOD;
        k >>= 1;
    }
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

struct Combi {
    int n; vector<int> facts, finvs, invs;
    Combi(int _n): n(_n), facts(_n), finvs(_n), invs(_n) {
        facts[0] = finvs[0] = 1;
        invs[1] = 1;
        for (int i = 2; i < n; i++) invs[i] = 1LL * invs[MOD % i] * (MOD - MOD / i) % MOD;
        for (int i = 1; i < n; i++) {
            facts[i] = 1LL * facts[i - 1] * i % MOD;
            finvs[i] = 1LL * finvs[i - 1] * invs[i] % MOD;
        }
    }
    inline int ncr(int n, int k) { return n < k ? 0 : 1LL * facts[n] * finvs[k] % MOD * finvs[n - k] % MOD; }
};
Combi C(N);

int Lagrange(const vector<int> &p, int x) {
    int n = p.size() - 1;
    if (x <= n) return p[x];
    
    vector<int> pref(n + 1, 1), suf(n + 1, 1);
    for (int i = 0; i < n; i++) pref[i + 1] = 1LL * pref[i] * (x - i + MOD) % MOD;
    for (int i = n; i > 0; i--) suf[i - 1] = 1LL * suf[i] * (x - i + MOD) % MOD;
    
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        int num = 1LL * p[i] * pref[i] % MOD * suf[i] % MOD;
        int den = 1LL * C.finvs[i] * C.finvs[n - i] % MOD;
        int term = 1LL * num * den % MOD;
        if ((n - i) & 1) ans = (ans - term + MOD) % MOD;
        else ans = (ans + term) % MOD;
    }
    return ans;
}

// Computes S(n) = sum_{i=0}^{n} a^i * i^k
int yo(vector<int> S, long long n, int a, int k) {
    assert(k + 1 == S.size());
    assert(a != 1);
    int c = 0;
    int neg_a = (MOD - a) % MOD;
    int neg_a_plus_1_inv = modInverse((neg_a + 1) % MOD);
    int term_pow = power(neg_a_plus_1_inv, k + 1);
    
    for (int i = 0; i <= k; i++) {
        int term = 1LL * C.ncr(k + 1, i + 1) * power(neg_a, k - i) % MOD * S[i] % MOD;
        c = (c + term) % MOD;
    }
    c = 1LL * c * term_pow % MOD;
    
    int nw = 1;
    int inv_a = modInverse(a);
    for (int i = 0; i <= k; i++) {
        S[i] = 1LL * (S[i] - c + MOD) % MOD * nw % MOD;
        nw = 1LL * nw * inv_a % MOD;
    }
    
    int lag = Lagrange(S, n % MOD);
    return (1LL * lag * power(a, n) % MOD + c) % MOD;
}

int K;
int f(int i) { return power(i, K); }

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    long long a, n; cin >> a >> K >> n; n--;
    if (n < 0) { cout << 0 << '\n'; return 0; }
    
    vector<int> p;
    int sum = 0;
    for (int i = 0; i <= K; i++) {
        sum = (sum + 1LL * f(i) * power(a, i)) % MOD;
        p.push_back(sum);
    }
    
    if (a == 1) {
        sum = (sum + 1LL * f(K + 1) * power(a, K + 1)) % MOD;
        p.push_back(sum);
        cout << Lagrange(p, n % MOD) << '\n';
    } else {
        cout << yo(p, n, a, K) << '\n';
    }
    return 0;
}

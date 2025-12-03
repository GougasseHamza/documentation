/*
 * Problem it Solves: Computes Stirling Numbers of the Second Kind S2(n, i) for fixed n and all 0 <= i <= n.
 * Usage: main() demonstrates calculation using convolution.
 * Input/Output: n (integer) -> vector of values.
 * Time Complexity: O(n log n).
 * Tweaks: Uses relationship S2(n, k) = sum_{j=0}^k (-1)^(k-j) C(k, j) j^n / k!.
 * Other Use Cases: Combinatorics.
 * Warnings: MOD must be NTT-friendly.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 18;
const int MOD = 998244353;
const int root = 3;
int lim, rev[N], wn[N], inv_lim;

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

void precompute(int len) {
    lim = 1; int s = -1;
    while (lim < len) lim <<= 1, ++s;
    for (int i = 0; i < lim; ++i) rev[i] = rev[i >> 1] >> 1 | (i & 1) << s;
    int g = power(root, (MOD - 1) / lim);
    inv_lim = modInverse(lim);
    wn[0] = 1;
    for (int i = 1; i < lim; ++i) wn[i] = 1LL * wn[i - 1] * g % MOD;
}

void ntt(vector<int> &a, int typ) {
    for (int i = 0; i < lim; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int i = 1; i < lim; i <<= 1) {
        for (int j = 0; j < lim; j += 2 * i) {
            for (int k = 0; k < i; ++k) {
                int w = wn[lim / (2 * i) * k];
                int x = a[j + k], y = 1LL * a[j + k + i] * w % MOD;
                a[j + k] = (x + y) % MOD;
                a[j + k + i] = (x - y + MOD) % MOD;
            }
        }
    }
    if (typ == -1) {
        reverse(a.begin() + 1, a.begin() + lim);
        for (int i = 0; i < lim; ++i) a[i] = 1LL * a[i] * inv_lim % MOD;
    }
}

vector<int> multiply(vector<int> &f, vector<int> &g) {
    int n = f.size() + g.size() - 1;
    precompute(n);
    vector<int> a = f, b = g;
    a.resize(lim); b.resize(lim);
    ntt(a, 1); ntt(b, 1);
    for (int i = 0; i < lim; ++i) a[i] = 1LL * a[i] * b[i] % MOD;
    ntt(a, -1);
    a.resize(n);
    return a;
}

int fact[N], ifact[N];

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = 1LL * fact[i - 1] * i % MOD;
    ifact[n] = modInverse(fact[n]);
    for (int i = n - 1; i >= 0; i--) ifact[i] = 1LL * ifact[i + 1] * (i + 1) % MOD;
    
    vector<int> a(n + 1), b(n + 1);
    for (int i = 0; i <= n; i++) {
        a[i] = (i & 1 ? MOD - ifact[i] : ifact[i]) % MOD;
    }
    for (int i = 0; i <= n; i++) {
        b[i] = 1LL * power(i, n) * ifact[i] % MOD;
    }
    auto ans = multiply(a, b);
    ans.resize(n + 1);
    for (int i = 0; i <= n; i++) {
        cout << ans[i] << ' ';
    }
    cout << '\n';
    return 0;
}

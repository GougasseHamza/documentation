/*
 * Problem it Solves: Computes Stirling Numbers of the Second Kind S2(n, k) for fixed k and range of n.
 * Usage: stirling(n, k) returns vector of S2(i, k) for 0 <= i <= n.
 * Input/Output: n, k (integers) -> vector of values.
 * Time Complexity: O(n log^2 n).
 * Tweaks: Uses polynomial inversion and multiplication.
 * Other Use Cases: Partitions of set into k non-empty subsets.
 * Warnings: MOD must be NTT-friendly.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 998244353;
const int root = 3;

struct base {
    double x, y;
    base() { x = y = 0; }
    base(double x, double y) : x(x), y(y) {}
};
// Simplified for NTT friendly MOD
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

int lim, rev[N], wn[N], inv_lim;
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
                int w = wn[lim / (2 * i) * k]; // Adjusted index
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

// Basic Poly operations needed for inverse
using poly = vector<int>;

poly add(poly a, poly b) {
    poly c(max(a.size(), b.size()));
    for(int i = 0; i < c.size(); i++) c[i] = ((i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0)) % MOD;
    return c;
}
poly sub(poly a, poly b) {
    poly c(max(a.size(), b.size()));
    for(int i = 0; i < c.size(); i++) c[i] = ((i < a.size() ? a[i] : 0) - (i < b.size() ? b[i] : 0) + MOD) % MOD;
    return c;
}
poly mod_xk(poly a, int k) {
    if (a.size() > k) a.resize(k);
    return a;
}
poly inverse(poly a, int n) {
    assert(!a.empty() && a[0] != 0);
    poly ans = {modInverse(a[0])};
    for (int i = 1; i < n; i *= 2) {
        poly f = mod_xk(a, 2 * i);
        poly prod = multiply(ans, f);
        prod = mod_xk(prod, 2 * i); 
        for (int &x : prod) x = (MOD - x) % MOD; 
        prod[0] = (prod[0] + 2) % MOD; 
        ans = multiply(ans, prod);
        ans = mod_xk(ans, 2 * i);
    }
    return mod_xk(ans, n);
}

int fact[N], invFact[N];
void init_fact() {
    fact[0] = 1; invFact[0] = 1;
    for (int i = 1; i < N; i++) fact[i] = 1LL * fact[i - 1] * i % MOD;
    invFact[N - 1] = modInverse(fact[N - 1]);
    for (int i = N - 2; i >= 1; i--) invFact[i] = 1LL * invFact[i + 1] * (i + 1) % MOD;
}
int nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

// mul (1 - ix)
void yo(int l, int r, poly &ans) {
    if (l == r) {
        ans = {(int)1, (int)((MOD - l) % MOD)};
        return;
    }
    int mid = (l + r) >> 1;
    poly a, b;
    yo(l, mid, a);
    yo(mid + 1, r, b);
    ans = multiply(a, b);
}

// stirling2nd(i, k) for 0 <= i <= n
vector<int> stirling(int n, int k) {
    poly p;
    yo(1, k, p);
    p = inverse(p, n + 1);
    p.insert(p.begin(), k, 0);
    p.resize(n + 1);
    return p;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    init_fact();
    int n, k; cin >> n >> k;
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        int p; cin >> p; sum = (sum + p) % MOD;
    }
    if (k == 1) {
        cout << 1LL * sum * n % MOD << '\n';
        return 0;
    }
    auto p = stirling(n, k - 1);
    int ans = 0;
    for (int i = 1; i <= n; i++) 
        if (n - i >= 0) ans = (ans + 1LL * nCr(n - 1, i - 1) * i % MOD * p[n - i]) % MOD;
    ans = 1LL * ans * sum % MOD;
    cout << ans << '\n';
    return 0;
}

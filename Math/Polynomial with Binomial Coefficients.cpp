/*
 * Problem it Solves: Computes sum_{i=0}^{k} a_i * nCr(N, i) using polynomial multiplication.
 * Usage: extract(p) converts to appropriate form.
 * Input/Output: n, k -> sum.
 * Time Complexity: O(K log K).
 * Tweaks: MOD.
 * Other Use Cases: Sum of powers.
 * Warnings: MOD must be NTT-friendly.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 998244353;
const int root = 3;

int power(int x, int y) {
    int ans = 1;
    for (; y; y >>= 1, x = 1LL * x * x % MOD) if (y & 1) ans = 1LL * ans * x % MOD;
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

int lim, rev[1 << 19], w[1 << 19], wn[1 << 19], inv_lim;
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
        for (int j = 0, t = lim / i / 2; j < i; ++j) w[j] = wn[j * t];
        for (int j = 0; j < lim; j += i << 1) {
            for (int k = 0; k < i; ++k) {
                int x = a[k + j], y = 1LL * a[k + j + i] * w[k] % MOD;
                a[k + j] = (x + y) % MOD;
                a[k + j + i] = (x - y + MOD) % MOD;
            }
        }
    }
    if (typ == -1) {
        reverse(a.begin() + 1, a.begin() + lim);
        for (int i = 0; i < lim; ++i) a[i] = 1LL * a[i] * inv_lim % MOD;
    }
}

vector<int> multiply(vector<int> f, vector<int> g) {
    int n = f.size() + g.size() - 1;
    precompute(n);
    vector<int> a = f, b = g;
    a.resize(lim); b.resize(lim);
    ntt(a, 1), ntt(b, 1);
    for (int i = 0; i < lim; ++i) a[i] = 1LL * a[i] * b[i] % MOD;
    ntt(a, -1);
    a.resize(n);
    return a;
}

int fact[N], invFact[N];
void pre() {
    fact[0] = 1;
    for (int i = 1; i < N; i++) fact[i] = 1LL * fact[i - 1] * i % MOD;
    invFact[N - 1] = modInverse(fact[N - 1]);
    for (int i = N - 2; i >= 0; i--) invFact[i] = 1LL * invFact[i + 1] * (i + 1) % MOD;
}

// p(x) = sum a_i * C(x, i)
// Given p(0)...p(n), returns a_0...a_n
vector<int> extract(vector<int> &p) {
    int n = p.size();
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) {
        a[i] = 1LL * p[i] * invFact[i] % MOD;
        b[i] = 1LL * (i & 1 ? MOD - 1 : 1) * invFact[i] % MOD;
    }
    auto ans = multiply(a, b);
    ans.resize(n);
    for (int i = 0; i < n; i++) ans[i] = 1LL * ans[i] * fact[i] % MOD;
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    pre();
    int n, k; cin >> n >> k;
    vector<int> p;
    int sum = 0; p.push_back(0);
    for (int i = 1; i <= k + 1; i++) {
        sum = (sum + power(i, k)) % MOD;
        p.push_back(sum);
    }
    
    auto a = extract(p);
    
    // Calculate nCr(n, i) for i from 0 to size of a
    vector<int> nck(a.size(), 0);
    nck[0] = 1;
    // nCr(n, i) = nCr(n, i-1) * (n - i + 1) / i
    for (int i = 1; i < nck.size(); i++) {
        int num = (n - i + 1) % MOD;
        int den = 1LL * num * modInverse(i) % MOD; // Wait, inverse of i, not num
        // Actually: nCr(n, i) = nCr(n, i-1) * (n - i + 1) * inv(i)
        nck[i] = 1LL * nck[i - 1] * num % MOD * modInverse(i) % MOD;
    }
    
    int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        ans = (ans + 1LL * a[i] * nck[i]) % MOD;
    }
    cout << ans << '\n';
    return 0;
}

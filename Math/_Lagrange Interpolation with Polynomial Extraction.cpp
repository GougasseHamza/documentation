/*
 * Problem it Solves: Lagrange Interpolation with explicit polynomial extraction using Divide & Conquer and NTT.
 * Usage: Lagrange(y_values) assuming x = 0, 1, ... N-1.
 * Input/Output: Y-values -> Polynomial coefficients.
 * Time Complexity: O(N log^2 N).
 * Tweaks: MOD must be NTT-friendly.
 * Other Use Cases: Multipoint evaluation (inverse).
 * Warnings: Requires N to be power of 2 ideally for full speed, but works generally.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 998244353;
const int root = 3;

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

vector<int> add(vector<int> a, vector<int> b) {
    int n = max(a.size(), b.size());
    vector<int> ans(n);
    for(int i = 0; i < n; i++) ans[i] = ((i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0)) % MOD;
    return ans;
}

int fact[N], invFact[N];
void init_fact() {
    fact[0] = 1;
    for (int i = 1; i < N; i++) fact[i] = 1LL * fact[i - 1] * i % MOD;
    invFact[N - 1] = modInverse(fact[N - 1]);
    for (int i = N - 2; i >= 0; i--) invFact[i] = 1LL * invFact[i + 1] * (i + 1) % MOD;
}

vector<int> t[N * 4];
void build(int id, int l, int r) {
    if (l == r) {
        t[id] = {(MOD - l) % MOD, 1};
        return;
    }
    int mid = l + r >> 1;
    build(id << 1, l, mid);
    build(id << 1 | 1, mid + 1, r);
    t[id] = multiply(t[id << 1], t[id << 1 | 1]);
}

vector<int> yo(int id, int l, int r, vector<int> &p) {
    if (l == r) {
        int n = (int)p.size() - 1;
        int tmp = ((n - l) & 1 ? MOD - 1 : 1);
        int val = 1LL * p[l] * tmp % MOD * invFact[l] % MOD * invFact[n - l] % MOD;
        return {val};
    }
    int mid = l + r >> 1;
    vector<int> L = yo(id << 1, l, mid, p);
    vector<int> R = yo(id << 1 | 1, mid + 1, r, p);
    return add(multiply(L, t[id << 1 | 1]), multiply(t[id << 1], R));
}

vector<int> Lagrange(vector<int> &p) {
    int n = p.size();
    build(1, 0, n - 1);
    auto ans = yo(1, 0, n - 1, p);
    ans.resize(n);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    init_fact();
    int n, k; cin >> n >> k;
    vector<int> p;
    int sum = 0; p.push_back(0);
    for (int i = 1; i <= k + 1; i++) {
        sum = (sum + power(i, k)) % MOD;
        p.push_back(sum);
    }
    auto a = Lagrange(p);
    int ans = 0;
    for (int i = 0; i < a.size(); i++) ans = (ans + 1LL * a[i] * power(n, i)) % MOD;
    cout << ans << '\n';
    return 0;
}

/*
 * Problem it Solves: Computes sum_{k=1}^n C(a_k, m) for various m using graph properties and NTT.
 * Usage: yo(n, a) returns vector of sums.
 * Input/Output: n, a -> vector of sums.
 * Time Complexity: O(N log^2 N).
 * Tweaks: NTT friendly MOD.
 * Other Use Cases: Graph theory problems involving subtrees.
 * Warnings: Highly specific to a tree problem.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, MOD = 924844033;
const int root = 5; // Root for this MOD? Original code used 998244353 usually 3. 924844033 primitive root is 5.

struct base {
    double x, y;
    base() { x = y = 0; }
    base(double x, double y): x(x), y(y) { }
};
// Using NTT implementation since MOD is large prime (924844033 = 2^21 * 441 + 1)
// Need correct primitive root for 924844033. 5 is a primitive root.

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
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

vector<int> g[N];
int sz[N];
void dfs(int u, int p = 0) {
    sz[u] = 1;
    for (auto v : g[u]) {
        if (v == p) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}

int f[N], ifac[N];
// ans_k = sum_(C(a_i, k))
vector<int> yo(int n, vector<int> a) {
    vector<int> cnt(N, 0);
    for (auto x : a) cnt[x]++;
    for (int i = 0; i < N; i++) cnt[i] = 1LL * cnt[i] * f[i] % MOD;
    vector<int> b(N, 0);
    for (int i = 0; i < N; i++) b[i] = ifac[i];
    reverse(b.begin(), b.end());
    auto ans = multiply(cnt, b);
    ans.erase(ans.begin(), ans.begin() + N - 1);
    ans.resize(n + 1);
    for (int i = 0; i <= n; i++) ans[i] = 1LL * ans[i] * modInverse(f[i]) % MOD;
    return ans;
}

int ncr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return 1LL * f[n] * ifac[k] % MOD * ifac[n - k] % MOD;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    f[0] = 1;
    for (int i = 1; i < N; i++) f[i] = 1LL * i * f[i - 1] % MOD;
    ifac[N - 1] = modInverse(f[N - 1]);
    for (int i = N - 2; i >= 0; i--) ifac[i] = 1LL * ifac[i + 1] * (i + 1) % MOD;
    
    int n; cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1);
    vector<int> a;
    for (int i = 2; i <= n; i++) a.push_back(sz[i]);
    vector<int> ans(n + 1, 0);
    for (int i = 1; i <= n; i++) ans[i] = 1LL * ncr(n, i) * n % MOD;
    
    auto p = yo(n, a);
    for (int i = 1; i <= n; i++) ans[i] = (ans[i] - p[i] + MOD) % MOD;
    for (auto &x : a) x = n - x;
    p = yo(n, a);
    for (int i = 1; i <= n; i++) ans[i] = (ans[i] - p[i] + MOD) % MOD;
    
    for (int i = 1; i <= n; i++) cout << ans[i] << '\n';
    return 0;
}

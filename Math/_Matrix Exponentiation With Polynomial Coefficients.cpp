/*
 * Problem it Solves: Matrix Exponentiation where matrix elements are polynomials.
 * Usage: M.pow(k, z).
 * Input/Output: N, K -> coefficients of polynomial.
 * Time Complexity: O(N^3 * poly_len log poly_len * log K).
 * Tweaks: MOD, poly multiplication logic.
 * Other Use Cases: Generating functions on graphs.
 * Warnings: Matrix size N is small (e.g., 2).
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 16;
const int MOD = 998244353;
const int root = 3;
int lim, rev[N], w[N], wn[N], inv_lim;

int power(int x, int y) {
    int ans = 1;
    for (; y; y >>= 1, x = 1LL * x * x % MOD) if (y & 1) ans = 1LL * ans * x % MOD;
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

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
                const int x = a[k + j], y = 1LL * a[k + j + i] * w[k] % MOD;
                a[k + j] = (x + y) % MOD;
                a[k + j + i] = (x - y + MOD) % MOD;
            }
        }
    }
    if (!typ) {
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
    ntt(a, 0);
    return a;
}

using poly = vector<int>;

struct Mat {
    int n, m;
    vector<vector<poly>> a;
    Mat() { }
    Mat(int _n, int _m) { n = _n; m = _m; a.assign(n, vector<poly>(m, poly({0}))); }
    
    void make_unit() {
        assert(n == m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) a[i][j] = poly({i == j});
        }
    }
    
    Mat operator * (const Mat &b) {
        assert(m == b.n);
        Mat ans = Mat(n, b.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < b.m; j++) {
                for (int k = 0; k < m; k++) {
                    poly prod = multiply(a[i][k], b.a[k][j]);
                    if (ans.a[i][j].size() < prod.size()) ans.a[i][j].resize(prod.size());
                    for (int x = 0; x < prod.size(); x++) {
                        ans.a[i][j][x] = (ans.a[i][j][x] + prod[x]) % MOD;
                    }
                }
            }
        }
        return ans;
    }
    
    void reduce(int z) { // % x^z
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j].size() > z) a[i][j].resize(z);
            }
        }
    }
    
    Mat pow(long long k, int z) { // M^k % x^z
        assert(n == m);
        Mat ans(n, n), t = *this; ans.make_unit();
        while (k) {
            if (k & 1) {
                ans = ans * t;
                ans.reduce(z);
            }
            t = t * t;
            t.reduce(z);
            k >>= 1;
        }
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, k; cin >> n >> k;
    Mat M(2, 2);
    M.a[0][0] = poly({1, 1});
    M.a[0][1] = poly({0, 1});
    M.a[1][0] = poly({1});
    M.a[1][1] = poly({0});
    
    M = M.pow(n - 1, k + 1);
    
    poly p0 = poly({1});
    poly p1 = poly({1, 1});
    poly a = multiply(M.a[0][0], p1);
    poly b = multiply(M.a[0][1], p0);
    
    vector<int> ans(k + 1);
    a.resize(k + 1);
    b.resize(k + 1);
    for (int i = 0; i <= k; i++) {
        ans[i] = (a[i] + b[i]) % MOD;
    }
    for (int i = 1; i <= k; i++) cout << ans[i] << ' ';
    cout << '\n';
    return 0;
}

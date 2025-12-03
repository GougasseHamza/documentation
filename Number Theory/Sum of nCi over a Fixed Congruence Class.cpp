/*
 * Problem it Solves: Computes sum_{k=0}^n C(n, k) such that k % m = r.
 * Usage: solve(n, m, r).
 * Input/Output: n (long long), m (int), r -> sum.
 * Time Complexity: O(m log m log n).
 * Tweaks: Uses polynomial multiplication.
 * Other Use Cases: Roots of unity filter.
 * Warnings: MOD must be NTT-friendly.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 16;
const int MOD = 998244353;
const int root = 3;
using ll = long long;
int lim, rev[N], wn[N], inv_lim;

int power(int x, int y, int ans = 1) {
    for (; y; y >>= 1, x = (long long) x * x % MOD) if (y & 1) ans = (long long) ans * x % MOD;
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
    while ((int)a.size() && a.back() == 0) a.pop_back();
    return a;
}

int m;
void fix(vector<int> &ans) {
    int k = ans.size();
    if (k < m) return;
    for (int i = k - 1; i >= m; i--) {
        ans[i % m] = (ans[i % m] + ans[i]) % MOD;
    }
    ans.resize(m);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ll n; cin >> n; int x; cin >> x >> m;
    vector<int> a({1, 1}), ans({1});
    // (1+x)^n expanded, taking terms mod x^m-1
    // Actually the problem asks for sum of C(n, k) where k%m = r
    // This corresponds to [x^r] (1+x)^n mod (x^m - 1)
    
    while (n) {
        if (n & 1) {
            ans = multiply(ans, a); fix(ans);
        }
        a = multiply(a, a); fix(a);
        n >>= 1;
    }
    ans.resize(m);
    
    // If the question is specialized (like weighted sum), modify below loop
    int cur = 1, res = 0;
    for (int i = 0; i < m; i++) {
        res = (res + 1LL * cur * ans[i]) % MOD;
        cur = 1LL * cur * x % MOD;
    }
    cout << res << '\n';
    return 0;
}

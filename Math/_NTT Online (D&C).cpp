/*
 * Problem it Solves: Online Convolution (calculating convolution as elements arrive).
 * Usage: C.extend(i, a[i], b[i]) returns c[i].
 * Input/Output: a[i], b[i] -> c[i] online.
 * Time Complexity: O(N log^2 N).
 * Tweaks: MOD.
 * Other Use Cases: DP optimizations where dp[i] depends on dp[j]*b[i-j].
 * Warnings: Requires MOD to be NTT-friendly.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
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
    return a;
}

// 1s for n = 2e5
struct OnlineConvolution {
    vector<int> a, b, c;
    int k;
    
    OnlineConvolution(int n): a(n), b(n), c(n), k(0) {}
    
    // poly c = poly a * poly b
    // add a[i] = x and b[i] = y and it will return c[i]
    // online!
    int extend(int i, int x, int y) {
        assert(i == k);
        a[k] = x;
        b[k] = y;
        int s = k + 2;
        // Check powers of 2 which divide s
        for (int w = 1; s % w == 0 && w < s; w <<= 1) {
            for (int ri = 0; ri < 2; ri++) {
                // Optimization: if calculating self convolution (a=b), skip symmetric part?
                // This logic handles a*b where a and b are different or same.
                // The condition `w * 2 != s` handles overlap?
                // Standard relaxed convolution logic:
                // When size reaches 2*w, we convolve a[w..2w-1] with b[0..w-1] and a[0..w-1] with b[w..2w-1]
                if (ri == 0 || w * 2 != s) {
                    vector<int> f(w), g(w);
                    for (int j = 0; j < w; j++) f[j] = a[w - 1 + j], g[j] = b[k - w + 1 + j];
                    f = multiply(f, g);
                    for (int j = 0, idx = k; j < f.size() && idx < c.size(); j++, idx++) {
                        c[idx] = (c[idx] + f[j]) % MOD;
                    }
                }
                swap(a, b);
            }
        }
        return c[k++];
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    OnlineConvolution C(n);
    vector<int> f(n + 1);
    f[0] = 1;
    int pref = 0;
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        // Example: f[i] = x * sum(f[j] * f[i-1-j]) -> f[i] depends on convolution of f with itself
        int y = C.extend(i - 1, f[i - 1], f[i - 1]) % MOD;
        pref = (pref + y) % MOD;
        f[i] = 1LL * x * pref % MOD;
        cout << f[i] << ' ';
    }
    cout << '\n';
    return 0;
}

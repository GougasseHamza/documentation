/*
 * Problem it Solves: 2D NTT (Convolution of Matrices).
 * Usage: multiply(a, b) returns convolution.
 * Input/Output: Matrix A, B -> Matrix C.
 * Time Complexity: O(N^2 log N).
 * Tweaks: MOD.
 * Other Use Cases: 2D Pattern Matching.
 * Warnings: Matrix dimensions should be power of 2 or padded.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 13; // Max Dimension
const int MOD = 998244353;
const int root = 3;

using Mat = vector<vector<int>>;

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

Mat multiply(Mat a, Mat b) {
    int n = a.size(), m = b.size();
    int len = n + m - 1;
    precompute(len);
    
    a.resize(lim); b.resize(lim);
    for (int i = 0; i < lim; i++) a[i].resize(lim, 0);
    for (int i = 0; i < lim; i++) b[i].resize(lim, 0);
    
    // 2D NTT: Apply NTT on rows, then columns
    for (int i = 0; i < lim; i++) {
        ntt(a[i], 1);
        ntt(b[i], 1);
    }
    
    // Transpose to apply on columns easily (or iterate columns)
    // Here we just iterate columns and copy
    for (int j = 0; j < lim; j++) {
        vector<int> col_a(lim), col_b(lim);
        for (int i = 0; i < lim; i++) col_a[i] = a[i][j], col_b[i] = b[i][j];
        
        ntt(col_a, 1);
        ntt(col_b, 1);
        
        for (int i = 0; i < lim; i++) col_a[i] = 1LL * col_a[i] * col_b[i] % MOD;
        
        ntt(col_a, -1);
        for (int i = 0; i < lim; i++) a[i][j] = col_a[i];
    }
    
    // Inverse NTT on rows
    for (int i = 0; i < lim; i++) ntt(a[i], -1);
    
    a.resize(n + m - 1);
    for (int i = 0; i < n + m - 1; i++) a[i].resize(n + m - 1);
    return a;
}

int main() {
    // Example Usage
    return 0;
}

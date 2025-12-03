/*
 * Problem it Solves: Online Convolution using a different strategy (possibly simpler).
 * Usage: yo(l, r) computes dp in range using Divide and Conquer.
 * Input/Output: coefficients -> dp values.
 * Time Complexity: O(N log^2 N).
 * Tweaks: MOD.
 * Other Use Cases: Any recurrence of form dp[i] = sum(dp[j] * a[i-j]).
 * Warnings: Requires MOD to be NTT-friendly.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1 << 19;
const int MOD = 998244353;
const int root = 3;

int power(int x, int y) {
    int ans = 1;
    for (; y; y >>= 1, x = 1LL * x * x % MOD) if (y & 1) ans = 1LL * ans * x % MOD;
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

int lim, rev[N], w[N], wn[N], inv_lim;
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

int ans[N], a[N], dp[N];

// dp[l] depends on previous values
void yo(int l, int r) {
    if (l == r) {
        dp[l] = ((l ? dp[l - 1] : 0) + ans[l]) % MOD;
        if (dp[l] < 0) dp[l] += MOD;
        return;
    }
    int mid = l + r >> 1;
    yo(l, mid);
    
    vector<int> b;
    for (int i = l; i <= r; i++) b.push_back(a[i - l]);
    vector<int> f;
    for (int i = l; i <= mid; i++) f.push_back(dp[i]);
    
    auto res = multiply(b, f);
    // res[k] corresponds to convolution sum for index L + k?
    // b starts at a[0], a[1]... which are coefficients for dp[i], dp[i-1]...
    // Check original logic: `b` has `a[0]...a[r-l]`. `f` has `dp[l]...dp[mid]`.
    // We want contribution to `ans[mid+1...r]`.
    // ans[i] += sum_{j=l}^{mid} dp[j] * a[i-j]
    // Let j' = j - l (0 to mid-l). i' = i - l.
    // ans[l + i'] += sum dp[l + j'] * a[i' - j']
    
    // In multiply, we have `dp` and `a`.
    // res[k] = sum dp[l+j'] * a[k-j']. 
    // So res[k] contributes to ans[l+k].
    
    for (int i = mid + 1; i <= r; i++) {
        ans[i] = (ans[i] + res[i - l]) % MOD;
    }
    yo(mid + 1, r);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // Example setup from original code (CSAcademy Jetpack)
    // Need to define `a` and call `yo`.
    return 0;
}

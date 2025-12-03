/*
 * Problem it Solves: Computes Bell Numbers using polynomial exponentiation.
 * Usage: bell(n) returns the n-th Bell number.
 * Input/Output: n (integer) -> Bell number (integer mod MOD).
 * Time Complexity: O(N log N) using FFT/NTT logic (MTT for 1e9+7).
 * Tweaks: MOD is hardcoded as 1e9+7.
 * Other Use Cases: Generating functions, partition problems.
 * Warnings: Large constant factor due to MTT.

 In Competitive Programming (CP), Bell Numbers (Bn​) appear in combinatorics problems involving set partitions.

Here is a comprehensive guide to using Bell Numbers in contests, ranging from definition to O(NlogN) implementation techniques.

1. Definition

The Bell number Bn​ counts the number of ways to partition a set of n distinguishable elements into non-empty, distinguishable subsets.

A. Recursive / DP Approach (O(N2))

Bn+1​=k=0∑n​(kn​)Bk​
B. Via Stirling Numbers (O(NlogN))
Bn​=k=0∑n​S(n,k)

S(n,k)=1/k!​∑k​(−1)k−j(j k​)j^n

This can be rewritten as a convolution:
S(n,k)=∑​(((−1)^(k−j​)/(k-j)!) (j^n /j!​)

Algorithm:
    Define Polynomial A(x)=∑i=0n​i!(−1)i​xi.
    Define Polynomial C(x)=∑i=0n​i!in​xi
    Multiply A(x) and C(x) using NTT.
    The coefficient of xk in the result is S(n,k).
    Sum all coefficients to get Bn​.

The Exponential Generating Function (EGF) for Bell numbers is:
B(x)=n=0∑∞​Bn​n!xn​=eex−1



Touchard's Congruence

Useful if N is massive (1018) but you need the answer modulo a small prime p.
Bn+p​≡Bn​+Bn+1​(modp)
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const double PI = acos(-1.0);

struct base {
    double x, y;
    base() { x = y = 0; }
    base(double x, double y) : x(x), y(y) {}
};
inline base operator+(base a, base b) { return base(a.x + b.x, a.y + b.y); }
inline base operator-(base a, base b) { return base(a.x - b.x, a.y - b.y); }
inline base operator*(base a, base b) { return base(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline base conj(base a) { return base(a.x, -a.y); }

int lim = 1;
vector<base> roots = {{0, 0}, {1, 0}};
vector<int> rev = {0, 1};

void ensure_base(int p) {
    if (p <= lim) return;
    rev.resize(1 << p);
    for (int i = 0; i < (1 << p); i++) rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (p - 1));
    roots.resize(1 << p);
    while (lim < p) {
        double angle = 2 * PI / (1 << (lim + 1));
        for (int i = 1 << (lim - 1); i < (1 << lim); i++) {
            roots[i << 1] = roots[i];
            double angle_i = angle * (2 * i + 1 - (1 << lim));
            roots[(i << 1) + 1] = base(cos(angle_i), sin(angle_i));
        }
        lim++;
    }
}

void fft(vector<base> &a, int n = -1) {
    if (n == -1) n = a.size();
    int zeros = __builtin_ctz(n);
    ensure_base(zeros);
    int shift = lim - zeros;
    for (int i = 0; i < n; i++)
        if (i < (rev[i] >> shift)) swap(a[i], a[rev[i] >> shift]);
    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                base z = a[i + j + k] * roots[j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] = a[i + j] + z;
            }
        }
    }
}

vector<int> multiply(vector<int> &a, vector<int> &b, int eq = 0) {
    int need = a.size() + b.size() - 1;
    int p = 0;
    while ((1 << p) < need) p++;
    ensure_base(p);
    int sz = 1 << p;
    vector<base> A, B;
    if (sz > (int)A.size()) A.resize(sz);
    for (int i = 0; i < (int)a.size(); i++) {
        int x = (a[i] % MOD + MOD) % MOD;
        A[i] = base(x & ((1 << 15) - 1), x >> 15);
    }
    fill(A.begin() + a.size(), A.begin() + sz, base{0, 0});
    fft(A, sz);
    if (sz > (int)B.size()) B.resize(sz);
    if (eq) copy(A.begin(), A.begin() + sz, B.begin());
    else {
        for (int i = 0; i < (int)b.size(); i++) {
            int x = (b[i] % MOD + MOD) % MOD;
            B[i] = base(x & ((1 << 15) - 1), x >> 15);
        }
        fill(B.begin() + b.size(), B.begin() + sz, base{0, 0});
        fft(B, sz);
    }
    double ratio = 0.25 / sz;
    base r2(0, -1), r3(ratio, 0), r4(0, -ratio), r5(0, 1);
    for (int i = 0; i <= (sz >> 1); i++) {
        int j = (sz - i) & (sz - 1);
        base a1 = (A[i] + conj(A[j])), a2 = (A[i] - conj(A[j])) * r2;
        base b1 = (B[i] + conj(B[j])) * r3, b2 = (B[i] - conj(B[j])) * r4;
        if (i != j) {
            base c1 = (A[j] + conj(A[i])), c2 = (A[j] - conj(A[i])) * r2;
            base d1 = (B[j] + conj(B[i])) * r3, d2 = (B[j] - conj(B[i])) * r4;
            A[i] = c1 * d1 + c2 * d2 * r5;
            B[i] = c1 * d2 + c2 * d1;
        }
        A[j] = a1 * b1 + a2 * b2 * r5;
        B[j] = a1 * b2 + a2 * b1;
    }
    fft(A, sz);
    fft(B, sz);
    vector<int> res(need);
    for (int i = 0; i < need; i++) {
        long long aa = A[i].x + 0.5;
        long long bb = B[i].x + 0.5;
        long long cc = A[i].y + 0.5;
        res[i] = (aa + ((bb % MOD) << 15) + ((cc % MOD) << 30)) % MOD;
    }
    return res;
}

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

using poly = vector<int>;

poly add(poly a, poly b) {
    int n = max(a.size(), b.size());
    poly ans(n);
    for (int i = 0; i < n; i++) {
        int val = (i < a.size() ? a[i] : 0) + (i < b.size() ? b[i] : 0);
        ans[i] = val >= MOD ? val - MOD : val;
    }
    return ans;
}

poly sub(poly a, poly b) {
    int n = max(a.size(), b.size());
    poly ans(n);
    for (int i = 0; i < n; i++) {
        int val = (i < a.size() ? a[i] : 0) - (i < b.size() ? b[i] : 0);
        ans[i] = val < 0 ? val + MOD : val;
    }
    return ans;
}

poly mul(poly a, poly b) {
    if (a.empty() || b.empty()) return {};
    return multiply(a, b, a == b);
}

poly mod_xk(poly a, int k) {
    if (a.size() > k) a.resize(k);
    return a;
}

poly differentiate(poly a) {
    int n = a.size();
    if (n <= 1) return {};
    poly ans(n - 1);
    for (int i = 1; i < n; i++) ans[i - 1] = 1LL * a[i] * i % MOD;
    return ans;
}

poly integrate(poly a) {
    int n = a.size();
    poly ans(n + 1);
    ans[0] = 0;
    for (int i = 0; i < n; i++) ans[i + 1] = 1LL * a[i] * modInverse(i + 1) % MOD;
    return ans;
}

poly inverse(poly a, int n) {
    assert(!a.empty() && a[0] != 0);
    poly ans = {modInverse(a[0])};
    for (int i = 1; i < n; i *= 2) {
        poly f = mod_xk(a, 2 * i);
        poly prod = mul(ans, f);
        prod = mod_xk(prod, 2 * i); // ans * a
        
        // ans = ans * (2 - ans * a)
        for (int &x : prod) x = (MOD - x) % MOD; // - (ans * a)
        prod[0] = (prod[0] + 2) % MOD; // 2 - ans * a
        
        ans = mul(ans, prod);
        ans = mod_xk(ans, 2 * i);
    }
    return mod_xk(ans, n);
}

poly log(poly a, int n) {
    assert(!a.empty() && a[0] == 1);
    poly der = differentiate(a);
    der = mod_xk(der, n - 1);
    poly inv = inverse(a, n - 1);
    poly prod = mul(der, inv);
    prod = mod_xk(prod, n - 1);
    return integrate(prod);
}

poly exp(poly a, int n) {
    if (a.empty()) return {1};
    assert(a[0] == 0);
    poly ans = {1};
    int i = 1;
    while (i < n) {
        poly lg = log(ans, 2 * i);
        // ans = ans * (1 - ln(ans) + a)
        poly term = sub(mod_xk(a, 2 * i), lg);
        term[0] = (term[0] + 1) % MOD;
        ans = mul(ans, term);
        ans = mod_xk(ans, 2 * i);
        i *= 2;
    }
    return mod_xk(ans, n);
}

vector<int> bell(int n) {
    poly p(n + 1);
    int f = 1;
    for (int i = 0; i <= n; i++) {
        p[i] = modInverse(f);
        if (i < n) f = 1LL * f * (i + 1) % MOD;
    }
    p[0] = (p[0] - 1 + MOD) % MOD; // e^x - 1
    p = exp(p, n + 1);
    
    vector<int> ans(n + 1);
    f = 1;
    for (int i = 0; i <= n; i++) {
        ans[i] = 1LL * p[i] * f % MOD;
        if (i < n) f = 1LL * f * (i + 1) % MOD;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    auto ans = bell(n);
    cout << ans[n] << '\n';
    return 0;
}

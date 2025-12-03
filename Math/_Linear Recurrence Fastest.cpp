/*
 * Problem it Solves: Computes k-th term of linear recurrence S[n] = sum(C[i]*S[n-i-1]).
 * Usage: LinearRecurrence(S, C, n).
 * Input/Output: S (initial), C (coeffs), n (target index) -> value.
 * Time Complexity: O(K log K log N). Uses FFT.
 * Tweaks: K is size of recurrence. MOD must be FFT-friendly.
 * Other Use Cases: Fibonacci for large N.
 * Warnings: S and C must be same size K.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 104857601; // A prime for FFT

struct base {
    double x, y;
    base() { x = y = 0; }
    base(double x, double y): x(x), y(y) { }
    base operator + (const base &b) const { return base(x + b.x, y + b.y); }
    base operator - (const base &b) const { return base(x - b.x, y - b.y); }
    base operator * (const base &b) const { return base(x * b.x - y * b.y, x * b.y + y * b.x); }
};
inline base conj(base a) { return base(a.x, -a.y); }

int lim = 1;
vector<base> roots = {{0, 0}, {1, 0}};
vector<int> rev = {0, 1};
const double PI = acosl(-1.0);

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
    for (int i = 0; i < n; i++) if (i < (rev[i] >> shift)) swap(a[i], a[rev[i] >> shift]);
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
    A.resize(sz);
    for (int i = 0; i < (int)a.size(); i++) {
        int x = (a[i] % MOD + MOD) % MOD;
        A[i] = base(x & ((1 << 15) - 1), x >> 15);
    }
    fft(A, sz);
    if (sz > (int)B.size()) B.resize(sz);
    if (eq) copy(A.begin(), A.begin() + sz, B.begin());
    else {
        for (int i = 0; i < (int)b.size(); i++) {
            int x = (b[i] % MOD + MOD) % MOD;
            B[i] = base(x & ((1 << 15) - 1), x >> 15);
        }
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
    fft(A, sz); fft(B, sz);
    vector<int> res(need);
    for (int i = 0; i < need; i++) {
        long long aa = A[i].x + 0.5;
        long long bb = B[i].x + 0.5;
        long long cc = A[i].y + 0.5;
        res[i] = (aa + ((bb % MOD) << 15) + ((cc % MOD) << 30)) % MOD;
    }
    return res;
}

int LinearRecurrence(vector<int>& S, vector<int> &C, long long n) {
    if (C.empty()) return 0;
    int k = (int)C.size();
    assert((int)S.size() == k);
    if (n < k) return S[n];
    
    auto Q = C;
    for (auto &x : Q) x = (MOD - x) % MOD;
    Q.insert(Q.begin(), 1); // Q(x) = 1 - sum(C_i x^{i+1})
    
    auto P = multiply(S, Q);
    P.resize(k);
    
    while (n) {
        auto Qminus = Q;
        for (int i = 1; i < (int)Qminus.size(); i += 2) Qminus[i] = (MOD - Qminus[i]) % MOD;
        auto S_poly = multiply(P, Qminus);
        auto T_poly = multiply(Q, Qminus);
        
        if (n & 1) {
            for (int i = 1; i < (int)S_poly.size(); i += 2) P[i >> 1] = S_poly[i];
            for (int i = 0; i < (int)T_poly.size(); i += 2) Q[i >> 1] = T_poly[i];
        } else {
            for (int i = 0; i < (int)S_poly.size(); i += 2) P[i >> 1] = S_poly[i];
            for (int i = 0; i < (int)T_poly.size(); i += 2) Q[i >> 1] = T_poly[i];
        }
        n >>= 1;
    }
    return P[0];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int k; long long n; cin >> k >> n; n--;
    vector<int> S(k), C(k);
    for (int i = 0; i < k; i++) cin >> S[i];
    for (int i = 0; i < k; i++) cin >> C[i];
    cout << LinearRecurrence(S, C, n) << '\n';
    return 0;
}

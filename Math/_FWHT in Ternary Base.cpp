/*
 * Problem it Solves: FWHT in base 3.
 * Usage: FWHT::multiply(n, a, b).
 * Input/Output: vectors a, b.
 * Time Complexity: O(3^BIT * BIT).
 * Tweaks: BIT defines size. MOD must be 1 mod 3.
 * Other Use Cases: Ternary logic problems.
 * Warnings: M must be power of 3.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 1e9 + 7;

struct cmplx {
    int a, b;
    cmplx(int _a = 0, int _b = 0) { a = _a, b = _b; }
    cmplx operator+(cmplx x) { return cmplx((a + x.a) % MOD, (b + x.b) % MOD); }
    cmplx operator*(int k) { return cmplx(1LL * a * k % MOD, 1LL * b * k % MOD); }
    cmplx operator*(cmplx x) {
        int na = (1LL * a * x.a - 1LL * b * x.b) % MOD;
        if (na < 0) na += MOD;
        int nb = (1LL * a * x.b + 1LL * b * x.a - 1LL * b * x.b) % MOD;
        if (nb < 0) nb += MOD;
        return cmplx(na, nb);
    }
};

cmplx w(0, 1), w2(MOD - 1, MOD - 1);

typedef vector<cmplx> poly;

struct FWHT {
    const int BIT = 11; // log3(MAX VALUE)
    const int M = 177147; // 3^BIT, it must be a power of 3
    
    void fwht(poly &coefs, poly &vals, bool invert = false) {
        vals = coefs;
        int n = vals.size();
        for (int len = 1; len < n; len *= 3) {
            int pitch = len * 3;
            int len2 = len * 2;
            for (int i = 0; i < n; i += pitch) {
                for (int j = 0; j < len; j++) {
                    cmplx a = vals[i + j];
                    cmplx b = vals[i + j + len];
                    cmplx c = vals[i + j + len2];
                    vals[i + j] = a + b + c;
                    vals[i + j + len] = a + b * w + c * w2;
                    vals[i + j + len2] = a + b * w2 + c * w;
                    if(invert) swap(vals[i + j + len], vals[i + j + len2]);
                }
            }
        }
        int inv3 = (MOD + 1) / 3;
        int inv = 1;
        for(int i = 1; i < n; i *= 3) inv = 1LL * inv * inv3 % MOD;
        if (invert) for (int i = 0; i < n; i++) vals[i] = vals[i] * inv;
    }
    
    cmplx power(cmplx x, long long n) {
        cmplx ret(1);
        while (n) {
            if(n & 1) ret = ret * x;
            x = x * x;
            n >>= 1;
        }
        return ret;
    }
    
    int tobase3(int x) {
        int ret = 0;
        for(int i = BIT - 1; i >= 0; i--){
            ret *= 3;
            if(x & (1 << i)) ret++;
        }
        return ret;
    }
    
    poly convert(int n, vector<int> &a){
        poly p(M);
        for(int i = 0; i < n; i++){
            int x = tobase3(i);
            assert(x < M);
            p[x].a = (p[x].a + a[i]) % MOD;
        }
        return p;
    }
    
    vector<int> multiply(int n, vector<int> &a, vector<int> &b) {
        poly p = convert(n, a), q = convert(n, b);
        fwht(p, p); fwht(q, q);
        for(int i = 0; i < M; i++) p[i] = p[i] * q[i];
        fwht(p, p, true);
        vector<int> ans;
        for(int i = 0; i < M; i++) ans.push_back(p[i].a);
        return ans;
    }
} t;

int main() {
    // Example usage
    return 0;
}

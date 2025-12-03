/*
 * Problem it Solves: Finite Field Arithmetic GF(2^B).
 * Usage: add(a, b), mul(a, b), eval(poly).
 * Input/Output: Polynomial string -> evaluation.
 * Time Complexity: O(B) for mul.
 * Tweaks: B size. mask must match B.
 * Other Use Cases: Schwartz-Zippel Lemma check for multivariate polynomials.
 * Warnings: Uses polynomial representation of fields.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 44;

// https://en.wikipedia.org/wiki/Finite_field_arithmetic
namespace FFA {
    const int B = 30, mask = (1 << B) - 1; // KEY: B=30, mask=2^30-1
    inline int add(int a, int b) { return a ^ b; }
    inline int sub(int a, int b) { return a ^ b; }
    inline int mul(int a, int b) {
        int ret = 0;
        while (b) {
            if (b & 1) ret = add(ret, a);
            b >>= 1;
            a <<= 1;
            // KEY: Field reduction polynomial. Here x^B + x + 1 is implicit?
            // Actually (1<<B)^1 implies x^B + 1.
            if (a & (1 << B)) a ^= (1 << B) ^ 1; 
        }
        return ret;
    }
    inline int power(int a, long long k) {
        int ret = 1;
        while (k) {
            if (k & 1) ret = mul(ret, a);
            k >>= 1;
            a = mul(a, a);
        }
        return ret;
    }
}
using namespace FFA;

int mp[40];
int eval(string s) {
    int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '0') {
            i++;
            continue;
        }
        int cur = 1;
        if (s[i] == '1') i++;
        else {
            while (s[i] && s[i] != '+') {
                int p = 0;
                char x = s[i];
                while (s[i] == x) i++, p++;
                cur = mul(cur, power(mp[x - 'a'], p));
            }
        }
        ans = add(ans, cur);
    }
    return ans;
}

int a[N][N];
bool detZero(int m[N][N], int d) {
    for (int i = 0; i < d; ++i) {
        int piv = i;
        while (piv < d && !m[piv][i]) ++piv;
        if (piv == d) return true;
        for (int j = i; j < d; ++j) swap(m[i][j], m[piv][j]);
        for (int j = i + 1; j < d; ++j)
            if (m[j][i]) {
                // Elimination without division in ring/field?
                // m[j][k] = m[i][i]*m[j][k] - m[j][i]*m[i][k]
                for (int k = d - 1; k >= i; --k)
                    m[j][k] = add(mul(m[i][i], m[j][k]), mul(m[j][i], m[i][k]));
            }
    }
    return false;
}

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
string mat[N][N];

bool check(int n) {
    for (int i = 0; i < 30; i++) mp[i] = rnd() & mask;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = eval(mat[i][j]);
        }
    }
    return detZero(a, n);
}

int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) cin >> mat[i][j];
        }
        bool ans = true;
        for (int i = 0; i < 20; i++) {
            ans &= check(n);
        }
        if (!ans) cout << "Some Odd\n";
        else cout << "All Even\n";
    }
    return 0;
}

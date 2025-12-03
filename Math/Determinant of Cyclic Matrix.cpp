/*
 * Problem it Solves: Computes Determinant of a matrix formed by a cyclic permutation (and variations).
 * Usage: Solves Det(A) where A is formed by permuting a row vector.
 * Input/Output: vector a, permutation p.
 * Time Complexity: O(N^2) for resultant calculation.
 * Tweaks: MOD constant.
 * Other Use Cases: Circulant matrices.
 * Warnings: Requires polynomial arithmetic (modulo).
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
using Poly = vector<int>;

int power(int a, int b) {
    int res = 1;
    for (; b; b >>= 1, a = 1LL * a * a % MOD)
        if (b & 1) res = 1LL * res * a % MOD;
    return res;
}

int modInverse(int n) {
    return power(n, MOD - 2);
}

// Polynomial modulo b
Poly operator%(Poly a, const Poly &b) {
    assert(!b.empty());
    int m = b.size() - 1, n = a.size() - 1;
    int inv = modInverse(b[m]);
    while (n >= m) {
        int x = 1LL * inv * (MOD - a[n]) % MOD;
        for (int j = 0; j < m; ++j)
            a[n - m + j] = (a[n - m + j] + 1LL * x * b[j]) % MOD;
        a.pop_back();
        while (!a.empty() && a.back() == 0)
            a.pop_back();
        n = a.size() - 1;
    }
    return a;
}

// Computes Resultant(A, B)
// Resultant(A, B) = b[m]^n * prod(A(roots of B))
int resultant(const Poly &a, const Poly &b) {
    if (b.empty()) return 0;
    if (b.size() == 1) return power(b[0], a.size() - 1);
    auto c = a % b;
    int term = 1LL * power(b.back(), a.size() - c.size()) * (((a.size() & 1) | (b.size() & 1)) ? 1 : MOD - 1) % MOD;
    return 1LL * term * resultant(b, c) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n), p(n); // for cyclic matrix set p accordingly
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) { cin >> p[i]; --p[i]; }
    
    for (int i = 0, x = 0; i < n - 1; ++i) {
        x = p[x];
        if (x == 0) { // if permutation has more than one cycle than ans is 0 (for this specific problem)
            cout << 0 << "\n";
            return 0;
        }
    }
    vector<int> q(n);
    for (int i = n - 1, x = 0; i >= 0; --i) {
        q[i] = x;
        x = p[x];
    }
    vector<int> vis(n);
    int coef = (n & 1) ? MOD - 1 : 1;
    for (int i = 0; i < n; ++i) {
        if (vis[i]) continue;
        for (int j = i; !vis[j]; j = q[j]) vis[j] = 1;
        coef = MOD - coef;
    }
    vector<int> b(n);
    for (int i = 0; i < n; ++i) b[i] = a[q[i]];
    
    // Characteristic polynomial of permutation matrix is x^n - 1 (if single cycle)
    // Here we compute Resultant(x^n - 1, Sum b_i x^i)
    // Because Det(C) = Prod (Sum b_j w^(jk))
    Poly P_char(n + 1, 0);
    P_char[0] = MOD - 1;
    P_char[n] = 1;
    
    int ans = resultant(P_char, b);
    ans = 1LL * ans * coef % MOD;
    cout << ans << "\n";
    return 0;
}

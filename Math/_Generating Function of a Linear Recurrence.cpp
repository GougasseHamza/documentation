/*
 * Problem it Solves: Finds generating function of a linear recurrence and computes k-th term using derivative trick?
 * Actually, this code calculates the limit value of a rational function P(x)/Q(x) at x=1? 
 * The code computes (v * du - u * dv) / (v * v) which is derivative of u/v at x=1?
 * Usage: yo(a, c).
 * Input/Output: Recurrence coefficients -> Value.
 * Time Complexity: O(N^2) for BM.
 * Tweaks: MOD.
 * Other Use Cases: Probability on graphs.
 * Warnings: Specific to problem structure.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e3 + 9, MOD = 998244353;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

// Berlekamp-Massey Algorithm
// Returns the coefficients C of the recurrence relation: S[i] = sum(C[j] * S[i-j])
vector<int> BerlekampMassey(vector<int> S) {
    int n = (int)S.size(), L = 0, m = 0;
    vector<int> C(n), B(n), T;
    C[0] = B[0] = 1;
    int b = 1;
    for (int i = 0; i < n; i++) {
        ++m;
        int d = S[i];
        for (int j = 1; j <= L; j++) d = (d + 1LL * C[j] * S[i - j]) % MOD;
        if (d == 0) continue;
        T = C;
        int coef = 1LL * d * modInverse(b) % MOD;
        for (int j = m; j < n; j++) C[j] = (C[j] - 1LL * coef * B[j - m] % MOD + MOD) % MOD;
        if (2 * L > i) continue;
        L = i + 1 - L; B = T; b = d; m = 0;
    }
    C.resize(L + 1); C.erase(C.begin());
    for (auto &x : C) x = (MOD - x) % MOD;
    return C;
}

// Solves specific problem: Expected value calculation on graph using GF
int yo(vector<int> a, vector<int> c) {
    int n = a.size();
    if (!n) return 0;
    vector<int> p(n + 1, 0);
    p[0] = 1;
    for (int i = 0; i < n; i++) p[i + 1] = (MOD - c[i]) % MOD;
    
    vector<int> up(n + n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            up[i + j] = (up[i + j] + 1LL * a[i] * p[j]) % MOD;
        }
    }
    up.resize(n);
    
    // Calculate (u/v)' = (v*u' - u*v') / v^2 at x = 1
    int v = 0, u = 0, du = 0, dv = 0;
    for (int i = 0; i < p.size(); i++) {
        v = (v + p[i]) % MOD;
        dv = (dv + 1LL * p[i] * i) % MOD;
    }
    for (int i = 0; i < up.size(); i++) {
        u = (u + up[i]) % MOD;
        du = (du + 1LL * up[i] * i) % MOD;
    }
    
    int num = (1LL * v * du - 1LL * u * dv) % MOD;
    if (num < 0) num += MOD;
    int den = 1LL * v * v % MOD;
    return 1LL * num * modInverse(den) % MOD;
}

int dp[N * 2][N];
vector<int> g[N];
int deg[N];
int ideg[N];

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    for (int i = 1; i <= n; i++) {
        int x, y; cin >> x >> y; // Coordinates not used
    }
    int m; cin >> m;
    while (m--) {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
        ++deg[u]; ++deg[v];
    }
    for (int i = 1; i <= n; i++) ideg[i] = modInverse(deg[i]);
    
    dp[0][1] = 1;
    for (int k = 1; k <= n * 2; k++) {
        for (int u = 1; u <= n; u++) {
            if (u == n) continue;
            for (auto v : g[u]) {
                dp[k][v] = (dp[k][v] + 1LL * dp[k - 1][u] * ideg[u]) % MOD;
            }
        }
    }
    vector<int> cur;
    for (int i = 0; i <= 2 * n; i++) cur.push_back(dp[i][n]);
    
    auto tr = BerlekampMassey(cur);
    cur.resize(tr.size());
    cout << yo(cur, tr) << '\n';
    return 0;
}

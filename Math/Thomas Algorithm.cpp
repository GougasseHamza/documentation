/*
 * Problem it Solves: Solves Tridiagonal System of Equations (TDMA / Thomas Algorithm).
 * Usage: Thomas(matrix) returns solution.
 * Input/Output: Matrix (n x 4, columns: a, b, c, d) -> solution vector.
 * Time Complexity: O(N).
 * Tweaks: MOD.
 * Other Use Cases: Spline interpolation, Poisson equation.
 * Warnings: Unstable if diagonal dominance not satisfied.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = 1LL * ans * n % MOD;
        n = 1LL * n * n % MOD;
        k >>= 1;
    }
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

// matrix form:
// b1 c1 0  0 | d1
// a2 b2 c2 0 | d2
// 0  a3 b3 c3| d3
// Input a[i] = {a_i, b_i, c_i, d_i}
// Note: a[0][0] is unused (a_1), a[n-1][2] is unused (c_n)
vector<int> Thomas(vector<vector<int>> a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int x = 1LL * a[i][0] * modInverse(a[i - 1][1]) % MOD;
        a[i][1] = (a[i][1] - 1LL * x * a[i - 1][2] % MOD + MOD) % MOD;
        a[i][3] = (a[i][3] - 1LL * x * a[i - 1][3] % MOD + MOD) % MOD;
    }
    for (int i = n - 2; i >= 0; i--) {
        int x = 1LL * a[i][2] * modInverse(a[i + 1][1]) % MOD;
        a[i][3] = (a[i][3] - 1LL * x * a[i + 1][3] % MOD + MOD) % MOD;
    }
    vector<int> ans;
    for (int i = 0; i < n; i++) {
        ans.push_back(1LL * a[i][3] * modInverse(a[i][1]) % MOD);
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    long long sum = 0;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        sum += a[i];
    }
    int m = sum % MOD;
    vector<vector<int>> mat(m + 1);
    for (int i = 0; i <= m; i++) {
        vector<int> v(4, 0);
        int inv_m = modInverse(m);
        if (i) v[0] = 1LL * i * inv_m % MOD;
        
        // v[1] = (m - i) * (n - 2) / m / (n - 1) - 1;
        int term1 = 1LL * (m - i + MOD) % MOD * (n - 2) % MOD;
        int term2 = 1LL * m * (n - 1) % MOD;
        v[1] = (1LL * term1 * modInverse(term2) % MOD - 1 + MOD) % MOD;
        
        if (i + 1 <= m) {
            v[2] = 1LL * (m - i + MOD) % MOD * modInverse(term2) % MOD;
        }
        v[3] = 1LL * (MOD - i) * inv_m % MOD;
        mat[i] = v;
    }
    vector<int> f = Thomas(mat);
    int ans = 0;
    for (int i = 0; i < n; i++) ans = (ans + f[a[i]]) % MOD;
    
    int sub = (1LL * (n - 1) * f[0] + f[m]) % MOD;
    ans = (ans - sub + MOD) % MOD;
    cout << ans << '\n';
    return 0;
}

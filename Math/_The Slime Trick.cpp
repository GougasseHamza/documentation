/*
 * Problem it Solves: The Slime Trick (Expected Value on Graph). Uses Thomas Algorithm.
 * Usage: Solves specific problem where states depend on neighbors (tridiagonal-like).
 * Input/Output: N -> Expected Value.
 * Time Complexity: O(N).
 * Tweaks: MOD.
 * Other Use Cases: 1D Random Walk.
 * Warnings: Specific recurrence structure.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 998244353;

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

// Thomas algorithm for solving tri-diagonal system
// a[i][0]*x_{i-1} + a[i][1]*x_i + a[i][2]*x_{i+1} = a[i][3]
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

int a[N];
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    long long m = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        m += a[i];
    }
    int M = m % MOD; // Assuming M fits in int for vector size logic
    // Actually m can be large, but the problem implies m is relevant for states
    // If m is very large, vector size fails. Problem constraints say m is manageable?
    // Or recurrence is on 'i' up to 'm'.
    
    vector<vector<int>> mat(m + 1);
    for (int i = 0; i <= m; i++) {
        vector<int> v(4, 0);
        if (i) v[0] = 1LL * i * modInverse(M) % MOD;
        
        // v[1] = (m - i) * (n - 2) / m / (n - 1) - 1;
        int term1 = 1LL * (m - i) % MOD * (n - 2) % MOD;
        int term2 = 1LL * M * (n - 1) % MOD;
        v[1] = (1LL * term1 * modInverse(term2) % MOD - 1 + MOD) % MOD;
        
        if (i + 1 <= m) {
            // v[2] = (m - i) / m / (n - 1);
            term1 = (m - i) % MOD;
            v[2] = 1LL * term1 * modInverse(term2) % MOD;
        }
        v[3] = (1LL * -i % MOD * modInverse(M) % MOD + MOD) % MOD;
        mat[i] = v;
    }
    
    vector<int> f = Thomas(mat);
    int ans = 0;
    for (int i = 0; i < n; i++) ans = (ans + f[a[i]]) % MOD;
    
    // ans -= (n - 1) * f[0] + f[m];
    int sub1 = 1LL * (n - 1) * f[0] % MOD;
    int sub = (sub1 + f[m]) % MOD;
    ans = (ans - sub + MOD) % MOD;
    
    cout << ans << '\n';
    return 0;
}

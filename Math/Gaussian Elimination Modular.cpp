/*
 * Problem it Solves: Solves system of linear equations Ax = B modulo P.
 * Usage: Gauss(A, ans) returns number of free variables or -1 (no solution).
 * Input/Output: Augmented Matrix A (last column is B), ans vector.
 * Time Complexity: O(N^3).
 * Tweaks: MOD constant.
 * Other Use Cases: finding inverse, rank.
 * Warnings: MOD must be prime for inverse.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

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

// A is n x (m+1) augmented matrix
int Gauss(vector<vector<int>> a, vector<int> &ans) {
    int n = a.size(), m = (int)a[0].size() - 1;
    vector<int> pos(m, -1);
    int free_var = 0;
    
    int row = 0;
    for (int col = 0; col < m && row < n; col++) {
        int mx = row;
        for (int k = row; k < n; k++) if (a[k][col] > a[mx][col]) mx = k;
        if (a[mx][col] == 0) continue;
        
        for (int j = col; j <= m; j++) swap(a[mx][j], a[row][j]);
        pos[col] = row;
        
        int inv = modInverse(a[row][col]);
        for (int i = 0; i < n; i++) {
            if (i != row && a[i][col]) {
                int x = 1LL * a[i][col] * inv % MOD;
                for (int j = col; j <= m; j++) {
                    a[i][j] = (a[i][j] - 1LL * a[row][j] * x % MOD + MOD) % MOD;
                }
            }
        }
        row++;
    }
    
    ans.assign(m, 0);
    for (int i = 0; i < m; i++) {
        if (pos[i] == -1) free_var++;
        else ans[i] = 1LL * a[pos[i]][m] * modInverse(a[pos[i]][i]) % MOD;
    }
    
    // Check for validity
    for (int i = 0; i < n; i++) {
        long long val = 0;
        for (int j = 0; j < m; j++) val = (val + 1LL * ans[j] * a[i][j]) % MOD;
        if (val != a[i][m]) return -1; // no solution
    }
    return free_var; // has solution
}

int main() {
    int n, m; cin >> n >> m;
    vector<vector<int>> a(n, vector<int>(m + 1));
    for (int i = 0; i < n; i++) for (int j = 0; j <= m; j++) cin >> a[i][j];
    vector<int> ans;
    int k = Gauss(a, ans);
    if (k == -1) cout << "no solution\n";
    else {
        for (auto x : ans) cout << x << '\n';
    }
    return 0;
}

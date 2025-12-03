/*
 * Problem it Solves: Computes Determinant modulo prime p using Gaussian Elimination with modular inverse.
 * Usage: Gauss(matrix) returns determinant.
 * Input/Output: matrix -> determinant.
 * Time Complexity: O(N^3).
 * Tweaks: MOD constant.
 * Other Use Cases: Linear basis, System of linear equations.
 * Warnings: MOD must be prime for inverse.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}

int modInverse(int n) {
    return power(n, MOD - 2);
}

int Gauss(vector<vector<int>> a) {
    int n = a.size(), m = (int)a[0].size();
    int det = 1;
    for (int col = 0, row = 0; col < m && row < n; col++) {
        int mx = row;
        for (int k = row; k < n; k++) if (a[k][col] > a[mx][col]) mx = k;
        if (a[mx][col] == 0) {
            det = 0; 
            continue;
        }
        for (int j = col; j < m; j++) swap(a[mx][j], a[row][j]);
        if (row != mx) det = det == 0 ? 0 : MOD - det;
        det = 1LL * det * a[row][col] % MOD;
        
        int inv = modInverse(a[row][col]);
        for (int i = 0; i < n; i++) {
            if (i != row && a[i][col]) {
                int x = 1LL * a[i][col] * inv % MOD;
                for (int j = col; j < m; j++) {
                    a[i][j] = (a[i][j] - 1LL * a[row][j] * x % MOD + MOD) % MOD;
                }
            }
        }
        row++;
    }
    return det;
}

int main() {
    int n; cin >> n;
    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> a[i][j];
    cout << Gauss(a) << '\n';
    return 0;
}

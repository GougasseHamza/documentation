/*
 * Problem it Solves: Computes Inverse of a Matrix modulo P using Gauss-Jordan.
 * Usage: Gauss(A, ans, mod) fills ans with inverse.
 * Input/Output: Matrix A, MOD -> bool (invertible?), ans (Inverse).
 * Time Complexity: O(N^3).
 * Tweaks: MOD.
 * Other Use Cases: Linear Algebra.
 * Warnings: MOD must be prime.
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

// Returns true if invertible, ans contains A^-1
bool Gauss(vector<vector<int>> a, vector<vector<int>> &ans) {
    int n = (int)a.size();
    ans.assign(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) ans[i][i] = 1;
    
    for (int i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && a[pivot][i] == 0) pivot++;
        if (pivot == n) return false; // Singular
        
        swap(a[i], a[pivot]);
        swap(ans[i], ans[pivot]);
        
        int inv = modInverse(a[i][i]);
        for (int j = 0; j < n; j++) {
            a[i][j] = 1LL * a[i][j] * inv % MOD;
            ans[i][j] = 1LL * ans[i][j] * inv % MOD;
        }
        
        for (int k = 0; k < n; k++) {
            if (k != i) {
                int factor = a[k][i];
                for (int j = 0; j < n; j++) {
                    a[k][j] = (a[k][j] - 1LL * factor * a[i][j] % MOD + MOD) % MOD;
                    ans[k][j] = (ans[k][j] - 1LL * factor * ans[i][j] % MOD + MOD) % MOD;
                }
            }
        }
    }
    return true;
}

int main() {
    int n;
    while (cin >> n && n) {
        vector<vector<int>> a(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) cin >> a[i][j];
            
        vector<vector<int>> ans;
        if (!Gauss(a, ans)) {
            cout << "singular\n";
        } else {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) cout << ans[i][j] << ' ';
                cout << '\n';
            }
        }
    }
    return 0;
}

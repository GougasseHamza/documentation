/*
 * Problem it Solves: Computes Determinant of M where M(i, j) = a[i]*b[j] + (i==j ? x : 0).
 * Usage: Formula implementation.
 * Input/Output: n, x, arrays a and b -> determinant.
 * Time Complexity: O(N).
 * Tweaks: MOD constant.
 * Other Use Cases: Matrix Tree Theorem with specific graph structure.
 * Warnings: x is added to diagonal.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

// Given two arrays and x
// Matrix M(i, j) = a(i) * b(j) if i != j, (x + a(i) * b(j)) if i == j
// This is a rank-1 update to a diagonal matrix (xI + a*b^T).
// By Matrix Determinant Lemma: det(A + uv^T) = det(A) + v^T adj(A) u
// Here A = xI. det(xI + ab^T) = x^n + x^(n-1) * (b^T a) = x^n + x^(n-1) * sum(a[i]*b[i]).

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, x;
    while (cin >> n >> x) {
        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int i = 0; i < n; i++) cin >> b[i];
        
        int sum_ab = 0;
        for (int i = 0; i < n; i++) {
            sum_ab = (sum_ab + 1LL * a[i] * b[i]) % MOD;
        }
        
        // Calculate x^(n-1)
        int p = 1;
        for (int i = 0; i < n - 1; i++) p = 1LL * p * x % MOD;
        
        // ans = x^n + x^(n-1) * sum_ab = x^(n-1) * (x + sum_ab)
        int ans = 1LL * p * ((x + sum_ab) % MOD) % MOD;
        
        cout << ans << '\n';
    }
    return 0;
}

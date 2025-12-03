/*
 * Problem it Solves: Solves system of linear equations over Reals using Gaussian Elimination.
 * Usage: Gauss(A, ans) returns status (0: no sol, 1: unique, 2: infinite).
 * Input/Output: Matrix A (Augmented), ans vector.
 * Time Complexity: O(N^3).
 * Tweaks: eps.
 * Other Use Cases: Computational geometry.
 * Warnings: Precision issues.
 */

#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-9;

int Gauss(vector<vector<double>> a, vector<double> &ans) {
    int n = (int)a.size(), m = (int)a[0].size() - 1;
    vector<int> pos(m, -1);
    
    int row = 0;
    for (int col = 0; col < m && row < n; ++col) {
        int mx = row;
        for (int i = row; i < n; i++) if (fabs(a[i][col]) > fabs(a[mx][col])) mx = i;
        if (fabs(a[mx][col]) < eps) continue;
        
        for (int i = col; i <= m; i++) swap(a[row][i], a[mx][i]);
        pos[col] = row;
        
        for (int i = 0; i < n; i++) {
            if (i != row && fabs(a[i][col]) > eps) {
                double c = a[i][col] / a[row][col];
                for (int j = col; j <= m; j++) a[i][j] -= a[row][j] * c;
            }
        }
        ++row;
    }
    
    ans.assign(m, 0);
    for (int i = 0; i < m; i++) {
        if (pos[i] != -1) ans[i] = a[pos[i]][m] / a[pos[i]][i];
    }
    
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < m; j++) sum += ans[j] * a[i][j];
        if (fabs(sum - a[i][m]) > eps) return -1; // no solution
    }
    
    for (int i = 0; i < m; i++) if (pos[i] == -1) return 2; // infinite solutions
    return 1; // unique solution
}

int main() {
    int n, m; cin >> n >> m;
    vector<vector<double>> v(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= m; j++) {
            double x; cin >> x; v[i].push_back(x);
        }
    }
    vector<double> ans;
    int k = Gauss(v, ans);
    if (k != -1) for (int i = 0; i < m; i++) cout << fixed << setprecision(5) << ans[i] << ' ';
    else cout << "no solution\n";
    return 0;
}

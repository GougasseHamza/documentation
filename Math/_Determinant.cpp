/*
 * Problem it Solves: Computes Determinant of a real matrix using Gaussian Elimination.
 * Usage: Gauss(matrix) returns determinant.
 * Input/Output: Matrix (vector<vector<double>>) -> determinant (double).
 * Time Complexity: O(N^3).
 * Tweaks: eps for precision.
 * Other Use Cases: Checking invertibility.
 * Warnings: Floating point errors.
 */

#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-9;

double Gauss(vector<vector<double>> a) {
    int n = (int)a.size(), m = (int)a[0].size();
    double det = 1;
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        int mx = row;
        for (int i = row; i < n; i++) if (fabs(a[i][col]) > fabs(a[mx][col])) mx = i;
        if (fabs(a[mx][col]) < eps) {
            det = 0; 
            continue; 
        }
        for (int i = col; i < m; i++) swap(a[row][i], a[mx][i]);
        if (row != mx) det = -det;
        det *= a[row][col];
        for (int i = 0; i < n; i++) {
            if (i != row && fabs(a[i][col]) > eps) {
                double c = a[i][col] / a[row][col];
                for (int j = col; j < m; j++) a[i][j] -= a[row][j] * c;
            }
        }
        ++row;
    }
    return det;
}

int main() {
    int n, m; cin >> n >> m;
    vector<vector<double>> v(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double x; cin >> x; v[i].push_back(x);
        }
    }
    cout << Gauss(v) << '\n';
    return 0;
}

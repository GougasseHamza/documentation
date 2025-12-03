/*
 * Problem it Solves: Probabilistic check if A * B = C for matrices A, B, C.
 * Usage: Freivalds(A, B, C) returns 1 if A*B=C (high prob), 0 otherwise.
 * Input/Output: Matrices -> boolean.
 * Time Complexity: O(Iterations * N^2).
 * Tweaks: Iteration count.
 * Other Use Cases: Checking matrix products.
 * Warnings: Probabilistic (error rate 2^-iterations).
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 1e9 + 7;

struct Mat {
    int n, m;
    vector<vector<int>> a;
    Mat(int _n, int _m) { n = _n; m = _m; a.assign(n, vector<int>(m, 0)); }
    
    Mat operator + (const Mat &b) {
        Mat ans(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ans.a[i][j] = (a[i][j] + b.a[i][j]) % MOD;
        return ans;
    }
    Mat operator - (const Mat &b) {
        Mat ans(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ans.a[i][j] = (a[i][j] - b.a[i][j] + MOD) % MOD;
        return ans;
    }
    Mat operator * (const Mat &b) {
        assert(m == b.n);
        Mat ans(n, b.m);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < m; k++)
                for (int j = 0; j < b.m; j++)
                    ans.a[i][j] = (ans.a[i][j] + 1LL * a[i][k] * b.a[k][j]) % MOD;
        return ans;
    }
    bool operator != (const Mat &b) { return a != b.a; }
};

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

// check if two n*n matrix a*b=c within complexity (iteration*n^2)
// probability of error 2^(-iteration)
int Freivalds(Mat a, Mat b, Mat c) {
    int n = a.n, iteration = 40;
    Mat zero(n, 1), r(n, 1);
    while (iteration--) {
        for (int i = 0; i < n; i++) r.a[i][0] = rnd() % 2;
        // Check if A * (B * r) - C * r == 0
        Mat br = b * r;
        Mat abr = a * br;
        Mat cr = c * r;
        Mat ans = abr - cr;
        if (ans != zero) return 0;
    }
    return 1;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, q; cin >> n >> q;
    Mat a(n, n);
    Mat c(n, n); // Temporary placeholder
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) cin >> a.a[i][j];
        
    while (q--) {
        int ty; cin >> ty;
        if (ty == 1) {
            int x1, y1, x2, y2, v; cin >> x1 >> y1 >> x2 >> y2 >> v;
            for (int i = x1; i <= x2; i++)
                for (int j = y1; j <= y2; j++)
                    a.a[i][j] = (a.a[i][j] + v) % MOD;
        } else {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) cin >> c.a[i][j];
            cout << (Freivalds(a, a, c) ? "yes\n" : "no\n");
        }
    }
    return 0;
}

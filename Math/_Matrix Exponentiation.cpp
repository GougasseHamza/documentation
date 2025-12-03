/*
 * Problem it Solves: Matrix Exponentiation.
 * Usage: Mat m(n, n); m.pow(k) returns M^k.
 * Input/Output: n, k, matrix -> matrix^k.
 * Time Complexity: O(N^3 log K).
 * Tweaks: MOD constant. Structure Mat.
 * Other Use Cases: Solving Linear Recurrences, counting paths.
 * Warnings: Matrix multiplication is O(N^3).
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

struct Mat {
    int n, m;
    vector<vector<int>> a;
    Mat() { }
    Mat(int _n, int _m) { n = _n; m = _m; a.assign(n, vector<int>(m, 0)); }
    Mat(vector<vector<int>> v) { n = v.size(); m = n ? v[0].size() : 0; a = v; }
    
    void make_unit() {
        assert(n == m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) a[i][j] = (i == j);
        }
    }
    
    Mat operator + (const Mat &b) {
        assert(n == b.n && m == b.m);
        Mat ans = Mat(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans.a[i][j] = (a[i][j] + b.a[i][j]) % MOD;
            }
        }
        return ans;
    }
    
    Mat operator - (const Mat &b) {
        assert(n == b.n && m == b.m);
        Mat ans = Mat(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ans.a[i][j] = (a[i][j] - b.a[i][j] + MOD) % MOD;
            }
        }
        return ans;
    }
    
    Mat operator * (const Mat &b) {
        assert(m == b.n);
        Mat ans = Mat(n, b.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < b.m; j++) {
                for (int k = 0; k < m; k++) {
                    ans.a[i][j] = (ans.a[i][j] + 1LL * a[i][k] * b.a[k][j]) % MOD;
                }
            }
        }
        return ans;
    }
    
    Mat pow(long long k) {
        assert(n == m);
        Mat ans(n, n), t = *this;
        ans.make_unit();
        while (k) {
            if (k & 1) ans = ans * t;
            t = t * t;
            k >>= 1;
        }
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; long long k; cin >> n >> k;
    Mat a(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a.a[i][j];
        }
    }
    Mat ans = a.pow(k);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << ans.a[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}

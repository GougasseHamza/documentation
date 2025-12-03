/*
 * Problem it Solves: Computes Hafnian of a symmetric matrix (SQRT of Permanent of zero-diagonal symmetric matrix).
 * Counts perfect matchings in general graph.
 * Usage: Hafnian::solve(matrix).
 * Input/Output: Matrix -> Hafnian modulo MOD.
 * Time Complexity: O(N^3) empirically, formally exponential/polynomial depending on algo variant. This implementation is optimized recursive.
 * Tweaks: MOD constant.
 * Other Use Cases: Quantum computing simulation.
 * Warnings: Matrix must be symmetric, even dimension.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int add(int a, int b) { return (a + b) % MOD; }
int mul(int a, int b) { return 1LL * a * b % MOD; }
int sub(int a, int b) { return (a - b + MOD) % MOD; }

namespace Hafnian {
    int h;
    void add_vec(vector<int>& ans, const vector<int>& a, const vector<int>& b) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < h - 1 - i; j++) {
                ans[i + j + 1] = add(ans[i + j + 1], mul(a[i], b[j]));
            }
        }
    }
    
    vector<int> yo(const vector<vector<vector<int>>>& v) {
        vector<int> ans(h, 0);
        if (v.empty()) {
            ans[0] = 1; return ans;
        }
        int m = (int)v.size() - 2;
        auto V = v; V.resize(m);
        vector<int> zero = yo(V);
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < i; j++) {
                // V[i][j] += v[m][i] * v[m+1][j] + v[m+1][i] * v[m][j]
                add_vec(V[i][j], v[m][i], v[m + 1][j]);
                add_vec(V[i][j], v[m + 1][i], v[m][j]);
            }
        }
        
        vector<int> one = yo(V);
        for (int i = 0; i < h; i++) ans[i] = add(ans[i], sub(one[i], zero[i]));
        add_vec(ans, one, v[m + 1][m]);
        return ans;
    }
    
    // Number of Perfect Matchings of a general graph
    // SQRT of the Permanent of a zero diagonal symmetric matrix
    // Takes ~2s for (38 * 38) matrix
    int solve(vector<vector<int>> a) {
        int n = a.size();
        assert(n % 2 == 0);
        h = n / 2 + 1;
        vector<vector<vector<int>>> v(n);
        for (int i = 0; i < n; i++) {
            v[i].resize(i);
            for (int j = 0; j < i; j++) {
                v[i][j] = vector<int>(h, 0);
                v[i][j][0] = a[i][j];
            }
        }
        return yo(v).back();
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cin >> a[i][j];
    }
    cout << Hafnian::solve(a) << '\n';
    return 0;
}

/*
 * Problem it Solves: Lagrange Interpolation (O(N^2)) to find value at N given K+1 points.
 * Usage: Lagrange(x_pts, y_pts) returns polynomial. Then evaluate.
 * Input/Output: points -> polynomial coefficients.
 * Time Complexity: O(K^2).
 * Tweaks: MOD.
 * Other Use Cases: Recovering secret sharing.
 * Warnings: Slow for large K.
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

// generates the polynomial explicitly
vector<int> Lagrange(vector<int> &x, vector<int> &y) {
    int n = x.size();
    vector<int> ans(n, 0);
    vector<int> all(n + 1, 0); // (x - x0) * (x - x1) * ... * (x - x(n-1))
    all[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = n; j >= 0; j--) {
            all[j] = 1LL * all[j] * -x[i] % MOD;
            if (j) all[j] = (all[j] + all[j - 1]) % MOD;
        }
    }
    for (int i = 0; i < n; i++) {
        vector<int> up(n); // all / (x - xi)
        int rem = all[n];
        for (int j = n - 1; j >= 0; --j) {
            up[j] = rem;
            rem = (all[j] + 1LL * rem * x[i]) % MOD;
        }
        int down = 1;
        for (int j = 0; j < n; j++) if (i != j) down = 1LL * down * (x[i] - x[j]) % MOD;
        down = 1LL * modInverse(down) * y[i] % MOD;
        for (int j = 0; j < n; j++) ans[j] = (ans[j] + 1LL * up[j] * down) % MOD;
    }
    for (auto &v : ans) if (v < 0) v += MOD;
    return ans;
}

int main() {
    int n, k; cin >> n >> k;
    vector<int> y, x;
    int sum = 0; y.push_back(0); x.push_back(0);
    for (int i = 1; i <= k + 1; i++) {
        sum = (sum + power(i, k)) % MOD;
        y.push_back(sum); x.push_back(i);
    }
    auto p = Lagrange(x, y);
    int ans = 0;
    for (int i = 0; i < p.size(); i++) ans = (ans + 1LL * p[i] * power(n, i)) % MOD;
    cout << ans << '\n';
    return 0;
}

/*
 * Problem it Solves: Sum of powers of AP terms: sum_{x=0}^{n} x^k1 * floor((ax + b)/c)^k2.
 * Usage: solve(k1, k2, a, b, c, n).
 * Input/Output: k1, k2, a, b, c, n -> matrix.
 * Time Complexity: O((k1+k2)^3 log n).
 * Tweaks: N defines max degree.
 * Other Use Cases: Advanced counting.
 * Warnings: Complex recurrence.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 25, MOD = 1e9 + 7;

long long power(long long n, long long k) {
    int ans = 1 % MOD;
    n %= MOD;
    if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}

long long modInverse(long long n) {
    return power(n, MOD - 2);
}

// generates the polynomial explicitly
vector<long long> Lagrange(vector<long long> &x, vector<long long> &y) {
    int n = x.size();
    vector<long long> ans(n, 0);
    vector<long long> all(n + 1, 0); //(x - x0) * (x - x1) * ... * (x - x(n-1))
    all[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = n; j >= 0; j--) {
            all[j] = (all[j] * -x[i]) % MOD;
            if (j) all[j] = (all[j] + all[j - 1]) % MOD;
        }
    }
    for (int i = 0; i < n; i++) {
        vector<long long> up(n); //all / (x - xi)
        long long rem = all[n];
        for (int j = n - 1; j >= 0; --j) {
            up[j] = rem;
            rem = (all[j] + rem * x[i]) % MOD;
        }
        long long down = 1;
        for (int j = 0; j < n; j++) if (i != j) down = down * (x[i] - x[j]) % MOD;
        down = modInverse(down) * y[i] % MOD;
        for (int j = 0; j < n; j++) ans[j] = (ans[j] + up[j] * down) % MOD;
    }
    return ans;
}

long long ncr[N][N], d[N][N]; // N > k1 + k2

void prec() {
    for (int i = 0; i < N; i++) ncr[i][0] = 1;
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < N; j++) {
            ncr[i][j] = (ncr[i - 1][j - 1] + ncr[i - 1][j]) % MOD;
        }
    }
    for (int k = 0; k < N; k++) {
        vector<long long> y, x;
        long long sum = 0; y.push_back(0); x.push_back(0);
        for (int i = 1; i <= k + 1; i++) {
            sum = (sum + power(i, k)) % MOD;
            y.push_back(sum); x.push_back(i);
        }
        auto p = Lagrange(x, y);
        for (int i = 0; i <= k + 1; i++) d[k][i] = (i < p.size() ? p[i] : 0);
    }
    d[0][0] = 1;
}

long long power_sum(long long n, int k) {
    assert(k >= 0);
    long long ans = 0;
    for (int i = 0; i <= k + 1; i++) {
        ans = (ans + d[k][i] * power(n, i)) % MOD;
    }
    return (ans + MOD) % MOD;
}

vector<vector<long long>> solve(long long k1, long long k2, long long a, long long b, long long c, long long n) {
    if (n < 0) {
        return vector<vector<long long>>(k1 + k2 + 1, vector<long long>(k1 + k2 + 1, 0));
    }
    if (!a) {
        vector<vector<long long>> ret(k1 + k2 + 1, vector<long long>(k1 + k2 + 1, 0));
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = (ret[_k1][0] * power(b / c, _k2)) % MOD;
            }
        }
        return ret;
    }

    vector<vector<long long>> ret(k1 + k2 + 1, vector<long long>(k1 + k2 + 1, 0));
    assert(c != 0);
    if (a >= c) {
        vector<vector<long long>> nxt = solve(k1, k2, a % c, b, c, n);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int y = 0; y <= _k2; ++y) {
                    long long term = (ncr[_k2][y] * power(a / c, y)) % MOD;
                    ret[_k1][_k2] = (ret[_k1][_k2] + term * nxt[_k1 + y][_k2 - y]) % MOD;
                }
            }
        }
    } else if (b >= c) {
        vector<vector<long long>> nxt = solve(k1, k2, a, b % c, c, n);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int y = 0; y <= _k2; ++y) {
                    long long term = (ncr[_k2][y] * power(b / c, y)) % MOD;
                    ret[_k1][_k2] = (ret[_k1][_k2] + term * nxt[_k1][_k2 - y]) % MOD;
                }
            }
        }
    } else {
        long long m = (a * n + b) / c;
        vector<vector<long long>> nxt = solve(k1, k2, c, c - b - 1, a, m - 1);
        vector<long long> psum(k1 + k2 + 1), pre(k1 + k2 + 1);
        for (int i = 0; i <= k1 + k2; ++i) psum[i] = power_sum(m - 1, i);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            for (int i = 0; i <= k1 + k2; ++i) {
                pre[i] = 0;
                for (int j = 0; j <= _k1 + 1; ++j) {
                    pre[i] = (pre[i] + d[_k1][j] * nxt[i][j]) % MOD;
                }
            }
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int i = 0; i < _k2; ++i) {
                    long long term = (ncr[_k2][i] * psum[i]) % MOD;
                    term = (term * ret[_k1][0]) % MOD;
                    long long sub = (ncr[_k2][i] * pre[i]) % MOD;
                    ret[_k1][_k2] = (ret[_k1][_k2] + term - sub + MOD) % MOD;
                }
            }
        }
    }
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    prec();
    int t; cin >> t;
    while (t--) {
        long long k1, k2, a, b, c, n; cin >> n >> a >> b >> c >> k1 >> k2;
        auto ret = solve(k1, k2, a, b, c, n);
        cout << ret[k1][k2] << '\n';
    }
    return 0;
}

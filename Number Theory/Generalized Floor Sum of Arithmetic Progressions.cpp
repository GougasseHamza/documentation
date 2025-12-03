/*
 * Problem it Solves: Generalized Floor Sum of Arithmetic Progression.
 * Computes sum_{x=0}^{n} x^k1 * floor((ax + b)/c)^k2.
 * Usage: solve(k1, k2, a, b, c, n).
 * Input/Output: k1, k2, a, b, c, n (long long) -> matrix of sums (vector<vector<ll>>).
 * Time Complexity: O((k1+k2)^3 * log n).
 * Tweaks: Depends on global MOD. N defines max k1+k2.
 * Other Use Cases: Advanced counting problems.
 * Warnings: Very high constant factor.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 25, MOD = 1e9 + 7;

using ll = long long;

ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    if (base < 0) base += MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

ll modInverse(ll n) {
    return power(n, MOD - 2);
}

// generates the polynomial explicitly
vector<int> Lagrange(vector<int> &x, vector<int> &y) {
    int n = x.size();
    vector<int> ans(n, 0);
    vector<int> all(n + 1, 0); //(x - x0) * (x - x1) * ... * (x - x(n-1))
    all[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = n; j >= 0; j--) {
            all[j] = (long long)all[j] * -x[i] % MOD;
            if (all[j] < 0) all[j] += MOD;
            if (j) all[j] = (all[j] + all[j - 1]) % MOD;
        }
    }
    for (int i = 0; i < n; i++) {
        vector<int> up(n); //all / (x - xi)
        int rem = all[n];
        for (int j = n - 1; j >= 0; --j) {
            up[j] = rem;
            rem = (all[j] + (long long)rem * x[i]) % MOD;
        }
        int down = 1;
        for (int j = 0; j < n; j++) if (i != j) down = (long long)down * (x[i] - x[j]) % MOD;
        if (down < 0) down += MOD;
        
        up.resize(n);
        int invDown = modInverse(down);
        int factor = (long long)invDown * y[i] % MOD;
        for (int j = 0; j < n; j++) ans[j] = (ans[j] + (long long)up[j] * factor) % MOD;
    }
    return ans;
}

ll ncr[N][N], d[N][N]; // N > k1 + k2

void prec() {
    for (int i = 0; i < N; i++) ncr[i][0] = 1;
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < N; j++) {
            ncr[i][j] = (ncr[i - 1][j - 1] + ncr[i - 1][j]) % MOD;
        }
    }
    for (int k = 0; k < N; k++) {
        vector<int> y, x;
        int sum = 0; y.push_back(0); x.push_back(0);
        for (int i = 1; i <= k + 1; i++) {
            sum = (sum + power(i, k)) % MOD;
            y.push_back(sum); x.push_back(i);
        }
        auto p = Lagrange(x, y);
        for (int i = 0; i <= k + 1; i++) d[k][i] = (i < p.size() ? p[i] : 0);
    }
    d[0][0] = 1; //to satisfy 0^0 = 1
}

ll power_sum(ll n, int k) {
    if (k < 0) return 0; // Safety
    ll ans = 0;
    for (int i = 0; i <= k + 1; i++) {
        ans = (ans + d[k][i] * power(n, i)) % MOD;
    }
    return ans;
}

// \sum{x=0}^{n}{x^k1 * ((ax + b)/c)^k2} //floor division
vector<vector<ll>> solve(ll k1, ll k2, ll a, ll b, ll c, ll n) {
    if (n < 0) {
        return vector<vector<ll>>(k1 + k2 + 1, vector<ll>(k1 + k2 + 1, 0));
    }
    if (!a) {
        vector<vector<ll>> ret(k1 + k2 + 1, vector<ll>(k1 + k2 + 1, 0));
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = (ret[_k1][0] * power(b / c, _k2)) % MOD;
            }
        }
        return ret;
    }
    vector<vector<ll>> ret(k1 + k2 + 1, vector<ll>(k1 + k2 + 1, 0));
    if (a >= c) {
        vector<vector<ll>> nxt = solve(k1, k2, a % c, b, c, n);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int y = 0; y <= _k2; ++y) {
                    ll term = (ncr[_k2][y] * power(a / c, y)) % MOD;
                    ret[_k1][_k2] = (ret[_k1][_k2] + term * nxt[_k1 + y][_k2 - y]) % MOD;
                }
            }
        }
    } else if (b >= c) {
        vector<vector<ll>> nxt = solve(k1, k2, a, b % c, c, n);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int y = 0; y <= _k2; ++y) {
                    ll term = (ncr[_k2][y] * power(b / c, y)) % MOD;
                    ret[_k1][_k2] = (ret[_k1][_k2] + term * nxt[_k1][_k2 - y]) % MOD;
                }
            }
        }
    } else {
        ll m = (a * n + b) / c;
        vector<vector<ll>> nxt = solve(k1, k2, c, c - b - 1, a, m - 1);
        vector<ll> psum(k1 + k2 + 1), pre(k1 + k2 + 1);
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
                    ll term = (ncr[_k2][i] * psum[i]) % MOD * ret[_k1][0] % MOD;
                    term = (term - ncr[_k2][i] * pre[i]) % MOD;
                    ret[_k1][_k2] = (ret[_k1][_k2] + term + MOD) % MOD;
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
        ll k1, k2, a, b, c, n; cin >> n >> a >> b >> c >> k1 >> k2;
        auto ret = solve(k1, k2, a, b, c, n);
        cout << ret[k1][k2] << '\n';
    }
    return 0;
}

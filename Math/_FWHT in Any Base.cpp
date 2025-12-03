/*
 * Problem it Solves: Fast Walsh-Hadamard Transform in base K.
 * Usage: multiply(a, b, k) returns convolution in base K.
 * Input/Output: Vectors a, b, base k.
 * Time Complexity: O(N log_K N * K).
 * Tweaks: MXK defines max base.
 * Other Use Cases: K-ary XOR (addition without carry in base K).
 * Warnings: Modulo arithmetic must support roots of unity.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, MOD = 330301441;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}

namespace FWHT {
    const int MXK = 10; // KEY: (mod - 1) should be divisible by each base used. TWEAK: Increase if needed.
    struct Matrix {
        int a[MXK][MXK], n, m;
        Matrix(int n, int m) : n(n), m(m) { memset(a, 0, sizeof a); }
    };
    
    Matrix operator * (Matrix& p, Matrix& q) {
        Matrix result(p.n, q.m);
        for (int i = 0; i < p.n; i++)
            for (int j = 0; j < q.m; j++) {
                long long val = 0;
                for (int k = 0; k < p.m; k++) val += 1LL * p.a[i][k] * q.a[k][j];
                result.a[i][j] = val % MOD;
            }
        return result;
    }
    
    Matrix constructMatrix(int k, bool invert) {
        vector<int> divs;
        int x = MOD - 1;
        for (int i = 2; i * i <= x; i++) if (x % i == 0) {
            divs.push_back((MOD - 1) / i);
            while (x % i == 0) x /= i;
        }
        if (x > 1) divs.push_back((MOD - 1) / x);
        int g = 2;
        while (true) {
            bool ok = true;
            for (int d : divs) if (power(g, d) == 1) ok = false;
            if (ok) break;
            g++;
        }
        int root = power(g, (MOD - 1) / k);
        Matrix result(k, k);
        for (int i = 0; i < k; i++) {
            int x = power(root, invert ? k - i : i);
            for (int j = 0; j < k; j++) result.a[i][j] = power(x, j);
        }
        return result;
    }
    
    void transform(vector<int> &a, int n, int k, bool invert) {
        Matrix M = constructMatrix(k, invert);
        for (int len = 1; len < n; len *= k)
            for (int i = 0; i < n; i += k * len)
                for (int j = 0; j < len; j++) {
                    Matrix V(1, k);
                    for (int p = 0; p < k; p++) V.a[0][p] = a[i + j + p * len];
                    V = V * M;
                    for (int p = 0; p < k; p++) a[i + j + p * len] = V.a[0][p];
                }
        if (invert) {
            long long d = power(n, MOD - 2);
            for (int i = 0; i < n; i++) a[i] = a[i] * d % MOD;
        }
    }
    
    vector<int> multiply(vector<int> a, vector<int> b, int k) {
        int n = 1;
        int nw = max((int)a.size() - 1, (int)b.size() - 1);
        while (n <= nw) n *= k;
        a.resize(n); b.resize(n);
        transform(a, n, k, false);
        transform(b, n, k, false);
        for (int i = 0; i < n; i++) a[i] = 1LL * a[i] * b[i] % MOD;
        transform(a, n, k, true);
        return a;
    }
    
    vector<int> pow(vector<int> a, long long p, int k) {
        int n = 1;
        int nw = (int)a.size() - 1;
        while (n <= nw) n *= k;
        a.resize(n);
        transform(a, n, k, false);
        for (int i = 0; i < n; i++) a[i] = power(a[i], p);
        transform(a, n, k, true);
        return a;
    }
}

int cnt[N];
int main() {
    int t; cin >> t;
    while (t--) {
        memset(cnt, 0, sizeof cnt);
        int n, k; long long x; cin >> n >> k >> x;
        for (int i = 0; i < n; i++) {
            int z; cin >> z; cnt[z]++;
        }
        int cur = power(2, n), mx = 0;
        vector<int> MEX(N, 0);
        for (int i = 0; cur != 0; i++) {
            cur = 1LL * cur * power(2, 1LL * cnt[i] * (MOD - 2LL) % (MOD - 1)) % MOD;
            MEX[i] = cur;
            mx = i;
            cur = 1LL * cur * (power(2, cnt[i]) - 1) % MOD;
        }
        MEX.resize(mx + 1);
        auto res = FWHT::pow(MEX, x, k);
        int sum = 0, ans = 0;
        for (auto v : res) sum = (sum + v) % MOD;
        for (int y = 0; y < res.size(); y++) {
            long long p = 1LL * res[y] * power(sum, MOD - 2) % MOD;
            p = 1LL * power(p, 3) * power(y, 2) % MOD;
            ans = (ans + power(p, y)) % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}

/*
 * Problem it Solves: Number of solutions to linear algebraic equation with variable upper bounds.
 * Usage: yo(a, s) returns count.
 * Input/Output: a (vector of bounds), s (target sum) -> count (integer mod MOD).
 * Time Complexity: O(2^(n/2) * n^2).
 * Tweaks: Uses Meet-in-the-middle and polynomial operations.
 * Other Use Cases: Counting integer partitions with limits.
 * Warnings: Uses polynomial interpolation/multiplication.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
using ll = long long;

int power(long long n, long long k) {
    int ans = 1 % MOD;
    n %= MOD;
    if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long)ans * n % MOD;
        n = (long long)n * n % MOD;
        k >>= 1;
    }
    return ans;
}

int modInverse(int n) {
    return power(n, MOD - 2);
}

using poly = vector<int>;

int eval(const poly& a, int x) {
    int ans = 0, p = 1;
    for (int b : a) {
        ans = (ans + 1LL * p * b) % MOD;
        p = 1LL * p * x % MOD;
    }
    return ans;
}

poly add(poly a, const poly& b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    for (size_t i = 0; i < b.size(); i++) {
        a[i] = (a[i] + b[i]) % MOD;
    }
    return a;
}

poly sub(poly a, const poly& b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    for (size_t i = 0; i < b.size(); i++) {
        a[i] = (a[i] - b[i] + MOD) % MOD;
    }
    return a;
}

poly mul(const poly& a, const poly& b) {
    vector<int> r(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++) {
        for (size_t j = 0; j < b.size(); j++) {
            r[i + j] = (r[i + j] + 1LL * a[i] * b[j]) % MOD;
        }
    }
    return r;
}

vector<pair<long long, int>> get(vector<long long> a) {
    int n = a.size();
    vector<pair<long long, int>> ans;
    for (int mask = 0; mask < (1 << n); mask++) {
        long long add_val = 0; int x = 1;
        for (int i = 0; i < n; i++) {
            if (mask >> i & 1) {
                add_val += a[i];
                x = -x;
            }
        }
        ans.push_back({add_val, x});
    }
    return ans;
}

// (x + n)(x + n - 1)...(x + n - k + 1);
poly falling(long long n, int k) {
    poly ans({1});
    int p = n % MOD;
    for (int i = 0; i < k; i++) {
        ans = mul(ans, { (p - i + MOD) % MOD, 1 });
    }
    return ans;
}

// returns the number of solutions to the equation x_0 + ... + x_(n-1) = s s.t. 0 <= x_i <= a_i
// O(2^(n/2)*n^2)
int yo(vector<long long> a, long long s) {
    int n = a.size();
    for (auto &x : a) ++x;
    vector<long long> l, r;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) l.push_back(a[i]);
        else r.push_back(a[i]);
    }
    auto L = get(l);
    auto R = get(r);
    sort(L.begin(), L.end());
    sort(R.rbegin(), R.rend());
    int i = 0;
    poly f({0});
    int ans = 0;
    for (auto [x, y] : R) {
        while (i < L.size() and L[i].first + x <= s) {
            if (L[i].second == 1) f = add(f, falling(s + n - 1 - L[i].first, n - 1));
            else f = sub(f, falling(s + n - 1 - L[i].first, n - 1));
            ++i;
        }
        int term = 1LL * eval(f, (-x % MOD + MOD) % MOD) * (y == 1 ? 1 : MOD - 1) % MOD;
        ans = (ans + term) % MOD;
    }
    int fact = 1;
    for (int k = 1; k <= n - 1; k++) fact = 1LL * fact * k % MOD;
    ans = 1LL * ans * modInverse(fact) % MOD;
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<long long> a(n);
        long long s = 0;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            --a[i];
            s += a[i];
        }
        s /= 2;
        cout << yo(a, s) << '\n';
    }
    return 0;
}

/*
 * Problem it Solves: Number of solutions to x_0 + ... + x_{n-1} = s s.t. 0 <= x_i <= a_i.
 * Usage: yo(a, s).
 * Input/Output: Limits a, target s -> count.
 * Time Complexity: O(2^(n/2) * n^2). Uses Meet-in-the-middle.
 * Tweaks: MOD.
 * Other Use Cases: Restricted partitions.
 * Warnings: N should be small (< 40).
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

using poly = vector<int>;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = 1LL * ans * n % MOD;
        n = 1LL * n * n % MOD;
        k >>= 1;
    }
    return ans;
}
int modInverse(int n) { return power(n, MOD - 2); }

int eval(poly a, int x) {
    int ans = 0, p = 1;
    for (int b : a) {
        ans = (ans + 1LL * p * b) % MOD;
        p = 1LL * p * x % MOD;
    }
    return ans;
}

poly add(poly a, poly b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    for (int i = 0; i < b.size(); i++) a[i] = (a[i] + b[i]) % MOD;
    return a;
}

poly sub(poly a, poly b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    for (int i = 0; i < b.size(); i++) a[i] = (a[i] - b[i] + MOD) % MOD;
    return a;
}

poly mul(poly a, poly b) {
    vector<int> r(a.size() + b.size() - 1, 0);
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            r[i + j] = (r[i + j] + 1LL * a[i] * b[j]) % MOD;
        }
    }
    return r;
}

vector<pair<long long, int>> get(vector<long long> a) {
    int n = a.size();
    vector<pair<long long, int>> ans;
    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0; int x = 1;
        for (int i = 0; i < n; i++) {
            if (mask >> i & 1) {
                sum += a[i];
                x = -x;
            }
        }
        ans.push_back({sum, x});
    }
    return ans;
}

// (x + n)(x + n - 1)...(x + n - k + 1)
poly falling(long long n, int k) {
    poly ans({1});
    int p = n % MOD;
    for (int i = 0; i < k; i++) {
        ans = mul(ans, {(p - i + MOD) % MOD, 1});
    }
    return ans;
}

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
        int val = eval(f, (-x % MOD + MOD) % MOD);
        if (y == 1) ans = (ans + val) % MOD;
        else ans = (ans - val + MOD) % MOD;
    }
    
    int fact = 1;
    for (int j = 1; j <= n - 1; j++) fact = 1LL * fact * j % MOD;
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

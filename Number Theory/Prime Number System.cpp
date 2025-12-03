/*
 * Problem it Solves: Computes the value of a number given its Prime Number System representation (exponent of primes in prime factorization).
 * Usage: base(n) creates a representation. Operators *, /, pow, sqrt are supported.
 * Input/Output: n -> base object -> value.
 * Time Complexity: O(sqrt(N)) to factorize initial inputs, O(pi(N)) per operation worst case.
 * Tweaks: N size.
 * Other Use Cases: Working with very large numbers represented as prime factorizations.
 * Warnings: Does not support 0 or negative numbers well.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, MOD = 1e9 + 7;

int spf[N];
vector<int> primes;
void linear_sieve() {
    for (int i = 2; i < N; i++) {
        if (spf[i] == 0) spf[i] = i, primes.push_back(i);
        int sz = primes.size();
        for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

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

struct base { // doesn't support zero
    map<int, int> mp;
    base() {}
    base(int n) {
        while (n > 1) {
            int k = spf[n], cnt = 0;
            while (n % k == 0) {
                n /= k;
                cnt++;
            }
            mp[k] = cnt;
        }
    }
    base(map<int, int> z) {
        mp = z;
    }
    int val() {
        int ans = 1;
        for (auto x : mp) {
            assert(x.second >= 0);
            ans = 1LL * ans * power(x.first, x.second) % MOD;
        }
        return ans;
    }
    base pow(int k) {
        map<int, int> ans = mp;
        for (auto &x : ans) {
            x.second *= k;
        }
        return base(ans);
    }
    base sqrt() {
        map<int, int> ans = mp;
        for (auto &x : ans) {
            assert(x.second % 2 == 0);
            x.second /= 2;
        }
        return base(ans);
    }
    base operator * (const base &p) const {
        map<int, int> ans = mp;
        for (auto x : p.mp) {
            ans[x.first] += x.second;
        }
        return base(ans);
    }
    base operator / (const base &p) const {
        map<int, int> ans = mp;
        for (auto x : p.mp) {
            ans[x.first] -= x.second;
        }
        return base(ans);
    }
    base& operator *= (const base &p) {
        return *this = (*this) * p;
    }
    base& operator /= (const base &p) {
        return *this = (*this) / p;
    }
    void print() {
        bool f = 0;
        for (auto x : mp) {
            if (x.second) {
                f = 1;
                cout << "(" << x.first << ' ' << x.second << ") ";
            }
        }
        if (!f) {
            cout << 1;
        }
        cout << '\n';
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    linear_sieve();
    base a = base(10), b = base(15);
    a *= a;
    a = a.sqrt();
    cout << a.val() << '\n';
    return 0;
}

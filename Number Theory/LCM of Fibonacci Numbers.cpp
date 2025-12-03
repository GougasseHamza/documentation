/*
 * Problem it Solves: Computes LCM of Fibonacci numbers.
 * Usage: yo(vector<int> a) returns lcm(F(a_1), ..., F(a_n)) % MOD.
 * Input/Output: a (vector<int>) -> lcm (int mod MOD).
 * Time Complexity: O(N * log(max_a) * #divisors).
 * Tweaks: N/A.
 * Other Use Cases: Fibonacci properties.
 * Warnings: Recursion depth can be significant.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

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

int fib(long long n) {
    assert(n >= 0);
    if (n <= 1) return n;
    int a = 0;
    int b = 1;
    long long i = 1ll << (63 - __builtin_clzll(n) - 1);
    for (; i; i >>= 1) {
        int na = (1LL * a * a + 1LL * b * b) % MOD;
        int nb = (2LL * a + b) * b % MOD;
        a = na;
        b = nb;
        if (n & i) {
            int c = a + b;
            if (c >= MOD) c -= MOD;
            a = b;
            b = c;
        }
    }
    return b;
}

map<vector<int>, int> dp;

/**
 * lcm(a1, a2, ... an)
 * = lcm(lcm(a1, ..., a[n-1]), an)
 * = lcm(a1, ..., a[n-1]) * an / gcd(lcm(a1, ..., a[n-1]), an)
 * = lcm(a1, ..., a[n-1]) * an / lcm(gcd(a1, an), ... gcd(a[n-1], an))
 */
int yo(vector<int> a) {
    sort(a.rbegin(), a.rend());
    while (!a.empty() && a.back() <= 2) a.pop_back();
    a.resize(unique(a.begin(), a.end()) - a.begin());
    if (a.empty()) return 1;
    if (a.size() == 1) return fib(a[0]);
    if (dp.count(a)) return dp[a];
    
    vector<int> b(a.begin(), a.end() - 1);
    long long res = yo(b);
    
    for (int i = 0; i < b.size(); ++i) b[i] = __gcd(b[i], a.back());
    
    res = res * fib(a.back()) % MOD * power(yo(b), MOD - 2) % MOD;
    dp[a] = res;
    return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    cout << yo(a) << '\n';
    return 0;
}

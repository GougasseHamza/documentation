/*
 * Problem it Solves: Smallest number having exactly K divisors.
 * Usage: yo(0, n).second returns the smallest number.
 * Input/Output: n (long long) -> result (pair).
 * Time Complexity: Heuristic search, usually fast enough.
 * Tweaks: Uses precomputed primes and log values.
 * Other Use Cases: Divisor problems.
 * Warnings: n is incremented at start.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9, MOD = 1e9 + 7;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}

int spf[N];
vector<int> primes;
void sieve() {
    for(int i = 2; i < N; i++) {
        if (spf[i] == 0) spf[i] = i, primes.push_back(i);
        int sz = primes.size();
        for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

double lgp[N];
vector<long long> v;
unordered_map<long long, pair<double, int>> dp[100];

pair<double, int> yo(int i, long long n) { // it solves for odd divisors
    if (n == 1) {
        return {0, 1};
    }
    if (dp[i].find(n) != dp[i].end()) {
        return dp[i][n];
    }
    pair<double, int> ans = {1e50, 0};
    for (auto x: v) {
        if (x > n) break;
        if (n % x != 0) continue;
        auto z = lgp[i + 1] * (x - 1); // i for all divisors
        if (z > ans.first) {
            break;
        }
        auto cur = yo(i + 1, n / x);
        cur.first += z;
        cur.second = 1LL * cur.second * power(primes[i + 1], x - 1) % MOD; // i for all divisors
        ans = min(ans, cur);
    }
    return dp[i][n] = ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    sieve();
    for (int i = 0; i < 100; i++) {
        lgp[i] = log(primes[i]);
    }
    int t, cs = 0; cin >> t;
    while (t--) {
        long long n; cin >> n;
        ++n;
        if (n == 1) {
            cout << "Case " << ++cs << ": " << 1 << '\n';
            continue;
        }
        v.clear();
        for (int i = 1; 1LL * i * i <= n; i++) {
            if (n % i == 0) {
                if (i > 1) v.push_back(i);
                if (i != n / i) {
                    v.push_back(n / i);
                }
            }
        }
        sort(v.begin(), v.end());
        cout << "Case " << ++cs << ": " << yo(0, n).second << '\n';
    }
    return 0;
}

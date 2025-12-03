/*
 * Problem it Solves: Min_25 Sieve for prefix sums of multiplicative functions.
 * Usage: solve(n, poly_coeffs) returns sum_{i=1}^n f(i).
 * Input/Output: n (long long), coeffs -> sum (int mod MOD).
 * Time Complexity: O(N^(3/4) / log N).
 * Tweaks: Change `eval` and `g` calculation logic for different functions.
 * Other Use Cases: Sum of primes, sum of phi, etc.
 * Warnings: Highly technical, requires understanding of Min_25 structure.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, D = 3; // N >= 2 * sqrt(n)
const int MOD = 1e9 + 7;
using ll = long long;

/**
 * Let f(x) be a multiplicative function and
 * when p is a prime, f(p) has a polynomial representation 
 * and f(p^k) can be calculated quickly
 * We can compute the prefix sum of f(n) in O((n^(3/4)) / logn) using Min_25 sieve
 */

int primes[N], p, deg;
int pref[D][N];
int poly[D]; // polynomial representation of f(p)

void sieve(int n) {
    vector<bool> f(n + 1, false);
    p = 0;
    for (int i = 2; i <= n; i++) {
        if (!f[i]) {
            primes[++p] = i;
            int cur = 1;
            // pref[k][p] = sum of primes[i]^k s.t. i <= p
            for (int k = 0; k < deg; k++) {
                pref[k][p] = (pref[k][p - 1] + cur) % MOD;
                cur = 1LL * cur * i % MOD;
            }
        }
        for (int j = 1; j <= p and primes[j] * i <= n; j++) {
            f[i * primes[j]] = 1;
            if (i % primes[j] == 0) break;
        }
    }
}

int r, id1[N], id2[N];
ll n;
int get_id(ll x) {
    return x <= r ? id1[x] : id2[n / x];
}
int tot;
int g[D][N];
ll a[N];

// f(p^k) in O(1)
inline int eval(int p, int k, ll pw) { // pw = p^k, pw <= n
    int ans = pw % MOD;
    return 1LL * ans * (ans - 1) % MOD; // Example function: f(p^k) = p^k(p^k - 1)
}

// no memorization needed!
// sum of f(i) s.t. spf[i] > primes[j]
int yo(ll x, int j) {
    if (primes[j] >= x) return 0;
    int id = get_id(x);
    int ans = 0;
    // initialization: ans = g(x) - g(primes[j])
    for (int k = 0; k < deg; k++) {
        ans = (ans + 1LL * poly[k] * g[k][id]) % MOD;
    }
    for (int k = 0; k < deg; k++) {
        ans = (ans - 1LL * poly[k] * pref[k][j] % MOD + MOD) % MOD;
    }
    for (int i = j + 1; i <= p and primes[i] <= x / primes[i]; i++) {
        ll pw = primes[i];
        for (int e = 1; pw <= x; e++) {
            int val = eval(primes[i], e, pw);
            int rec = yo(x / pw, i);
            if (e != 1) rec = (rec + 1) % MOD;
            
            int term = 1LL * val * (rec + (e != 1)) % MOD;
            ans = (ans + term) % MOD;
            
            if (!(pw <= x / primes[i])) break;
            pw *= primes[i];
        }
    }
    return ans;
}

int power(int base, int exp) {
    int res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = 1LL * res * base % MOD;
        base = 1LL * base * base % MOD;
        exp /= 2;
    }
    return res;
}

int inverse(int n) {
    return power(n, MOD - 2);
}

// sum of f(i) for 1 <= i <= n
// pol is the polynomial representation of f(p)
int solve(ll _n, vector<int> pol) {
    n = _n;
    deg = pol.size();
    for (int i = 0; i < deg; i++) {
        poly[i] = pol[i];
    }
    r = sqrt(n);
    while (1LL * r * r < n) ++r;
    while (1LL * r * r > n) --r;
    sieve(r);
    tot = 0;
    ll i = 1;
    int inv2 = inverse(2);
    int inv6 = inverse(6);
    
    while (i <= n) {
        ll x = n / i;
        ll j = n / x;
        a[++tot] = x;

        // initialization g[k][tot] = sum of i^k for 2 <= i <= x
        int z = x % MOD;
        for (int k = 0; k < deg; k++) {
            if (k == 0) {
                g[k][tot] = (z - 1 + MOD) % MOD;
            }
            if (k == 1) {
                g[k][tot] = (1LL * z * (z + 1) % MOD * inv2 % MOD - 1 + MOD) % MOD;
            }
            if (k == 2) {
                g[k][tot] = (1LL * z * (z + 1) % MOD * (2LL * z + 1) % MOD * inv6 % MOD - 1 + MOD) % MOD;
            }
        }

        if (x <= r) id1[x] = tot;
        else id2[n / x] = tot;
        i = j + 1;
    }

    for (int i = 1; i <= p; i++) {
        for (int j = 1; j <= tot && primes[i] <= a[j] / primes[i]; j++) {
            int id = get_id(a[j] / primes[i]);
            int cur = 1;
            for (int k = 0; k < deg; k++) {
                int sub = (g[k][id] - pref[k][i - 1] + MOD) % MOD;
                g[k][j] = (g[k][j] - 1LL * cur * sub % MOD + MOD) % MOD;
                cur = 1LL * cur * primes[i] % MOD;
            }
        }
    }
    return (yo(n, 0) + 1) % MOD; // add f(1)
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    vector<int> p({0, MOD - 1, 1}); // Example polynomial
    ll n; cin >> n;
    cout << solve(n, p) << '\n';
    return 0;
}

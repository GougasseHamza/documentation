/*
 * Problem it Solves: Powerful Number Sieve.
 * Usage: solve() reads n and outputs answer.
 * Input/Output: n -> prefix sum of multiplicative function f (mod 2^64 approx).
 * Time Complexity: O(sqrt(n)).
 * Tweaks: h(p^k) definition, g_prefix_sum definition.
 * Other Use Cases: Summing multiplicative functions.
 * Warnings: Uses __int128 for modulo arithmetic to avoid overflow.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;
using ll = long long;

const ll mod = 4179340454199820289;

inline ll mul(ll x, ll y) {
    return __int128(x) * y % mod;
}

inline ll add(ll x, ll y) {
    return (x += y) >= mod ? x - mod : x;
}

ll power(ll x, ll n) {
    ll ans = 1;
    for (; n; n >>= 1, x = mul(x, x)) {
        if (n & 1) ans = mul(ans, x);
    }
    return ans;
}

// sieve upto sqrt(n)
int spf[N];
vector<int> primes;
void sieve() {
    for (int i = 2; i < N; i++) {
        if (spf[i] == 0) spf[i] = i, primes.push_back(i);
        int sz = primes.size();
        for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

ll inv[60];

// g(x) = x
ll g_prefix_sum(ll n) { // return prefix sum of g()
    return mul(mul(n, n + 1), inv[2]);
}

// h(p^k) = -p^k / (k * (k - 1))
ll get_h(int p, int k, ll pk) { // return h(p^k), k >= 2
    return mod - mul(mul(pk, inv[k]), inv[k - 1]);
}

ll n;
// generate all powerful numbers and add up the results
// params: current prime index, current number generated, and h(m)
ll brute(int i, ll m, ll h) {
    ll ans = mul(h, g_prefix_sum(n / m));
    ll lim = n / m;
    while (i < primes.size() and 1LL * primes[i] * primes[i] <= lim) {
        int k = 2; ll pk = 1LL * primes[i] * primes[i];
        while (pk <= lim) {
            ans = add(ans, brute(i + 1, m * pk, mul(h, get_h(primes[i], k, pk))));
            ++k;
            pk *= primes[i];
        }
        ++i;
    }
    return ans;
}

void solve() {
    cin >> n;
    ll prefix_sum = brute(0, 1, 1);
    ll ans = mul(power(n, mod - 2), prefix_sum);
    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    inv[1] = 1;
    for (int i = 2; i < 60; i++) {
        inv[i] = power(i, mod - 2);
    }
    sieve();

    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}

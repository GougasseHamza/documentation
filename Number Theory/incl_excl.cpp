/*
 * Count numbers <= n coprime with r (many queries version)
 * --------------------------------------------------------
 * Formula:
 *   C(n, r) = sum_{d | r} μ(d) * floor(n / d)
 *
 * Precompute μ(d) with a sieve up to max(r).
 */

 #include <bits/stdc++.h>
 using namespace std;
 using ll = long long;
 
 const int MAXN = 1000000; // adjust as needed
 int mu[MAXN+1];
 vector<int> primes;
 
 void mobius_sieve(int n = MAXN) {
     vector<int> spf(n+1);
     mu[1] = 1;
     for (int i=2;i<=n;i++) {
         if (!spf[i]) {
             spf[i]=i; primes.push_back(i);
             mu[i] = -1; // prime
         }
         for (int p:primes) {
             if (p>spf[i] || 1LL*i*p>n) break;
             spf[i*p]=p;
             if (p==spf[i]) { mu[i*p]=0; break; }
             else mu[i*p]=-mu[i];
         }
     }
 }
 
 // count numbers <= n coprime to r
 ll coprime_count(ll n, int r) {
     ll ans = 0;
     // iterate over divisors of r
     for (int d=1; 1LL*d*d<=r; d++) {
         if (r%d==0) {
             ans += 1LL*mu[d] * (n/d);
             if (d != r/d) ans += 1LL*mu[r/d] * (n/(r/d));
         }
     }
     return ans;
 }
 ll coprime_count_2(ll n, ll r) {
    auto p = prime_factors(r);
    int k = p.size();
    ll ans = n;
    for (int mask=1; mask < (1<<k); mask++) {
        ll mult = 1;
        for (int i=0; i<k; i++) if (mask>>i & 1) mult *= p[i];
        ll cnt = n / mult;
        if (__builtin_popcount(mask) & 1) ans -= cnt;
        else ans += cnt;
    }
    return ans;
} 
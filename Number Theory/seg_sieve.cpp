/*
 * Segmented Sieve of Eratosthenes
 * -------------------------------
 * Finds all primes in range [L, R] with R up to ~1e12
 * using O(√R + (R-L+1)) memory and time ~ O((R-L+1) log log R).
 *
 * Steps:
 *   1. Precompute primes up to √R using normal sieve
 *   2. Mark their multiples in [L, R]
 *
 * Usage:
 *   auto primes = segmented_sieve(L, R);
 *
 * Complexity:
 *   - Precomputation: O(√R log log R)
 *   - Segmentation: O(R-L+1)
 */

 #include <bits/stdc++.h>
 using namespace std;
 using ll = long long;
 
 // Simple sieve up to n
 vector<int> simple_sieve(int n) {
     vector<bool> mark(n+1,true);
     vector<int> primes;
     mark[0]=mark[1]=false;
     for (int i=2;i<=n;i++) {
         if (mark[i]) {
             primes.push_back(i);
             if ((ll)i*i <= n)
                 for (ll j=1LL*i*i;j<=n;j+=i) mark[j]=false;
         }
     }
     return primes;
 }
 
 // Segmented sieve for [L,R]
 vector<ll> segmented_sieve(ll L, ll R) {
     ll lim = sqrt(R)+1;
     vector<int> primes = simple_sieve(lim);
 
     vector<bool> is_prime(R-L+1,true);
     if (L==1) is_prime[0]=false;
 
     for (int p: primes) {
         for (ll j=max(1LL*p*p, ((L+p-1)/p)*1LL*p); j<=R; j+=p) {
             is_prime[j-L]=false;
         }
     }
     vector<ll> res;
     for (ll i=L;i<=R;i++) if (is_prime[i-L]) res.push_back(i);
     return res;
 }
 
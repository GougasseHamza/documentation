/*
 * Legendre's Formula
 * ------------------
 * Computes the exponent of a prime p in n!
 * That is, how many times p divides n!
 *
 * Formula:
 *   e_p(n!) = floor(n/p) + floor(n/p^2) + floor(n/p^3) + ...
 *
 * Usage:
 *   cout << legendre(n, p);
 *
 * Complexity:
 *   O(log_p n)
 */

 #include <bits/stdc++.h>
 using namespace std;
 using ll = long long;
 
 // exponent of prime p in n!
 ll legendre(ll n, ll p) {
     ll ans = 0;
     while (n) {
         n /= p;
         ans += n;
     }
     return ans;
 }
 
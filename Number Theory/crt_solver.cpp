/*
 * General Chinese Remainder Theorem (CRT)
 * ---------------------------------------
 * Solves system of congruences:
 *     x ≡ b[i] (mod m[i])   for i = 0..n-1
 *
 * Works for non-coprime moduli as well.
 *
 * Returns:
 *   (x, lcm)  where
 *      • x is the smallest non-negative solution
 *      • solution is unique modulo lcm = lcm(m[0..n-1])
 *   If no solution exists → returns (0, -1).
 *
 * Complexity: O(n log M), M = max modulus
 */

 #include <bits/stdc++.h>
 using namespace std;
 using ll = long long;
 
 // extended gcd: ax+by=g
 ll exgcd(ll a, ll b, ll &x, ll &y) {
     if (!b) { x=1; y=0; return a; }
     ll x1,y1;
     ll g = exgcd(b, a%b, x1,y1);
     x = y1;
     y = x1 - y1*(a/b);
     return g;
 }
 
 // merge two congruences: x ≡ a1 (mod m1), x ≡ a2 (mod m2)
 pair<ll,ll> mergeCRT(ll a1,ll m1,ll a2,ll m2) {
     ll p,q;
     ll g = exgcd(m1,m2,p,q);
     if ((a2 - a1) % g != 0) return {0,-1}; // no solution
     ll lcm = m1 / g * m2;
     ll x = ( (__int128)a1*(m2/g)%lcm*q%lcm + (__int128)a2*(m1/g)%lcm*p%lcm ) % lcm;
     x = (x % lcm + lcm) % lcm;
     return {x,lcm};
 }
 
 // solve n congruences
 pair<ll,ll> crt(const vector<ll>& a,const vector<ll>& m) {
     assert(a.size() == m.size());
     ll x=a[0], mod=m[0];
     for (int i=1;i<(int)a.size();i++) {
         auto res = mergeCRT(x,mod,a[i],m[i]);
         if (res.second==-1) return {0,-1};
         x=res.first; mod=res.second;
     }
     return {x,mod};
 }
 
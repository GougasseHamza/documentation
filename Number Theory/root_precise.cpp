/*
 * Precise Integer k-th Root
 * -------------------------
 * Computes floor(n^(1/k)) exactly for large n.
 *
 * Usage:
 *   auto r = kth_root(n, k);
 *   // r is the largest integer x such that x^k <= n
 *
 * Complexity:
 *   O(k log n)   (binary search + safe multiplication)
 */

 #include <bits/stdc++.h>
 using namespace std;
 using u64 = unsigned long long;
 using u128 = __uint128_t;
 
 // checks if mid^k <= n without overflow
 bool leq(u128 n, u64 mid, int k) {
     u128 x = 1;
     for (int i = 0; i < k; i++) {
         x *= mid;
         if (x > n) return false;
     }
     return true;
 }
 
 u64 kth_root(u128 n, int k) {
     u64 lo = 1, hi = (u64) pow((long double) n, 1.0L / k) + 2;
     while (lo < hi) {
         u64 mid = (lo + hi + 1) >> 1;
         if (leq(n, mid, k)) lo = mid;
         else hi = mid - 1;
     }
     // x = 1;
     //for (int i = 0; i < k; i++) x *= lo;
     //if (x == n) return lo;
     return lo;
 }

/*
 * Mo's Algorithm (Standard)
 *
 * Complexity: O((N + Q) * sqrt(N))
 *
 * Tricks & Tweaks:
 * 1. Block Size: Use N / sqrt(Q) for optimal performance if N != Q.
 * 2. Zig-Zag Optimization: The comparator `r < x.r` vs `r > x.r` based on 
 * odd/even block reduces pointer movement by ~50%.
 * 3. Hilbert Curve: For very tight time limits, replace the sort comparator 
 * with Hilbert Curve order (better locality).
 *
 * What Not To Forget:
 * - The add/remove functions MUST be inverses of each other.
 * - `curr_ans` logic must be careful with 0 or negative values if applicable.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e6 + 9;
 // Block size: usually N / sqrt(Q) or fixed ~450 for N=2e5
 const int B = 440; 
 
 struct query {
   int l, r, id;
   bool operator < (const query &x) const {
     // Zig-Zag optimization
     if(l / B == x.l / B) return ((l / B) & 1) ? r > x.r : r < x.r;
     return l / B < x.l / B;
   }
 } Q[N];
 
 int cnt[N], a[N];
 long long sum; // Current answer
 long long ans[N]; // Store answers
 
 // --- CUSTOMIZE THESE FUNCTIONS ---
 inline void add_left(int i) {
   int x = a[i];
   // Example: sum of (val * freq^2)
   // Remove old contribution: cnt[x]^2 * x
   sum -= 1LL * cnt[x] * cnt[x] * x;
   ++cnt[x];
   // Add new contribution: (cnt[x])^2 * x
   sum += 1LL * cnt[x] * cnt[x] * x;
 }
 inline void add_right(int i) { add_left(i); } // Usually symmetric
 
 inline void rem_left(int i) {
   int x = a[i];
   sum -= 1LL * cnt[x] * cnt[x] * x;
   --cnt[x];
   sum += 1LL * cnt[x] * cnt[x] * x;
 }
 inline void rem_right(int i) { rem_left(i); }
 // --------------------------------
 
 void solve_mos(int n, int q) {
   sort(Q + 1, Q + q + 1);
   int l = 1, r = 0;
   for(int i = 1; i <= q; i++) {
     int L = Q[i].l, R = Q[i].r;
     if(R < l) { 
       // Reset if range is invalid/disjoint (rarely needed if inputs valid)
       while (l > L) add_left(--l);
       while (l < L) rem_left(l++);
       while (r < R) add_right(++r);
       while (r > R) rem_right(r--);
     } else {
       while (r < R) add_right(++r);
       while (r > R) rem_right(r--);
       while (l > L) add_left(--l);
       while (l < L) rem_left(l++);
     }
     ans[Q[i].id] = sum;
   }
 }
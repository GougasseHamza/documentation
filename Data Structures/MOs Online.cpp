/*
 * Mo's Algorithm with Updates (3D Mo's)
 *
 * Complexity: O(N^(5/3))
 * Block Size: N^(2/3) (~2200 for 1e5)
 *
 * Tricks & Tweaks:
 * 1. Updates: Store updates as {pos, old_val, new_val}.
 * 2. Time Travel: Move global 't' pointer to match query 't'.
 * If moving forward, apply update. If moving backward, undo update (apply old_val).
 *
 * What Not To Forget:
 * - The 't' in Query is the index of the last update applied before this query.
 * - Sort logic: Block(L) -> Block(R) -> Time.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 5;
 const int B = 2200; // N^(2/3)
 
 // Example DS: MEX or Mode
 struct DataStructure {
   int cnt[N * 2], f[N * 2]; 
   void add(int x) {
     f[cnt[x]]--; ++cnt[x]; f[cnt[x]]++;
   }
   void del(int x) {
     f[cnt[x]]--; --cnt[x]; f[cnt[x]]++;
   }
   int get_mex() {
     int ans = 1;
     while(f[ans] > 0) ans++;
     return ans;
   }
 } ds;
 
 struct update { int pos, old_val, new_val; } U[N];
 struct query { int l, r, t, id; } Q[N];
 
 bool cmp(const query &a, const query &b) {
   int l1 = a.l / B, l2 = b.l / B;
   int r1 = a.r / B, r2 = b.r / B;
   if(l1 != l2) return l1 < l2;
   if(r1 != r2) return r1 < r2;
   return a.t < b.t;
 }
 
 int a[N], ans[N];
 int l = 1, r = 0, t = 0; // Current range [l, r] at time t
 
 void apply_update(int idx) {
     if(U[idx].pos >= l && U[idx].pos <= r) {
         ds.del(a[U[idx].pos]);
         ds.add(U[idx].new_val);
     }
     a[U[idx].pos] = U[idx].new_val;
 }
 
 void undo_update(int idx) {
     if(U[idx].pos >= l && U[idx].pos <= r) {
         ds.del(a[U[idx].pos]);
         ds.add(U[idx].old_val);
     }
     a[U[idx].pos] = U[idx].old_val;
 }
 
 void solve_3d_mos(int n, int q) {
   sort(Q + 1, Q + q + 1, cmp);
   for(int i = 1; i <= q; i++) {
     while(t < Q[i].t) apply_update(++t);
     while(t > Q[i].t) undo_update(t--);
     while(l > Q[i].l) ds.add(a[--l]);
     while(r < Q[i].r) ds.add(a[++r]);
     while(l < Q[i].l) ds.del(a[l++]);
     while(r > Q[i].r) ds.del(a[r--]);
     ans[Q[i].id] = ds.get_mex();
   }
 }
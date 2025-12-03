/**
 * Segment Tree Merging
 *
 * * PROBLEMS SOLVED:
 * 1. Subtree queries involving value constraints (e.g. Count elements > K in subtree).
 * 2. Maintaining Sets of values that merge (Disjoint Set on SegTrees).
 * 3. Range Sort (The provided code implements Range Sort using Split/Merge).
 *
 * * COMPLEXITY:
 * Merge: Amortized O(log N) per element inserted. Total O(N log N).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 
 struct STM {
 #define lc t[cur].l
 #define rc t[cur].r
   struct node {
     int l, r, sz; // l, r are indices of children
   } t[N * 30];
   int T, id[N * 30]; // Memory management (recycling nodes)
 
   STM() {
     T = 0; for (int i = 1; i < N * 30; i++) id[i] = i;
   }
   
   inline int new_node() {
     int cur = id[++T];
     t[cur].l = t[cur].r = t[cur].sz = 0;
     return cur;
   }
   inline void save_memory(int x) { id[T--] = x; }
 
   int build(int b, int e, int i) {
     int cur = new_node();
     if (b == e) { t[cur].sz = 1; return cur; }
     int mid = b + e >> 1;
     if(i <= mid) lc = build(b, mid, i);
     else rc = build(mid + 1, e, i);
     t[cur].sz = t[lc].sz + t[rc].sz;
     return cur;
   }
 
   // Merge tree b into a. Returns new root.
   int merge(int a, int b) {
     if(!a || !b) return a ^ b; // If one is 0, return the other
     t[a].l = merge(t[a].l, t[b].l);
     t[a].r = merge(t[a].r, t[b].r);
     t[a].sz += t[b].sz;
     save_memory(b); // b is merged into a, so b node is free
     return a;
   }
 
   // Split cur into (cur, b) such that cur keeps 'k' smallest elements
   void split(int cur, int &b, int k) {
     if (t[cur].sz < k) return; // cur already has <= k elements
     b = new_node();
     int lsz = t[lc].sz;
     if(k > lsz) split(rc, t[b].r, k - lsz); // Need more from right
     else swap(rc, t[b].r); // Give entire right child to b
     if(k < lsz) split(lc, t[b].l, k); // Split left
     
     t[b].sz = t[cur].sz - k;
     t[cur].sz = k;
   }
 } t;
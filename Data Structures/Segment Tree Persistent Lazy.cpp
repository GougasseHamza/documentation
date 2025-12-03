/**
 * Persistent Segment Tree with Lazy Propagation
 *
 * * LOGIC:
 * - Standard Persistence: Copy node before modifying.
 * - Lazy Persistence: When pushing lazy from node U, we MUST create copies 
 * of U's children, because pushing modifies them.
 *
 * * USAGE:
 * int root_t = upd(root_prev, 1, n, l, r);
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 using mint = long long; // Placeholder
 
 struct PST {
 #define lc t[cur].l
 #define rc t[cur].r
   struct node {
     int l = 0, r = 0, lazy = 0;
     mint val = 0;
   } t[300 * N];
   int T = 0;
 
   int build(int b, int e) {
     int cur = ++T;
     if(b == e) return cur;
     int mid = b + e >> 1;
     lc = build(b, mid);
     rc = build(mid + 1, e);
     return cur;
   }
 
   // Create new node as copy of 'pre' and apply lazy 'x'
   int push(int pre, int b, int e, int x = 0) {
     int cur = ++T;
     t[cur] = t[pre];
     t[cur].lazy ^= x; // Apply lazy logic (here XOR)
     
     if(t[cur].lazy) {
       // Update value based on lazy (Problem specific)
       // t[cur].val = ...
       
       if(b != e) {
         // Create copies of children to push down into
         lc = ++T; rc = ++T;
         t[lc] = t[t[pre].l]; t[rc] = t[t[pre].r];
         // Propagate lazy
         t[lc].lazy ^= 1; t[rc].lazy ^= 1;
       }
     }
     t[cur].lazy = 0;
     return cur;
   }
 
   int upd(int pre, int b, int e, int i, int j) {
     int cur = push(pre, b, e); // Ensure current node is clean/copied
     if(b > j || e < i) return cur;
     if(i <= b && e <= j) {
       return push(cur, b, e, 1); // Apply update lazy
     }
     int mid = b + e >> 1;
     lc = upd(t[cur].l, b, mid, i, j); // Note: pass t[cur].l which is the clean child
     rc = upd(t[cur].r, mid + 1, e, i, j);
     t[cur].val = t[lc].val + t[rc].val;
     return cur;
   }
 } t;
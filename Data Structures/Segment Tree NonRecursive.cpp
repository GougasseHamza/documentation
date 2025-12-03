/**
 * Iterative Segment Tree
 * * COMPLEXITY: O(log N). Faster constant factor.
 * * LOGIC: 
 * - Leaves are at indices [N, 2N-1].
 * - Parent of p is p>>1.
 * - Loop p>>=1 moves up the tree.
 */
 #include<bits/stdc++.h>
 using namespace std;
 
 struct Node { int sum = 0; };
 
 struct ST {
   int n;
   Node *t;
   ST(int _n) { n = _n; t = new Node[2 * n]; }
 
   inline Node combine(Node l, Node r) {
     return {l.sum + r.sum};
   }
 
   void build() {
     // Leaves [n, 2n-1] assumed filled
     for(int i = n - 1; i > 0; --i) t[i] = combine(t[i << 1], t[i << 1 | 1]);
   }
 
   void upd(int p, int v) {
     for (t[p += n] = {v}; p >>= 1; ) 
       t[p] = combine(t[p << 1], t[p << 1 | 1]);
   }
 
   Node query(int l, int r) { // [l, r)
     Node resl, resr;
     // If l is odd (right child), include it and move right.
     // If r is odd (right child), move left and include it.
     for(l += n, r += n; l < r; l >>= 1, r >>= 1) {
       if(l & 1) resl = combine(resl, t[l++]);
       if(r & 1) resr = combine(t[--r], resr);
     }
     return combine(resl, resr);
   }
 };
/*
 * Implicit Treap
 *
 * Description:
 * Dynamic Array supporting Split/Merge/Reverse/Lazy Ops.
 *
 * Tricks & Tweaks:
 * 1. Priority: Assigned randomly to keep tree balanced (O(log N) height).
 * 2. Implicit Key: The index is determined by the size of the left subtree + 1.
 * 3. Reverse: Swap children and toggle a 'rev' bit. Push down lazily.
 *
 * What Not To Forget:
 * - call push() before touching children.
 * - call update_cnt() after modifying children.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
 
 struct treap {
   struct node {
     int val, sz, prior, lazy, sum;
     bool rev;
     node *l, *r;
     node(int _val) {
       val = _val; sum = _val;
       rev = 0; lazy = 0; sz = 1; prior = rnd();
       l = NULL; r = NULL;
     }
   };
   typedef node* pnode;
   pnode root = NULL;
 
   int size(pnode t) { return t ? t->sz : 0; }
   
   void update_cnt(pnode t) {
     if(t) {
         t->sz = size(t->l) + size(t->r) + 1;
         t->sum = t->val + (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0);
     }
   }
 
   void push(pnode t) {
     if(!t) return;
     if(t->rev) {
         swap(t->l, t->r);
         if(t->l) t->l->rev ^= 1;
         if(t->r) t->r->rev ^= 1;
         t->rev = 0;
     }
     if(t->lazy) {
         t->val += t->lazy; 
         t->sum += t->lazy * size(t);
         if(t->l) t->l->lazy += t->lazy;
         if(t->r) t->r->lazy += t->lazy;
         t->lazy = 0;
     }
   }
 
   // Split: First k elements go to l, rest to r
   void split(pnode t, pnode &l, pnode &r, int k, int add = 0) {
     if(!t) { l = r = NULL; return; }
     push(t);
     int idx = add + size(t->l);
     if(idx <= k) split(t->r, t->r, r, k, idx + 1), l = t;
     else split(t->l, l, t->l, k, add), r = t;
     update_cnt(t);
   }
 
   void merge(pnode &t, pnode l, pnode r) {
     push(l); push(r);
     if(!l || !r) t = l ? l : r;
     else if(l->prior > r->prior) merge(l->r, l->r, r), t = l;
     else merge(r->l, l, r->l), t = r;
     update_cnt(t);
   }
 
   // Insert val at index pos (0-based)
   void insert(int pos, int val) {
     pnode l, r, mid = new node(val);
     split(root, l, r, pos - 1);
     merge(l, l, mid);
     merge(root, l, r);
   }
 
   // Reverse range [l, r]
   void reverse(int l, int r) {
     pnode t1, t2, t3;
     split(root, t1, t2, l - 1);
     split(t2, t2, t3, r - l);
     if(t2) t2->rev ^= 1;
     merge(root, t1, t2);
     merge(root, root, t3);
   }
   
   // Range Sum [l, r]
   int query(int l, int r) {
     pnode t1, t2, t3;
     split(root, t1, t2, l - 1);
     split(t2, t2, t3, r - l);
     int ans = t2 ? t2->sum : 0;
     merge(root, t1, t2);
     merge(root, root, t3);
     return ans;
   }
 };
/**
 * Treap (Standard)
 * * * PROBLEMS SOLVED:
 * 1. Range Move/Cut-Paste operations.
 * 2. Maintaining sorted structure with splitting/merging.
 * 3. Specific: CF 911G (Color updates).
 *
 * * * LOGIC:
 * - 'merge_treap(l, r)': Specialized merge that assumes 'l' and 'r' are NOT
 * disjoint in keys (indices) and merges them into one sorted treap.
 * This is essentially "Set Union" on Treaps.
 * - Logic for CF 911G: 
 * To change color X to Y in range [L, R]:
 * 1. Split treap for color X to extract part in [L, R].
 * 2. Merge (Union) that extracted part into treap for color Y.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 2e5 + 9;
 mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
 
 struct node {
   node *l, *r;
   int key, prior;
   node(int id) { l = r = nullptr; key = id; prior = rnd(); }
 };
 
 struct treap {
   node *root;
   treap() { root = nullptr; }
 
   // Standard Split by Key
   void split(node *t, int pos, node *&l, node *&r) {
     if (t == nullptr) { l = r = nullptr; return; }
     if (t->key <= pos) {
       split(t->r, pos, l, r);
       t->r = l; l = t;
     } else {
       split(t->l, pos, l, r);
       t->l = r; r = t;
     }
   }
 
   // Standard Merge (Assumption: MaxKey(l) < MinKey(r))
   node* merge(node *l, node *r) {
     if (!l || !r) return l ? l : r;
     if (l->prior < r->prior) {
       l->r = merge(l->r, r);
       return l;
     }
     r->l = merge(l, r->l);
     return r;
   }
 
   // Union Merge: Merges two treaps that might overlap in key ranges.
   // Complexity: O(M log(N/M)) where M is size of smaller treap.
   node* merge_treap(node *l, node *r) {
     if(!l) return r;
     if(!r) return l;
     if(l->prior < r->prior) swap(l, r); // Heuristic: Merge smaller into larger
     
     node *L, *R;
     split(r, l->key, L, R); // Split 'r' based on 'l->key'
     l->r = merge_treap(l->r, R);
     l->l = merge_treap(L, l->l);
     return l;
   }
 };
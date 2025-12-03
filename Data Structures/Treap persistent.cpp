/**
 * Persistent Implicit Treap
 * * * PROBLEMS SOLVED:
 * 1. Versioned Strings/Arrays: "What was the string at version T?"
 * 2. Range Copy/Paste between versions.
 * 3. Range Erase/Insert with history.
 *
 * * * COMPLEXITY:
 * Time: O(log N) per operation.
 * Space: O(log N) new nodes per operation.
 *
 * * * LOGIC:
 * - Implicit Key: The index is determined by subtree sizes.
 * - Persistence: In 'split' and 'merge', instead of modifying nodes, we
 * create copies ('copy' function) of the nodes we visit. 
 * This preserves the old tree structure (old versions).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 5e4 + 9;
 
 mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
 
 struct node {
   node *l, *r;
   int val, prior, sz;
   node() {}
   node(int id) {
     l = r = nullptr; val = id; prior = rnd(); sz = 1;
   }
 };
 
 namespace PersistentTreap {
     // Creates a copy of node 'b' into 'a'
     inline void copy(node* &a, node* b) {
       if (!b) a = nullptr;
       else {
         a = new node();
         *a = *b; // Shallow copy of values and children pointers
       }
     }
 
     int size(node* t) { return t ? t->sz : 0; }
     
     void update_size(node* &t) {
       if(t) t->sz = size(t->l) + size(t->r) + 1;
     }
 
     // Persistent Split: Copies path nodes
     void split(node *cur, int pos, node *&l, node *&r, int add = 0) {
       node* t;
       copy(t, cur); // Copy current node
       
       if (t == nullptr) {
         l = r = nullptr;
         return;
       }
       
       int id = add + size(t -> l);
       if (id <= pos) {
         split(t->r, pos, l, r, id + 1);
         t->r = l; // Link copied node to new split result
         l = t;
       } else {
         split(t->l, pos, l, r, add);
         t->l = r;
         r = t;
       }
       update_size(t);
     }
 
     // Persistent Merge
     node* merge(node *l, node *r) {
       node* t;
       // If one is null, copy the other (to ensure we return a new pointer)
       if (!l || !r) copy(t, l ? l : r);
       else if (l->prior < r->prior) {
         copy(t, l);
         t->r = merge(t->r, r); // Recurse
       } else {
         copy(t, r);
         t->l = merge(l, t->l);
       }
       update_size(t);
       return t;
     }
     
     // Insert 'val' at 'pos' in version 't'
     node* insert(node* t, int pos, int val) {
       node *l, *r;
       split(t, pos - 1, l, r);
       return merge(merge(l, new node(val)), r);
     }
 
     // Erase range [L, R] from version 't'
     node* erase(node* t, int L, int R) {
       node *l, *r, *mid, *mr;
       split(t, L - 1, l, r);
       split(r, R - L, mid, mr);
       // 'mid' is the range to delete. We just merge l and mr.
       return merge(l, mr);
     }
 }
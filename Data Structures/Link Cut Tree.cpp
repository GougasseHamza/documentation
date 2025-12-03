/*
 * Link Cut Tree (LCT)
 *
 * Description:
 * Handles dynamic tree operations. Each path in the tree is decomposed into 
 * Splay trees (Auxiliary Trees).
 *
 * Tricks & Tweaks:
 * 1. Lazy Propagation: Update `upd` and `push` to handle range adds/sets on paths.
 * 2. Subtree Queries: This implementation supports subtree queries by maintaining
 * `vsz` (virtual size) and `vsum` (virtual sum). These represent contributions
 * from children attached via "dashed" edges (non-preferred edges).
 * 3. Rooting: `make_root(u)` makes u the root of the represented tree (reverses path to old root).
 *
 * What Not To Forget:
 * - Use `access(u)` to bring u to the top of its auxiliary tree.
 * - `splay(u)` only works within the auxiliary tree.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 struct node {
   int p = 0, c[2] = {0, 0}, pp = 0;
   bool flip = 0;
   int sz = 0, ssz = 0, vsz = 0; // ssz=subtree size, vsz=virtual size (dashed edges)
   long long val = 0, sum = 0, lazy = 0, subsum = 0, vsum = 0;
   
   node() {}
   node(int x) {
     val = x; sum = x; sz = 1; lazy = 0;
     ssz = 1; vsz = 0; subsum = x; vsum = 0;
   }
 };
 
 struct LCT {
   vector<node> t;
   LCT(int n) : t(n + 1) {}
 
   int dir(int x, int y) { return t[x].c[1] == y; }
   void set(int x, int d, int y) {
     if (x) t[x].c[d] = y, pull(x);
     if (y) t[y].p = x;
   }
   void pull(int x) {
     if (!x) return;
     int &l = t[x].c[0], &r = t[x].c[1];
     push(l); push(r);
     t[x].sum = t[l].sum + t[r].sum + t[x].val;
     t[x].sz = t[l].sz + t[r].sz + 1;
     t[x].ssz = t[l].ssz + t[r].ssz + t[x].vsz + 1;
     t[x].subsum = t[l].subsum + t[r].subsum + t[x].vsum + t[x].val;
   }
   void push(int x) { 
     if (!x) return;
     int &l = t[x].c[0], &r = t[x].c[1];
     if (t[x].flip) {
       swap(l, r); 
       if (l) t[l].flip ^= 1; 
       if (r) t[r].flip ^= 1;
       t[x].flip = 0;
     }
     if (t[x].lazy) {
       t[x].val += t[x].lazy;
       t[x].sum += t[x].lazy * t[x].sz;
       t[x].subsum += t[x].lazy * t[x].ssz;
       t[x].vsum += t[x].lazy * t[x].vsz;
       if (l) t[l].lazy += t[x].lazy;
       if (r) t[r].lazy += t[x].lazy;
       t[x].lazy = 0;
     }
   }
   void rotate(int x, int d) { 
     int y = t[x].p, z = t[y].p, w = t[x].c[d];
     swap(t[x].pp, t[y].pp);
     set(y, !d, w); set(x, d, y); set(z, dir(z, y), x);
   }
   void splay(int x) { 
     for (push(x); t[x].p;) {
       int y = t[x].p, z = t[y].p;
       push(z); push(y); push(x);
       int dx = dir(y, x), dy = dir(z, y);
       if (!z) rotate(x, !dx); 
       else if (dx == dy) rotate(y, !dx), rotate(x, !dx); 
       else rotate(x, dy), rotate(x, dx);
     }
   }
 
   void make_root(int u) {
     access(u);
     int l = t[u].c[0];
     t[l].flip ^= 1;
     swap(t[l].p, t[l].pp);
     t[u].vsz += t[l].ssz; t[u].vsum += t[l].subsum;
     set(u, 0, 0);
   }
 
   int access(int _u) {
     int last = _u;
     for (int v = 0, u = _u; u; u = t[v = u].pp) {
       splay(u); splay(v);
       t[u].vsz -= t[v].ssz; t[u].vsum -= t[v].subsum;
       int r = t[u].c[1];
       t[u].vsz += t[r].ssz; t[u].vsum += t[r].subsum;
       t[v].pp = 0;
       swap(t[r].p, t[r].pp);
       set(u, 1, v);
       last = u;
     }
     splay(_u);
     return last;
   }
 
   void link(int u, int v) { // link u -> v (u becomes child of v)
     make_root(v); access(u); splay(u);
     t[v].pp = u;
     t[u].vsz += t[v].ssz; t[u].vsum += t[v].subsum;
   }
 
   void cut(int u) { // Cuts u from its parent
     access(u);
     if(t[u].c[0]) {
       t[t[u].c[0]].p = 0;
       t[u].c[0] = 0;
       pull(u);
     }
   }
 
   int find_root(int u) {
     access(u); splay(u); push(u);
     while (t[u].c[0]) { u = t[u].c[0]; push(u); }
     splay(u); return u;
   }
 
   long long query_path(int u, int v) {
     make_root(u); access(v);
     return t[v].sum;
   }
 
   long long query_subtree(int u) {
       // Must define a root logic, usually u's virtual + real sum
       access(u); splay(u); 
       return t[u].vsum + t[u].val; 
   }
 };
/*
 * Dynamic Diameter (Euler Tour + Segment Tree)
 *
 * Description:
 * Maintains tree diameter under edge weight updates.
 * Maps the tree to an Euler Tour (DFS order).
 * Segment Tree maintains: max(dist(u) + dist(v) - 2*dist(lca(u,v))).
 *
 * Variables in Node:
 * a: max(dist(u))
 * b: max(-2 * dist(u))
 * c: max(dist(u) - 2*dist(v))  [Left part of diameter]
 * d: max(-2*dist(u) + dist(v)) [Right part of diameter]
 * e: max diameter in range
 *
 * Usage:
 * 1. Build tree, call dfs(1).
 * 2. Initialize Segment Tree with edge weights.
 * 3. Update edge weights using t.upd(...).
 * 4. t.t[1].e contains the current diameter.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 
 struct node {
     long long a = 0, b = 0, c = 0, d = 0, e = 0;
     // Merge logic
     node operator + (const node &oth) const {
         node ret;
         ret.a = max(a, oth.a);
         ret.b = max(b, oth.b); 
         ret.c = max(max(c, oth.c), a + oth.b);
         ret.d = max(max(d, oth.d), b + oth.a);
         ret.e = max(max(e, oth.e), max(c + oth.a, a + oth.d));
         return ret;
     }
 };
 
 struct ST {
     #define lc (n << 1)
     #define rc ((n << 1) | 1)
     long long lazy[4 * N * 2];
     node t[4 * N * 2];
     
     ST() { memset(lazy, 0, sizeof lazy); }
     
     inline void push(int n, int b, int e) {
         if(lazy[n] == 0) return;
         long long v = lazy[n];
         // Updating depths: a increases by v, b decreases by 2v, etc.
         t[n].a += v; 
         t[n].b -= 2 * v; 
         t[n].c -= v; 
         t[n].d -= v;
         // t[n].e (diameter) is invariant to global depth shift of subtree!
         
         if (b != e) {
             lazy[lc] += v;
             lazy[rc] += v;
         }
         lazy[n] = 0;
     }
     
     inline void pull(int n) { t[n] = t[lc] + t[rc]; }
     
     void upd(int n, int b, int e, int i, int j, long long v) {
         push(n, b, e);
         if(j < b || e < i) return;
         if(i <= b && e <= j) {
             lazy[n] += v;
             push(n, b, e);
             return;
         }
         int mid = (b + e) >> 1;
         upd(lc, b, mid, i, j, v);
         upd(rc, mid + 1, e, i, j, v);
         pull(n);
     }
 } t;
 
 long long W[N];
 vector<pair<int, int>> g[N];
 int T, st[N * 2], en[N * 2], pos[N]; // pos[edge_index] = child_node
 
 void dfs (int u, int p = 0) {
     st[u] = ++T;
     for (auto e : g[u]) {
         int v = e.first, i = e.second;
         if (v == p) continue;
         pos[i] = v; // Edge i corresponds to node v (child)
         dfs(v, u);
         ++T;
     }
     en[u] = T;
 }
 
 int32_t main() {
     ios_base::sync_with_stdio(0); cin.tie(0);
     int n, q; long long mod; 
     if(cin >> n >> q >> mod) {
         for (int i = 1; i < n; i++) {
             int u, v; long long w; cin >> u >> v >> w;
             g[u].push_back({v, i});
             g[v].push_back({u, i});
             W[i] = w;
         }
         dfs(1);
         
         // Init weights
         for (int i = 1; i < n; ++i) 
             t.upd(1, 1, T, st[pos[i]], en[pos[i]], W[i]);
             
         long long last = 0;
         while (q--) {
             int d; long long e; cin >> d >> e;
             d = 1 + (d + last) % (n - 1); 
             e = (e + last) % mod;
             
             // Update range [st[u], en[u]] in Euler tour with delta
             t.upd(1, 1, T, st[pos[d]], en[pos[d]], e - W[d]);
             last = t.t[1].e;
             W[d] = e;
             cout << last << '\n';
         }
     }
     return 0;
 }
/*
 * Heavy-Light Decomposition (HLD)
 *
 * Description:
 * Decomposes tree into chains to support Path and Subtree operations using a Segment Tree.
 *
 * Complexity:
 * Build: O(N)
 * Query/Update Path: O(log^2 N)
 * Query/Update Subtree: O(log N)
 *
 * Tricks & Tweaks:
 * 1. CHANGE OPERATION: Look for "CHANGE HERE" comments in the ST struct.
 * - For Max Query: combine = max(a, b), neutral = -INF.
 * - For Sum Query: combine = a + b, neutral = 0.
 * 2. Edge Values: If values are on edges, push value to deeper node (u or v).
 * - When querying path(u, v), query(u, v) becomes query_up(u, lca) + query_up(v, k-th below lca).
 * - Ignore LCA itself in the query.
 *
 * What Not To Forget:
 * 1. 1-based indexing assumed.
 * 2. If using Lazy Propagation, ensure push() handles range length correctly.
 * 3. Subtree of u is range [st[u], en[u]] in the flattened array.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9, LG = 18, inf = 1e9 + 9;
 
 struct ST {
 #define lc (n << 1)
 #define rc ((n << 1) | 1)
   int t[4 * N], lazy[4 * N];
   ST() {
     fill(t, t + 4 * N, -inf); // CHANGE HERE: 0 for Sum, -inf for Max
     fill(lazy, lazy + 4 * N, 0);
   }
   inline void push(int n, int b, int e) {
     if(lazy[n] == 0) return;
     t[n] = t[n] + lazy[n]; // CHANGE HERE: t[n] += lazy[n] * (e - b + 1) for Sum
     if(b != e) {
       lazy[lc] = lazy[lc] + lazy[n];
       lazy[rc] = lazy[rc] + lazy[n];
     }
     lazy[n] = 0;
   }
   inline int combine(int a, int b) {
     return max(a, b); // CHANGE HERE: a + b for Sum
   }
   inline void pull(int n) {
     t[n] = max(t[lc], t[rc]); // CHANGE HERE: t[lc] + t[rc] for Sum
   }
   void build(int n, int b, int e) { // Call if initial values exist
     if(b == e) {
       t[n] = 0; // Initialize leaf
       return;
     }
     int mid = (b + e) >> 1;
     build(lc, b, mid);
     build(rc, mid + 1, e);
     pull(n);
   }
   void upd(int n, int b, int e, int i, int j, int v) {
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
   int query(int n, int b, int e, int i, int j) {
     push(n, b, e);
     if(i > e || b > j) return -inf; // CHANGE HERE: 0 for Sum
     if(i <= b && e <= j) return t[n];
     int mid = (b + e) >> 1;
     return combine(query(lc, b, mid, i, j), query(rc, mid + 1, e, i, j));
   }
 } t;
 
 vector<int> g[N];
 int par[N][LG + 1], dep[N], sz[N];
 int head[N], st[N], en[N], T;
 int n; // Global N
 
 void dfs_sz(int u, int p = 0) {
   par[u][0] = p;
   dep[u] = dep[p] + 1;
   sz[u] = 1;
   for (int i = 1; i <= LG; i++) par[u][i] = par[par[u][i - 1]][i - 1];
   if (p) g[u].erase(find(g[u].begin(), g[u].end(), p)); // Remove parent from adj
   for (auto &v : g[u]) if (v != p) {
       dfs_sz(v, u);
       sz[u] += sz[v];
       if(sz[v] > sz[g[u][0]]) swap(v, g[u][0]); // Heavy child first
     }
 }
 
 void dfs_hld(int u) {
   st[u] = ++T;
   for (auto v : g[u]) {
     head[v] = (v == g[u][0] ? head[u] : v);
     dfs_hld(v);
   }
   en[u] = T;
 }
 
 int lca(int u, int v) {
   if (dep[u] < dep[v]) swap(u, v);
   for (int k = LG; k >= 0; k--) if (dep[par[u][k]] >= dep[v]) u = par[u][k];
   if (u == v) return u;
   for (int k = LG; k >= 0; k--) if (par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
   return par[u][0];
 }
 
 // K-th ancestor logic if needed
 int kth(int u, int k) {
   for (int i = 0; i <= LG; i++) if (k & (1 << i)) u = par[u][i];
   return u;
 }
 
 int query_up(int u, int v) {
   int ans = -inf; // CHANGE HERE: 0 for Sum
   while(head[u] != head[v]) {
     ans = max(ans, t.query(1, 1, n, st[head[u]], st[u])); // CHANGE combine
     u = par[head[u]][0];
   }
   ans = max(ans, t.query(1, 1, n, st[v], st[u])); // CHANGE combine
   return ans;
 }
 
 // Main API
 int query_path(int u, int v) {
   int l = lca(u, v);
   int ans = query_up(u, l);
   if (v != l) ans = max(ans, query_up(v, kth(v, dep[v] - dep[l] - 1))); // CHANGE combine
   return ans;
 }
 
 void update_path(int u, int v, int val) {
   int l = lca(u, v);
   // Update u -> l
   int cur = u;
   while(head[cur] != head[l]) {
       t.upd(1, 1, n, st[head[cur]], st[cur], val);
       cur = par[head[cur]][0];
   }
   t.upd(1, 1, n, st[l], st[cur], val);
   
   // Update v -> child of l
   if(v != l) {
       cur = v;
       int target = kth(v, dep[v] - dep[l] - 1);
       while(head[cur] != head[target]) {
           t.upd(1, 1, n, st[head[cur]], st[cur], val);
           cur = par[head[cur]][0];
       }
       t.upd(1, 1, n, st[target], st[cur], val);
   }
 }
 
 // Usage Example
 void example_usage() {
     // 1. Build Graph
     // 2. dfs_sz(1);
     // 3. head[1] = 1; dfs_hld(1);
     // 4. t.build(1, 1, n); (Optional)
     // 5. query_path(u, v);
     // 6. t.upd(1, 1, n, st[u], en[u], val); // Subtree Update
 }
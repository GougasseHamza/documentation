/**
 * Binarizing a Tree & Centroid Decomposition
 * * Description:
 * 1. Binarize: Transforms an arbitrary tree into a binary tree by adding dummy nodes.
 * Useful for CD or HLD on trees with high degrees.
 * 2. Centroid Decomposition (CD): Solves path query problems by decomposing tree.
 * * Complexity:
 * Binarization: O(N)
 * CD Construction: O(N log N)
 * * How to use:
 * 1. Build adjacency list in G[].
 * 2. Call binarize(1). This populates g[] with the new binary tree.
 * 3. Run decompose(root) on g[].
 * * What Not To Forget:
 * - The 'g' array (binary tree) needs size up to 2*N (due to dummy nodes).
 * - Dummy edges usually have weight 0.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 2e5 + 9, M = N * 2 + N * 19 * 2; // M handles expanded size
 
 // G is original tree, g is binarized tree
 vector<pair<int, int>> g[N * 2], G[N];
 
 inline void add_edge(int u, int v, int w) {
   g[u].push_back({v, w});
 }
 
 int T; // Counter for dummy nodes
 void binarize(int u, int p = 0) {
   int last = 0, tmp = 0;
   for (auto e : G[u]) {
     int v = e.first, w = e.second;
     if (v == p) continue;
     ++tmp;
     // Standard binarization logic: Chain children with dummy nodes
     if (tmp == 1) {
       add_edge(u, v, w); add_edge(v, u, w);
       last = u;
     }
     else if (tmp == ((int) G[u].size()) - (u != 1)) {
       add_edge(last, v, w); add_edge(v, last, w);
     } 
     else {
       T++; // New dummy node
       add_edge(last, T, 0); add_edge(T, last, 0);
       last = T;
       add_edge(T, v, w); add_edge(v, T, w);
     }
   }
   for (auto e : G[u]) {
     int v = e.first;
     if (v == p) continue;
     binarize(v, u);
   }
 }
 
 // --- CENTROID DECOMPOSITION BELOW ---
 int sz[N * 2];
 int tot, done[N * 2], cenpar[N * 2];
 
 void calc_sz(int u, int p) {
   tot ++; sz[u] = 1;
   for (auto e: g[u]) {
     int v = e.first;
     if(v == p || done[v]) continue;
     calc_sz(v, u);
     sz[u] += sz[v];
   }
 }
 
 int find_cen(int u, int p) {
   for (auto e: g[u]) {
     int v = e.first;
     if(v == p || done[v]) continue;
     else if(sz[v] > tot / 2) return find_cen(v, u);
   }
   return u;
 }
 
 long long d[20][N * 2]; // Distance at each CD level
 void dist_dfs(int u, int p, long long nw, int l) {
   d[l][u] = nw;
   for(auto e : g[u]) {
     int v = e.first; long long w = e.second;
     if (v == p || done[v]) continue;
     dist_dfs(v, u, nw + w, l);
   }
 }
 
 int st[N * 2], en[N * 2], DT;
 struct node {
   vector<int> ct; 
   int level = 0, id = 0, cnt = 0;
   long long sum = 0, parsum = 0;
 } t[M];
 
 int decompose(int u, int p = 0, int l = 0) {
   tot = 0;
   calc_sz(u, p);
   int cen = find_cen(u, p);
   cenpar[cen] = p; done[cen] = 1;
   u = cen; st[u] = ++DT;
   t[u].id = u; t[u].level = l;
   
   dist_dfs(u, p, 0, l);
   
   for (auto e: g[u]) {
     int v = e.first;
     if(v == p || done[v]) continue;
     int x = decompose(v, u, l + 1);
     t[u].ct.push_back(x);
   }
   en[u] = DT;
   return u;
 }
 
 // Specific problem logic (Update/Query) omitted for brevity in template,
 // but the CD structure above is the core.
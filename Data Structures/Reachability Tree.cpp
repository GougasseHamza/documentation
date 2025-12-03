/**
 * Reachability Tree / Kruskal Reconstruction Tree
 *
 * * PROBLEMS SOLVED:
 * 1. "Find all nodes reachable from U using edges with weight <= W".
 * (Answer: Nodes in subtree of lift(u, W)).
 * 2. "What is the minimum weight of the maximum edge (bottleneck) on path u->v?"
 * (Answer: val[lca(u, v)]).
 *
 * * COMPLEXITY:
 * Build: O(M log M).
 * Lift: O(log N).
 *
 * * LOGIC:
 * - Sort edges by weight.
 * - Iterate edges. If (u, v) not connected:
 * - Create new virtual node 'new_id' with weight = edge.w.
 * - Make 'new_id' the parent of find(u) and find(v).
 * - DSU merge (u, v) into 'new_id'.
 * - Run DFS to get Euler Tour (st/en times) to handle subtree queries.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9, LG = 18;
 
 struct KruskalReconstructionTree {
   // par: DSU parent / Tree parent
   // val: Weight of the edge this node represents
   // st/en: Euler tour entry/exit times (linearizes the subtree)
   // rid: Reverse ID (maps time -> node index)
   int n, par[2 * N], sp[LG + 1][2 * N], val[2 * N], rid[2 * N];
   int T, st[2 * N], en[2 * N];
   vector<int> g[2 * N]; // Adjacency list for the tree
 
   // DSU Find
   int find(int x) {
     if (par[x] == x) return x;
     return par[x] = find(par[x]);
   }
 
   // DFS to build Sparse Table (LCA/Lift) and Euler Tour
   void dfs(int u) {
     st[u] = T + 1;
     for (int v : g[u]) {
       sp[0][v] = u; // Immediate parent for lifting
       dfs(v);
     }
     // Leaf node tracking (if needed)
     if (st[u] == T + 1) {
       rid[T + 1] = u; 
       T++;
     }
     en[u] = T;
   }
 
   // Build the Tree
   // Input: vector of {weight, u, v}
   void build(vector<array<int, 3>> e) { 
     n = e.size() + 1; // Approx num nodes
     sort(e.begin(), e.end()); // Sort edges by weight (Min Spanning Tree logic)
     
     // Init DSU and Tree
     for (int x = 1; x < 2 * n; x++) {
       par[x] = x; g[x].clear();
       val[x] = 0; sp[0][x] = 0;
     }
 
     int real_n = 0; // Track max ID used
     // Iterate sorted edges
     for (int i = 0; i < e.size(); i++) {
       int u = e[i][1], v = e[i][2], w = e[i][0];
       real_n = max({real_n, u, v});
       
       int root_u = find(u);
       int root_v = find(v);
       
       if (root_u != root_v) {
         int id = real_n + 1 + i; // New virtual node ID (starts after original nodes)
         // Link old roots to new virtual node
         par[root_u] = par[root_v] = id; 
         val[id] = w; // Store edge weight
         g[id].push_back(root_u);
         g[id].push_back(root_v);
         real_n = max(real_n, id); // Update max ID
       }
     }
     
     val[0] = 2e9; // Sentinel
     T = 0;
     
     // Run DFS from the absolute root of the KRT
     // Note: If graph is disconnected, run loop over all components
     int root = find(1); // Assuming 1 is in the main component or finding root of 1
     dfs(root); 
 
     // Build Sparse Table for Lifting
     for (int k = 1; k <= LG; k++) {
       for (int u = 1; u <= real_n; u++) 
         sp[k][u] = sp[k - 1][sp[k - 1][u]];
     }
   }
 
   // Find the highest ancestor of 'u' such that all edges in subtree are <= x.
   // This corresponds to the component containing 'u' using edges <= x.
   int lift(int u, int x) { 
     for (int k = LG; k >= 0; k--) {
       // If parent exists and its weight is still within limit x, go up
       if (sp[k][u] != 0 && val[sp[k][u]] <= x) 
         u = sp[k][u];
     }
     return u;
   }
   
   // TWEAK: To solve "Min weight on path", implement standard LCA
   // and return val[lca(u, v)].
 } rt;
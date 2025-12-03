/*
 * Centroid Decomposition
 *
 * Description:
 * Recursive decomposition of a tree. The centroid is a node which, if removed,
 * splits the tree into subtrees of size at most N/2.
 * The 'cenpar' array forms a new tree (Centroid Tree) of height O(log N).
 *
 * Complexity:
 * Construction: O(N log N)
 * Query/Update: O(log N) or O(log^2 N) depending on auxiliary structures.
 *
 * Usage:
 * 1. Build adjacency list 'g'.
 * 2. Call 'decompose(1, 0)'.
 * 3. Use 'cenpar[u]' to move up the centroid tree.
 * 4. Usually requires 'dist(u, v)' in O(1) or O(log N) (LCA needed).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 
 vector<int> g[N];
 int sz[N], tot;
 bool done[N]; // Marks nodes removed in CD
 int cenpar[N]; // Parent in Centroid Tree
 
 // 1. Calculate Subtree Sizes
 void calc_sz(int u, int p) {
   tot ++;
   sz[u] = 1;
   for (auto v : g[u]) {
     if(v == p || done[v]) continue;
     calc_sz(v, u);
     sz[u] += sz[v];
   }
 }
 
 // 2. Find Centroid
 int find_cen(int u, int p) {
   for (auto v : g[u]) {
     if(v == p || done[v]) continue;
     else if(sz[v] > tot / 2) return find_cen(v, u);
   }
   return u;
 }
 
 // 3. Decompose
 void decompose(int u, int pre) {
   tot = 0;
   calc_sz(u, pre);
   int cen = find_cen(u, pre);
   
   cenpar[cen] = pre; // Link in centroid tree
   done[cen] = 1;     // Remove this node
   
   for(auto v : g[cen]) {
     if(v == pre || done[v]) continue;
     decompose(v, cen);
   }
 }
 
 // --- Specific Problem Logic (Example: CF 321 C) ---
 int dep[N];
 void get_depths(int u, int p) { // Just for the CF problem output
     for(auto v : g[u]) {
         if(v == p) continue;
         dep[v] = dep[u] + 1;
         get_depths(v, u);
     }
 }
 
 int main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n; cin >> n;
   for(int i = 1; i < n; i++) {
     int u, v; cin >> u >> v;
     g[u].push_back(v); g[v].push_back(u);
   }
 
   decompose(1, 0);
 
   // Example: Outputting ranks for Codeforces 321C
   // Re-using 'g' to build the actual Centroid Tree for traversal (Optional)
   for(int i = 1; i <= n; i++) g[i].clear();
   int root = 0;
   for(int i = 1; i <= n; i++) {
     if (cenpar[i] == 0) root = i;
     else {
         g[cenpar[i]].push_back(i);
         g[i].push_back(cenpar[i]);
     }
   }
   
   get_depths(root, 0);
   for(int i = 1; i <= n; i++) cout << char(dep[i] + 'A') << ' ';
   
   return 0;
 }
/**
 * Centroid Decomposition
 *
 * * PROBLEMS SOLVED:
 * 1. Divide and Conquer on Trees.
 * 2. Path Counting: Count paths with length K (or property P).
 * 3. Distance Queries: "Nearest marked node" updates/queries.
 *
 * * COMPLEXITY:
 * Build: O(N log N). Because recursion depth is O(log N).
 * Space: O(N).
 *
 * * LOGIC:
 * A Centroid is a node that, if removed, splits the tree into components of size <= N/2.
 * We recursively find centroids and link them to form a "Centroid Tree" (cenpar[]).
 * The height of the Centroid Tree is at most O(log N).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 
 vector<int> g[N];    // Original Tree
 int sz[N];           // Subtree size
 int tot;             // Total nodes in current component
 int done[N];         // 'Removed' nodes (centroids of higher levels)
 int cenpar[N];       // Parent in Centroid Tree
 
 // 1. Calculate subtree sizes for the current component (ignoring 'done' nodes)
 void calc_sz(int u, int p) {
   tot++; sz[u] = 1;
   for (auto v : g[u]) {
     if(v == p || done[v]) continue;
     calc_sz(v, u);
     sz[u] += sz[v];
   }
 }
 
 // 2. Find the centroid: Move to a child if its subtree size > tot/2
 int find_cen(int u, int p) {
   for (auto v : g[u]) {
     if(v == p || done[v]) continue;
     else if(sz[v] > tot / 2) return find_cen(v, u);
   }
   return u;
 }
 
 // 3. Recursive Decomposition
 void decompose(int u, int pre) {
   tot = 0;
   calc_sz(u, pre);        // Compute sizes
   int cen = find_cen(u, pre); // Find centroid
   
   cenpar[cen] = pre;      // Build Centroid Tree edge
   done[cen] = 1;          // Mark as processed (virtually remove)
   
   for(auto v : g[cen]) {
     if(v == pre || done[v]) continue;
     decompose(v, cen);    // Recurse on subtrees
   }
 }
 
 // Helper: DFS for Depth (Just for the Codeforces problem logic)
 int dep[N];
 void dfs(int u, int p = 0) {
     // ... logic for depths ...
 }
 
 int main() {
     // Usage
     // 1. Build graph g[]
     // 2. decompose(1, 0)
     // 3. Solve problem using cenpar[] hierarchy.
     //    Usually involves jumping up the centroid tree from a node to the root.
     return 0;
 }
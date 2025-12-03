/*
 * DSU on Tree (Sack / Small-to-Large)
 *
 * Description:
 * Efficiently processes subtree queries (e.g., count distinct colors).
 * Keeps the result of the 'heavy' child and merges 'light' children into it.
 *
 * Complexity: O(N log N)
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 vector<int> g[N];
 int sz[N], col[N], cnt[N];
 bool big[N]; // Is node a heavy child?
 int ans[N];  // Store answers
 
 // 1. Precompute Sizes
 void getsz(int u, int p) {
   sz[u] = 1;
   for (auto v : g[u]) {
     if (v == p) continue;
     getsz(v, u);
     sz[u] += sz[v];
   }
 }
 
 // 2. Add/Remove Subtree contribution
 // Modify this for your specific problem
 void add(int u, int p, int x) {
   cnt[col[u]] += x; 
   for (auto v : g[u]) {
     if (v == p || big[v]) continue; // Don't re-process heavy child
     add(v, u, x);
   }
 }
 
 // 3. Main Algorithm
 void dsu(int u, int p, bool keep) {
   int bigchild = -1, mx = -1;
   // Find heavy child
   for (auto v : g[u]) {
     if (v == p) continue;
     if (sz[v] > mx) mx = sz[v], bigchild = v;
   }
   
   // Run for light children (and clear)
   for (auto v : g[u]) {
     if (v == p || v == bigchild) continue;
     dsu(v, u, 0);
   }
   
   // Run for heavy child (and keep)
   if (bigchild != -1) {
     big[bigchild] = 1;
     dsu(bigchild, u, 1);
   }
   
   // Add light children contribution + current node
   add(u, p, 1);
   
   // --- Answer Query for u ---
   ans[u] = cnt[col[u]]; // Example query
   // --------------------------
   
   if (bigchild != -1) big[bigchild] = 0;
   if (keep == 0) add(u, p, -1); // Clear if light child
 }
 
 int32_t main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n = 5; // Example
   getsz(1, 0);
   dsu(1, 0, 1);
   return 0;
 }
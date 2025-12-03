#include<bits/stdc++.h>
using namespace std;

/*Given an array a that contains n elements and the
operation op that satisfies associative property:
(x op y) op z=x op (y op z) is true for any x, y, z.

The following implementation of Sqrt Tree can perform the following operations:
build in O(nloglogn),
answer queries in O(1) and update an element in O(sqrt(n)).*/

/**
 * SQRT Tree
 * * PROBLEMS: Static Range Queries (Sum, Min, Max, GCD, Product).
 * * COMPLEXITY: Query O(1), Build O(N log log N).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 // TWEAK: Change type and operation
 #define SqrtTreeItem int
 
 // Associative Operation (e.g., min, +, gcd)
 SqrtTreeItem op(const SqrtTreeItem &a, const SqrtTreeItem &b) {
   return a + b; 
 }
 
 inline int log2Up(int n) {
   int res = 0; while ((1 << res) < n) res++; return res;
 }
 
 struct SqrtTree {
   int n, llg; 
   int indexSz; // Size of the compressed array for higher layers
   vector<SqrtTreeItem> v; // Original data
   vector<int> clz, layers, onLayer; // Bit manipulation helpers
   
   // core structures
   vector< vector<SqrtTreeItem> > pref, suf, between;
 
   // Precompute prefix and suffix for a specific block
   inline void buildBlock(int layer, int l, int r) {
     pref[layer][l] = v[l];
     for (int i = l + 1; i < r; i++) 
       pref[layer][i] = op(pref[layer][i - 1], v[i]);
       
     suf[layer][r - 1] = v[r - 1];
     for (int i = r - 2; i >= l; i--) 
       suf[layer][i] = op(v[i], suf[layer][i + 1]);
   }
 
   // Build the summary array "between" blocks
   inline void buildBetween(int layer, int lBound, int rBound, int betweenOffs) {
     int bSzLog = (layers[layer] + 1) >> 1; // ~ log(sqrt(Size))
     int bCntLog = layers[layer] >> 1;
     int bSz = 1 << bSzLog;
     int bCnt = (rBound - lBound + bSz - 1) >> bSzLog;
     
     for (int i = 0; i < bCnt; i++) {
       SqrtTreeItem ans;
       for (int j = i; j < bCnt; j++) {
         // Calculate accumulation of blocks i through j
         SqrtTreeItem add = suf[layer][lBound + (j << bSzLog)];
         ans = (i == j) ? add : op(ans, add);
         between[layer - 1][betweenOffs + lBound + (i << bCntLog) + j] = ans;
       }
     }
   }
 
   // Base case construction for small arrays
   inline void buildBetweenZero() {
     int bSzLog = (llg + 1) >> 1;
     for (int i = 0; i < indexSz; i++) v[n + i] = suf[0][i << bSzLog];
     build(1, n, n + indexSz, (1 << llg) - n);
   }
 
   // Main recursive build
   void build(int layer, int lBound, int rBound, int betweenOffs) {
     if (layer >= (int)layers.size()) return;
     
     int bSz = 1 << ((layers[layer] + 1) >> 1);
     for (int l = lBound; l < rBound; l += bSz) {
       int r = min(l + bSz, rBound);
       buildBlock(layer, l, r); // Build local prefix/suffix
       build(layer + 1, l, r, betweenOffs); // Recurse into block
     }
     
     if (layer == 0) buildBetweenZero();
     else buildBetween(layer, lBound, rBound, betweenOffs);
   }
 
   // O(1) Query Logic
   inline SqrtTreeItem query(int l, int r, int betweenOffs, int base) {
     if (l == r) return v[l];
     if (l + 1 == r) return op(v[l], v[r]);
     
     // Find lowest layer where l and r are in different blocks
     int layer = onLayer[clz[(l - base) ^ (r - base)]];
     
     int bSzLog = (layers[layer] + 1) >> 1;
     int bCntLog = layers[layer] >> 1;
     
     // Identify block boundaries
     int lBound = (((l - base) >> layers[layer]) << layers[layer]) + base;
     int lBlock = ((l - lBound) >> bSzLog) + 1;
     int rBlock = ((r - lBound) >> bSzLog) - 1;
     
     // Suffix of L's block
     SqrtTreeItem ans = suf[layer][l];
     
     // If there are full blocks between L's block and R's block
     if (lBlock <= rBlock) {
       SqrtTreeItem add = (layer == 0) ? (
                    query(n + lBlock, n + rBlock, (1 << llg) - n, n)
                  ) : (
                    between[layer - 1][betweenOffs + lBound + (lBlock << bCntLog) + rBlock]
                  );
       ans = op(ans, add);
     }
     
     // Prefix of R's block
     ans = op(ans, pref[layer][r]);
     return ans;
   }
 
   inline SqrtTreeItem query(int l, int r) { return query(l, r, 0, 0); }
 
   // Initialization
   SqrtTree(const vector<SqrtTreeItem>& a)
     : n((int)a.size()), llg(log2Up(n)), v(a), clz(1 << llg), onLayer(llg + 1) {
     // Precompute bit hacks
     clz[0] = 0;
     for (int i = 1; i < (int)clz.size(); i++) clz[i] = clz[i >> 1] + 1;
     
     int tllg = llg;
     while (tllg > 1) {
       onLayer[tllg] = (int)layers.size();
       layers.push_back(tllg);
       tllg = (tllg + 1) >> 1;
     }
     for (int i = llg - 1; i >= 0; i--) onLayer[i] = max(onLayer[i], onLayer[i + 1]);
     
     // Resize and allocate
     int bSzLog = (llg + 1) >> 1;
     int bSz = 1 << bSzLog;
     indexSz = (n + bSz - 1) >> bSzLog;
     v.resize(n + indexSz);
     
     pref.assign(layers.size(), vector<SqrtTreeItem>(n + indexSz));
     suf.assign(layers.size(), vector<SqrtTreeItem>(n + indexSz));
     int betweenLayers = max(0, (int)layers.size() - 1);
     between.assign(betweenLayers, vector<SqrtTreeItem>((1 << llg) + bSz));
     
     build(0, 0, n, 0);
   }
 };
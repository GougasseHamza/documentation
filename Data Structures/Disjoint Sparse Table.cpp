/*
 * Disjoint Sparse Table
 *
 * Description:
 * O(1) Range Queries for non-idempotent functions (Product, Matrix Mul, etc.).
 * Standard Sparse Table only works for Min/Max/GCD (idempotent) in O(1).
 *
 * Logic:
 * Computes prefix/suffix from midpoints of intervals [k*2^j, (k+1)*2^j].
 * Query combines suffix of left part + prefix of right part.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 int mod = 1e9 + 7;
 
 struct DST {
   vector<vector<int>> left, right;
   int k, n;
   
   DST(vector<int> & a) {
     n = (int)a.size();
     k = floor(log2(n)) + 2;
     left.assign(k + 1, vector<int>(n));
     right.assign(k + 1, vector<int>(n));
     
     for(int j = 0; (1 << j) <= n; ++j) {
       int mask = (1 << j) - 1;
       int nw = 1; // Neutral element (1 for product, 0 for sum)
       
       // Build Left (Prefix from block start)
       for(int i = 0; i < n; ++i) {
         nw = 1LL * nw * a[i] % mod; 
         left[j][i] = nw;
         if((i & mask) == mask) nw = 1; // Reset at block boundary
       }
       
       // Build Right (Suffix from block end)
       nw = 1;
       for(int i = n - 1; i >= 0; --i) {
         nw = 1LL * nw * a[i] % mod;
         right[j][i] = nw;
         if((i & mask) == 0) nw = 1; 
       }
     }
   }
   
   int query(int l, int r) {
     if(l == r) return left[0][l]; // Base case (often needed if 0-level logic varies)
     // Find highest bit where l and r differ
     // This gives the level where l and r are in different halves of a block
     int i = 31 - __builtin_clz(l ^ r);
     
     int uno = left[i][r];  // Prefix of right part
     int dos = right[i][l]; // Suffix of left part
     return 1LL * uno * dos % mod;
   }
 };
 
 int32_t main() {
   int n = 5;
   vector<int> a = {1, 2, 3, 4, 5};
   DST t(a);
   cout << t.query(1, 3) << endl; // 2*3*4 = 24
   return 0;
 }
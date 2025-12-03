/*
 * Disjoint Set Union (DSU)
 *
 * Description:
 * Standard implementation with Path Compression and Union by Size/Rank.
 * Maintains component size and total component count.
 *
 * Complexity: O(alpha(N)) ~ O(1)
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 struct DSU {
   vector<int> par, rnk, sz;
   int c; // Number of connected components
   
   DSU(int n) : par(n + 1), rnk(n + 1, 0), sz(n + 1, 1), c(n) {
     for (int i = 1; i <= n; ++i) par[i] = i;
   }
   
   int find(int i) {
     return (par[i] == i ? i : (par[i] = find(par[i])));
   }
   
   bool same(int i, int j) {
     return find(i) == find(j);
   }
   
   int get_size(int i) {
     return sz[find(i)];
   }
   
   int count() {
     return c;
   }
   
   // Returns new root or -1 if already same
   int merge(int i, int j) {
     if ((i = find(i)) == (j = find(j))) return -1;
     else --c;
     
     // Union by Rank
     if (rnk[i] > rnk[j]) swap(i, j);
     par[i] = j;
     sz[j] += sz[i];
     if (rnk[i] == rnk[j]) rnk[j]++;
     
     return j;
   }
 };
 
 int32_t main() {
   DSU d(10);
   d.merge(1, 2);
   d.merge(2, 3);
   cout << d.get_size(1) << endl; // 3
   return 0;
 }
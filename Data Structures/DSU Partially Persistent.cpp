/*
 * Partially Persistent DSU
 *
 * Description:
 * Supports 'merge(u, v)' at current time.
 * Supports 'find(u, t)' and 'size(u, t)' at any past time t.
 *
 * Complexity: O(log N) per operation.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 
 struct DSU {
   // par stores {parent, time_of_merge}
   // If vector size > 1, it means node was merged multiple times? 
   // Actually, typical implementation:
   // par[u] stores history of parents. But usually simpler:
   // par[u] = {p, t}. Since u only merges once into something else (unless root changes logic).
   // This implementation supports size history.
   vector<vector<pair<int, int>>> par; 
   int time = 0;
   
   DSU(int n) : par(n + 1, {{-1, 0}}) {}
   
   // Returns current root (for internal merging)
   int root(int u, int t) {
     // Traverse up as long as link exists at time t
     if (par[u].back().first >= 0 && par[u].back().second <= t) 
         return root(par[u].back().first, t);
     return u;
   }
   
   bool merge(int u, int v) {
     ++time;
     u = root(u, time); v = root(v, time);
     if (u == v) return 0;
     
     // Union by size (stored as negative in par[].back().first)
     if (par[u].back().first > par[v].back().first) swap(u, v);
     
     // Update v size: add u's size
     par[u].push_back({par[u].back().first + par[v].back().first, time});
     // Link v to u
     par[v].push_back({u, time}); 
     return 1;
   }
   
   bool same(int u, int v, int t) {
     return root(u, t) == root(v, t);
   }
   
   int size(int u, int t) {
     u = root(u, t);
     // Binary search for size at time t
     int l = 0, r = (int) par[u].size() - 1, ans = 0;
     while (l <= r) {
       int mid = (l + r) >> 1;
       if (par[u][mid].second <= t) ans = mid, l = mid + 1;
       else r = mid - 1;
     }
     return -par[u][ans].first;
   }
 };
 
 int a[N];
 int32_t main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n = 5, m = 5;
   DSU d(n);
   d.merge(1, 2); // Time 1
   cout << d.same(1, 2, 0) << endl; // 0 (False)
   cout << d.same(1, 2, 1) << endl; // 1 (True)
   return 0;
 }
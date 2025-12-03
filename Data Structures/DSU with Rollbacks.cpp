/*
 * DSU with Rollbacks & DSU Queue
 *
 * Description:
 * 1. DSU: Supports merge() and undo(). No path compression (O(log N)).
 * Maintains bipartite status of components.
 * 2. DSUQueue: Supports push() and pop() (FIFO) using two stacks + Rollback DSU.
 *
 * Usage:
 * DSU d(n); d.merge(u, v); d.undo();
 * DSUQueue q(n); q.push(update); q.pop();
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 2e5 + 9;
 
 struct DSU {
   vector<int> par, sz, w; // w[i] = parity/color relative to parent
   vector<array<int, 3>> op; // Stores history for undo
   bool flag; // Is Graph Bipartite?
   
   DSU() {}
   DSU(int n) {
     par.resize(n + 1); sz.resize(n + 1); w.resize(n + 1);
     flag = true;
     for (int i = 1; i <= n; i++) { par[i] = i; sz[i] = 1; w[i] = 0; }
   }
 
   // Find with distance to root (parity)
   pair<int, int> find(int u) {
     int ans = 0;
     while (par[u] != u)  {
       ans ^= w[u];
       u = par[u];
     }
     return {u, ans};
   }
 
   bool merge(int u, int v) {
     auto pu = find(u), pv = find(v);
     u = pu.first; v = pv.first;
     int last = flag;
     int z = pu.second ^ pv.second ^ 1; // Expected edge weight for bipartite
     
     if (u == v) {
       if (z) flag = false; // Odd cycle detected
       op.push_back({-1, -1, last}); // No-op
       return false;
     }
     
     if (sz[u] > sz[v]) swap(u, v);
     
     op.push_back({u, w[u], last});
     par[u] = v;
     w[u] = z;
     sz[v] += sz[u];
     return true;
   }
 
   void undo() {
     auto x = op.back(); op.pop_back();
     flag = x[2];
     int u = x[0];
     if (u != -1) {
       sz[par[u]] -= sz[u];
       par[u] = u;
       w[u] = x[1];
     }
   }
   
   bool is_bipartite() { return flag; }
 };
 
 // Queue-like DSU (SWERC 2020 Trick)
 struct update { int x, y; bool type; }; // type: internal logic
 
 struct DSUQueue {
   DSU D;
   vector<update> S; // Stack
   
   DSUQueue(int n) { D = DSU(n); }
   
   void push(update u) {
     D.merge(u.x, u.y);
     S.push_back(u);
   }
   
   void pop() {
     vector<update> t[2];
     // Move elements from Main Stack to Temp Stacks to reverse order
     do {
       t[S.back().type].push_back(S.back());
       S.pop_back();
       D.undo();
     } while (t[1].size() < t[0].size() && !S.empty());
     
     if (t[1].empty()) {
       for (auto &u : t[0]) { u.type = 1; push(u); }
     } else {
       for (int i : {0, 1}) {
         reverse(t[i].begin(), t[i].end());
         for (auto &u : t[i]) push(u);
       }
     }
     S.pop_back(); D.undo();
   }
   
   bool is_bipartite() { return D.is_bipartite(); }
 };
 
 int a[N], u[N], v[N];
 
 int32_t main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n, m, q; 
   if(cin >> n >> m >> q) {
       for (int i = 1; i <= m; i++) cin >> u[i] >> v[i];
       
       DSUQueue D(n);
       // Example: Sliding window bipartite check
       for (int l = 1, r = 1; l <= m; l++) {
         while (r <= m && (r < l || !D.is_bipartite())) {
            // In actual problem, logic slightly diff, but this shows usage
            if(!D.is_bipartite() && r > l) break; // Invalid state
            D.push({u[r], v[r], 0});
            r++;
         }
         // ... Logic ...
         if(l < r) D.pop();
       }
   }
   return 0;
 }
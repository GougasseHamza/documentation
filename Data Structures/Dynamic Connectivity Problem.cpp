/*
 * Dynamic Connectivity (Offline)
 *
 * Description:
 * Handles dynamic edge updates (add/remove) and connectivity queries.
 * Uses a "Segment Tree over Time".
 * Edges are added to the segment tree nodes covering their lifespan.
 * DFS traverses the tree: add edges -> recurse -> undo edges.
 *
 * Complexity: O(Q log Q log N)
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 
 // DSU with Rollback support
 struct persistent_dsu {
   struct state {
     int u, v, rnku, rnkv;
     state() : u(-1), v(-1), rnku(-1), rnkv(-1) {}
     state(int _u, int _rnku, int _v, int _rnkv) : u(_u), rnku(_rnku), v(_v), rnkv(_rnkv) {}
   };
 
   stack<state> st;
   int par[N], depth[N];
   int comp; // Number of components
 
   void init(int n) {
     comp = n;
     for(int i = 0; i <= n; i++) {
       par[i] = i;
       depth[i] = 1;
     }
   }
 
   int root(int x) {
     while(x != par[x]) x = par[x];
     return x;
   }
 
   void unite(int x, int y) {
     int rx = root(x), ry = root(y);
     if(rx == ry) {
       st.push(state()); // Push dummy state to keep stack sync
       return;
     }
     if(depth[rx] < depth[ry]) swap(rx, ry);
     
     par[ry] = rx;
     st.push(state(rx, depth[rx], ry, depth[ry]));
     if(depth[rx] == depth[ry]) depth[rx]++;
     comp--;
   }
 
   void backtrack(int c) { // Undo last c operations
     while(!st.empty() && c) {
       state top = st.top(); st.pop();
       c--;
       if(top.u == -1) continue; // Dummy
       par[top.v] = top.v;
       depth[top.u] = top.rnku;
       comp++;
     }
   }
 } d;
 
 vector<pair<int, int>> alive[4 * N];
 void upd(int n, int b, int e, int i, int j, pair<int, int> &p) {
   if(b > j || e < i) return;
   if(b >= i && e <= j) {
     alive[n].push_back(p);
     return;
   }
   int mid = (b + e) >> 1;
   upd(2 * n, b, mid, i, j, p);
   upd(2 * n + 1, mid + 1, e, i, j, p);
 }
 
 int ans[N];
 void solve(int n, int b, int e) {
   int prevsz = d.st.size();
   for(auto p : alive[n]) d.unite(p.first, p.second);
   
   if(b == e) {
     ans[b] = d.comp;
   } else {
     int mid = (b + e) >> 1;
     solve(2 * n, b, mid);
     solve(2 * n + 1, mid + 1, e);
   }
   
   // Rollback
   d.backtrack(d.st.size() - prevsz);
 }
 
 struct HASH {
   size_t operator()(const pair<int, int>&x)const {
     return hash<long long>()(((long long)x.first) ^ (((long long)x.second) << 32));
   }
 };
 
 bool isquery[N];
 unordered_map<pair<int, int>, int, HASH> st; // Stores start time of edges
 
 int main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n, m; 
   if(cin >> n >> m) {
       d.init(n);
       for(int i = 1; i <= m; i++) {
         string ty; cin >> ty;
         if(ty == "?") {
           isquery[i] = 1;
         } else {
           int u, v; cin >> u >> v;
           if(u > v) swap(u, v);
           pair<int, int> p = {u, v};
           
           if(ty == "+") {
             st[p] = i;
           } else {
             upd(1, 1, m, st[p], i - 1, p);
             st.erase(p);
           }
         }
       }
       // Edges still alive at end
       for(auto p : st) { auto &pp = p.first ; pair<int,int> z = pp; upd(1, 1, m, p.second, m, z);}
       
       solve(1, 1, m);
       
       for(int i = 1; i <= m; i++) 
         if(isquery[i]) cout << ans[i] << '\n';
   }
   return 0;
 }
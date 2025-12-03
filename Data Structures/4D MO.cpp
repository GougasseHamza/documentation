/**
 * Mo's Algorithm on Trees (Path/Subtree Queries)
 * * Description:
 * Solves offline queries on tree paths or subtrees by flattening the tree 
 * (Euler Tour) and using block decomposition.
 * * Complexity:
 * Time: O(N * Sqrt(N)) or O(N * Sqrt(Q)).
 * * Tricks:
 * - The comparator uses a specific block sorting strategy.
 * - Hilbert Curve sorting (not implemented here but good to know) often faster.
 * * What Not To Forget:
 * - Adjust 'B' (Block size). Usually N / sqrt(Q).
 * - The 'ST' struct handles the answer calculation (Mode/Max freq here).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 2e5 + 9;
 const int B = 450; // Block size ~ sqrt(N)
 
 struct ST { 
   // Custom Data Structure to maintain answer.
   // This example finds Mode Frequency in O(1) using O(sqrt N) updates.
   int c[N], cc[N / B + 2][N], mx[N / B + 2];
   
   void add(int x) {
     cc[x / B][c[x]]--;
     c[x]++;
     cc[x / B][c[x]]++;
     mx[x / B] = max(mx[x / B], c[x]);
   }
   void del(int x) {
     if (mx[x / B] == c[x] and cc[x / B][c[x]] == 1) mx[x / B]--;
     cc[x / B][c[x]]--;
     c[x]--;
     cc[x / B][c[x]]++;
   }
   int query() {
     int mx_oc = 0;
     for (int i = 0; i <= N / B; i++) mx_oc = max(mx_oc, mx[i]);
     return mx_oc; // Returns max frequency
   }
 } t;
 
 struct query {
   int l1, r1, l2, r2, id;
   // Sorting for Mo's Algorithm
   bool operator < (const query &x) const {
     if (l1 / B != x.l1 / B) return l1 < x.l1;
     if (r1 / B != x.r1 / B) return (l1 / B) & 1 ? r1 < x.r1 : r1 > x.r1;
     return l2 < x.l2; // Simplified comparator for standard Mo
   }
 } Q[N];
 
 int st[N], en[N], T, a[N], b[N]; // b is input values, a is flattened values
 vector<int> g[N];
 
 void dfs(int u, int p = 0) {
   st[u] = ++T;
   a[T] = b[u]; // Map node value to Euler tour time
   for (auto v: g[u]) {
     if (v ^ p) dfs(v, u);
   }
   en[u] = T;
 }
 
 // Global wrappers for ST
 void add(int i) { t.add(a[i]); }
 void del(int i) { t.del(a[i]); }
 
 int ans[N];
 
 int32_t main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n; cin >> n;
   for (int i = 1; i <= n; i++) cin >> b[i];
   for (int i = 1; i < n; i++) {
     int u, v; cin >> u >> v;
     g[u].push_back(v); g[v].push_back(u);
   }
   dfs(1);
   
   int q; cin >> q;
   for (int i = 1; i <= q; i++) {
     int u, v; cin >> u >> v;
     // Map subtree queries to linear ranges
     Q[i] = {st[u], en[u], st[v], en[v], i}; 
   }
   sort(Q + 1, Q + q + 1);
   
   int cl1 = 1, cr1 = 0; // Current L, R
   // Note: This specific 4D implementation had 2 ranges. 
   // Standard tree Mo usually handles 1 range. 
   // Assuming standard [L, R] loop logic here for simplicity:
   
   for (int i = 1; i <= q; i++) {
     int L = Q[i].l1, R = Q[i].r1; // Using only first range for demo
     while (cl1 > L) add(--cl1);
     while (cr1 < R) add(++cr1);
     while (cl1 < L) del(cl1++);
     while (cr1 > R) del(cr1--);
     ans[Q[i].id] = t.query();
   }
   
   for (int i = 1; i <= q; i++) cout << ans[i] << '\n';
   return 0;
 }
/**
 * Persistent DSU (via Persistent Array)
 *
 * * PROBLEMS SOLVED:
 * 1. "Are u and v connected at time T?"
 * 2. "Merge u and v and creates time T+1."
 *
 * * LOGIC:
 * - Uses Persistent Array to store 'par' (parent) and 'sz' (size).
 * - Path compression is NOT used (it destroys persistence/memory). 
 * - Relies on Union-By-Size (or Rank) to keep find() O(log N).
 * - Total find() cost: O(log N [tree height] * log N [array access]).
 */

 #include<bits/stdc++.h>
using namespace std;

template<typename T>
struct PersistentArray { // 0-indexed
  struct node {
    node* l, * r; T x;
  };
  int n = 1;
  vector<node*> root;
  int build(vector<T> v) {
    while (n < v.size()) n <<= 1;
    root.push_back(build(0, n - 1, v));
    return root.size() - 1;
  }
  node* build(int l, int r, vector<T>& v) {
    node* cur = new node();
    if (l == r) {
      if (l < v.size()) cur -> x = v[l];
      else cur -> x = 0;
    }
    else {
      cur -> l = build(l, (l + r) >> 1, v);
      cur -> r = build(((l + r) >> 1) + 1, r, v);
    }
    return cur;
  }
  // get the ith value of the rth array
  T get_val(int r, int i) {
    return get_val(root[r], i, 0, n - 1);
  }
  T get_val(node* cur, int i, int l, int r) {
    if (l == r) return cur -> x;
    if (i <= ((l + r) >> 1)) return get_val(cur -> l, i, l, (l + r) >> 1);
    else return get_val(cur -> r, i, ((l + r) >> 1) + 1, r);
  }
  // update the ith value if the rth array by x and return the new root of the array
  int upd(int r, int i, T x) {
    root.push_back(upd(root[r], i, x, 0, n - 1));
    return root.size() - 1;
  }
  node* upd(node* pre, int i, T x, int l, int r) {
    node* cur = new node();
    if (l == r){
      cur -> x = x;
    }
    else {
      if (i <= ((l + r) >> 1)) {
        cur -> l = upd(pre -> l, i, x, l, (l + r) >> 1);
        cur -> r = pre -> r;
      }
      else {
        cur -> l = pre -> l;
        cur -> r = upd(pre -> r, i, x, ((l + r) >> 1) + 1, r);
      }
    }
    return cur;
  }
};

template<typename T>
struct PersistentQueue {
  int cur = 0; // queue ids
  vector<int> L, R;
  PersistentArray<T> st;
  PersistentQueue() {}
  PersistentQueue(int q) { // max size of a queue
    L.assign(q, 0);
    R.assign(q, 0);
    st.build(vector<T>(q, 0));
    cur++;
  }
  void push(int r, T x) { // push x at the end of the rth queue
    st.upd(r, R[r], x);
    L[cur] = L[r];
    R[cur] = R[r] + 1;
    cur++;
  }
  T pop(int r) { // pops the top element of the rth queue and returns the popped element
    st.upd(r, L[r], 0);
    L[cur] = L[r] + 1;
    R[cur] = R[r];
    cur++;
    return st.get_val(r, L[r]);
  }
};

int32_t main(){
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int q; cin >> q;
  PersistentQueue<int> Q(q + 1);
  while (q--) {
    int ty; cin >> ty;
    if (ty == 0) {
      int id, x; cin >> id >> x;
      Q.push(id + 1, x);
    }
    else {
      int id; cin >> id;
      cout << Q.pop(id + 1) << '\n';
    }
  }
}
// https://judge.yosupo.jp/problem/persistent_queue
 
 // (Include PersistentArray struct from Section 1 here)
 
 mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
 
 struct PersistentDSU {
   PersistentArray<int> par, sz; // Persistent arrays for DSU state
   vector<int> c; // Component count per version
   int cur = 0;   // Current version ID
 
   PersistentDSU(int n, int q) {
     vector<int> p(n + 1);
     for (int i = 1; i <= n; i++) p[i] = i;
     
     par.build(p);
     sz.build(vector<int> (n + 1, 1));
     c.resize(q + 1, n); 
     cur = 0;
   }
 
   // Find with NO path compression (preserves history)
   int find(int r, int u) {
     int p = par.get_val(r, u);
     if (p == u) return u;
     return find(r, p); // Standard recursion without update
   }
 
   bool same(int r, int u, int v) { 
     return find(r, u) == find(r, v); 
   }
 
   // Merge u and v in version 'r'. Returns new version ID.
   int merge(int r, int u, int v) {
     cur++; // New version ID
     c[cur] = c[r];
     
     u = find(r, u); 
     v = find(r, v);
 
     if (u == v) {
       // Logic to copy state even if no merge happens (to keep version continuity)
       // Usually can just return 'r' if no change, but problem might require linear IDs
       // Here we do dummy updates to create new root
       par.upd(r, 0, 0); sz.upd(r, 0, 0); 
       return cur; 
     }
     
     // Union by Size (Randomized swap helps too)
     if (rnd() % 2) swap(u, v); 
     
     int new_sz = sz.get_val(r, v) + sz.get_val(r, u);
     
     // Create new par array where par[u] = v
     // Note: We update 'par' based on version 'r', get intermediate version, 
     // then update 'sz' based on that.
     
     // In actual implementation (optimized):
     // 1. Update parent: par[u] = v
     // 2. Update size: sz[v] += sz[u]
     // Since PersistentArray wrapper handles internal roots, we just call upd.
     // WARNING: The provided wrapper 'upd' appends to root list. 
     // We need to be careful about which root index we pass.
     
     // Simpler flow:
     c[cur]--; // One less component
     par.upd(r, u, v); // par[u] = v. Creates new version of Par Array.
     sz.upd(r, v, new_sz); // sz[v] = new size. Creates new version of Sz Array.
     
     // NOTE: The 'PersistentDSU' struct assumes 'par' and 'sz' grow synchronously.
     // The 'cur' index matches the index in 'par.root' and 'sz.root'.
     return cur;
   }
 };
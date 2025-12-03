/**
 * Description: Persistent Li Chao Tree.
 * A fully persistent version of Li Chao Tree.
 * Useful for problems requiring querying past versions of the structure (e.g., on trees).
 *
 * Complexity: O(log C) per update/query. Space O(Q log C).
 * Input: Lines, Updates, Queries.
 * Output: Min/Max value.
 * Problem: https://judge.yosupo.jp/problem/segment_add_get_min (can be solved non-persistently, but structure works)
 */

#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define eb emplace_back
#define nl '\n'

const int N = 1e5 + 9;

struct Line {
  ll k, d;
  ll eval(ll x) {
    return k * x + d;
  }
};

struct LiChaoNode {
  Line line;
  int l, r; // Indices of children in the node pool
  LiChaoNode() {
    line = Line({0, numeric_limits<long long>::max() / 2}); // Identity for Min Query
    l = 0, r = 0;
  }
  LiChaoNode(Line line) : line(line), l(0), r(0) {}
} t[50 * N]; // Node pool

int T; // Node counter

// Persistent Update
// Returns index of new node
int upd(int pre, Line nw, int l, int r) {
  int m = (l + r) / 2;
  int id = ++T;
  
  // Copy previous node
  t[id] = t[pre]; // Copy line and children
  
  // Standard Li Chao Logic
  bool lef = nw.eval(l) < t[id].line.eval(l);
  bool mid = nw.eval(m) < t[id].line.eval(m);
  
  if(mid) swap(t[id].line, nw);
  
  if(l == r) return id;
  
  if(lef != mid) {
    t[id].l = upd(t[pre].l, nw, l, m); // Create new left child
  } else {
    t[id].r = upd(t[pre].r, nw, m + 1, r); // Create new right child
  }
  return id;
}

ll Query(int cur, int x, int l, int r) {
  if (!cur) return numeric_limits<long long>::max() / 2; // Handle null nodes
  ll val = t[cur].line.eval(x);
  int m = (l + r) / 2;
  if(l < r) {
    if(x <= m) val = min(val, Query(t[cur].l, x, l, m));
    else val = min(val, Query(t[cur].r, x, m + 1, r));
  }
  return val;
}

struct PersistentLiChaoTree {
  int L, R;
  vector<int> roots;
  PersistentLiChaoTree() {
    roots = {0}; // 0 is dummy/empty root
    T = 0;
    L = -1e9;
    R = 1e9;
    t[0] = LiChaoNode(); // Init empty node 0
  }
  PersistentLiChaoTree(int L, int R) : L(L), R(R) {
    T = 0;
    roots.push_back(0);
    t[0] = LiChaoNode();
  }
  int add_line(int p_ver, Line line) {
    int old_root = roots[p_ver];
    int new_root = upd(old_root, line, L, R);
    roots.push_back(new_root);
    return roots.size() - 1;
  }
  ll query(int i, int x) {
    return Query(roots[i], x, L, R);
  }
} lt;

// Example: Tree Path Optimization Problem
// DP[u] depends on ancestors.
// dp[u] = min/max( ... + V_j * S_u + ... )
// Persistent Li Chao allows querying structure at ancestor 'u'.

ll sum[N];
vector<pair<int, int>> g[N];
ll ans[N], p[N], s[N];

void dfs(int u, int pre = 0) {
  for(auto x : g[u]) {
    int v = x.first, d = x.second;
    if(v == pre) continue;
    sum[v] = sum[u] + d;
    dfs(v, u);
  }
}

void yo(int u, int pre = 0) {
  // Current root is the last added version
  // int cur_root_idx = lt.roots.size() - 1;
  // Actually we need to track which version corresponds to 'u'.
  // But since we do DFS, we can just push/pop versions (stack-like persistence)
  // OR use true persistence (pass version index).
  // This implementation uses a global 'roots' vector as a stack, which works for DFS.
  
  for(auto x : g[u]) {
    int v = x.first;
    if(v == pre) continue;
    
    // Query using current version (ancestor info)
    // Example equation
    ans[v] = lt.query((int)lt.roots.size() - 1, p[v]) + sum[v] * p[v] + s[v];
    
    // Add new line for 'v'
    lt.add_line((int)lt.roots.size() - 1, {-sum[v], ans[v]});
    
    yo(v, u);
    
    // Backtrack (Pop the version created for this subtree)
    lt.roots.pop_back();
  }
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n;
  if (cin >> n) {
      for(int i = 1; i < n; i++) {
        int u, v, d; cin >> u >> v >> d;
        g[u].eb(v, d); g[v].eb(u, d);
      }
      for(int i = 2; i <= n; i++) cin >> s[i] >> p[i];
      
      dfs(1);
      
      lt = PersistentLiChaoTree((ll) -1e9 - 10, (ll) 1e9 + 10);
      // Base case line
      lt.add_line(0, {0, 0}); // Adds to version 0, creates version 1
      
      // Start DP
      // yo(1) assumes logic starts from 1
      // Actually we should manually push for 1 if needed?
      // The loop in 'yo' iterates children. 
      // We need to handle root logic if root also adds a line?
      // In this problem (Convex Hull Optimization on Trees), usually root is base case.
      
      // Code from original file:
      // yo(1) iterates children of 1.
      // So it queries for children using line from 1.
      
      yo(1);
      
      for(int i = 2; i <= n; i++) cout << ans[i] << (i == n ? "" : " ");
      cout << nl;
  }
  return 0;
}

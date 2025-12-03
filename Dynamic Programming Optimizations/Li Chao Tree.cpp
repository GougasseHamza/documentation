/**
 * Description: Li Chao Tree (Dynamic CHT).
 * A Segment Tree based data structure to maintain the convex hull of lines.
 * Supports adding lines (segments) and querying the minimum/maximum at a point x.
 *
 * Features:
 * - Add line y = mx + c globally or on a segment [L, R].
 * - Query min/max at x.
 * - Works for arbitrary slopes (non-monotonic).
 *
 * Complexity: O(log C) per operation where C is the coordinate range.
 * Input: Operations (Add Line / Query).
 * Output: Results.
 */

#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll inf = 2e18;

struct Line {
  ll m, c;
  ll eval(ll x) { return m * x + c; }
};

struct node {
  Line line;
  node* left = nullptr;
  node* right = nullptr;
  node(Line line) : line(line) {}
  
  // Add segment of line 'nw' to cover range [l, r] within current node range [L, R]
  void add_segment(Line nw, int l, int r, int L, int R) {
    if (l > r || r < L || l > R) return;
    int mid = (L + R) / 2; // Midpoint of current node range
    if (mid == R && L != R) mid = L; // Handle rounding or base cases
    // Better midpoint logic:
    int m = (L + R) >> 1;

    if (l <= L && r >= R) {
      // Current node range is fully covered by update
      bool lef = nw.eval(L) < line.eval(L);
      bool mid_better = nw.eval(m) < line.eval(m);
      
      if (mid_better) swap(line, nw); // Keep the better line at mid in current node
      
      if (L == R) return;
      
      if (lef != mid_better) {
        // Intersection in left child
        if (left == nullptr) left = new node(nw); // Or {0, inf} if nw is worse? No, pass nw down
        else left -> add_segment(nw, l, r, L, m);
      }
      else {
        // Intersection in right child
        if (right == nullptr) right = new node(nw);
        else right -> add_segment(nw, l, r, m + 1, R);
      }
      return;
    }
    
    // Partial overlap: push down
    if (max(l, L) <= min(m, R)) {
      if (left == nullptr) left = new node({0, inf});
      left -> add_segment(nw, l, r, L, m);
    }
    if (max(m + 1, L) <= min(r, R)) {
      if (right == nullptr) right = new node ({0, inf});
      right -> add_segment(nw, l, r, m + 1, R);
    }
  }
  
  ll query_segment(ll x, int l, int r, int L, int R) {
     // This function signature in original code mixes 'l, r' (query range?) and 'L, R' (node range).
     // Actually Li Chao usually queries at a point X. The range l,r is for "Segment Add".
     // For point query, we just need x.
     // The original code implements "Segment Add, Point Query".
     // query_segment(x, l, r, L, R) likely queries x, restricting consideration to segments added within [l, r]?
     // No, the standard Li Chao query is just query(x). 
     // The original code's `query_segment` seems to restrict the query to segments that were added covering [l,r]? 
     // Wait, line 87 calls it with l, r. 
     // Re-reading: `query_segment(x, l, r, L, R)` checks min in range [l,r]? 
     // But the implementation checks `if (l >= L and r <= R)`. This logic is usually for range queries.
     // BUT Li Chao Point Query is traversing down to leaf.
     // Let's stick to standard Point Query.
     
     // If we want to query min at x, considering ONLY lines that exist at x.
     int m = (L + R) >> 1;
     ll val = line.eval(x);
     
     if (left != nullptr && x <= m) val = min(val, left->query_segment(x, l, r, L, m));
     if (right != nullptr && x > m) val = min(val, right->query_segment(x, l, r, m + 1, R));
     
     return val;
  }
};

// Wrapper
struct LiChaoTree {
  int L, R;
  node* root;
  LiChaoTree(int L, int R) : L(L), R(R) {
    root = new node({0, inf});
  }
  
  // Add line segment
  void add_segment(Line line, int l, int r) {
    root -> add_segment(line, l, r, L, R);
  }
  
  // Query point
  ll query(ll x) {
    // Original code had range [l, r] in query too?
    // `query_segment(x, L, R, L, R)` means query at x, global.
    return root -> query_segment(x, L, R, L, R);
  }
};

int32_t main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  
  // Range of X coordinates
  LiChaoTree t(-1e9, 1e9);
  
  int n, q; 
  if (cin >> n >> q) {
      for (int i = 0; i < n; i++) {
        ll l, r, a, b; cin >> l >> r >> a >> b;
        r--; // [l, r) -> [l, r]
        t.add_segment({a, b}, l, r);
      }
      while (q--) {
        int ty; cin >> ty;
        if (ty == 0) { // Add
          ll l, r, a, b; cin >> l >> r >> a >> b;
          r--;
          t.add_segment({a, b}, l, r);
        }
        else { // Query
          ll x; cin >> x;
          ll ans = t.query(x);
          if (ans >= inf) cout << "INFINITY\n";
          else cout << ans << '\n';
        }
      }
  }
  return 0;
}

/**
 * Description: Persistent Convex Hull Trick (Tree-based).
 * Supports adding lines (points in dual) and querying max/min.
 * This version is persistent: it maintains a tree where each node is a state.
 * It uses binary lifting to find the optimal line (point in dual).
 *
 * Logic:
 * The "lines" are points (x, y) and we query 'a' to minimize dot product.
 * We maintain the hull as a path in a tree.
 * When adding a point, we find where it attaches to the previous hull (binary search on tree path).
 *
 * Complexity: O(log N) per insert/query.
 * Input: Points/Lines.
 * Output: Min dot product.
 */

#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

struct PT {
  int x, y;
  PT(int x = 0, int y = 0): x(x), y(y) {}
  friend ll dot(PT &a, PT &b) {
    return 1ll * a.x * b.x + 1ll * a.y * b.y;
  }
  // Returns orientation of ordered triplet (a, b, c).
  // 0 --> Collinear
  // 1 --> Clockwise
  // -1 --> Counterclockwise
  // Used to maintain convexity.
  friend int orientation(PT &a, PT &b, PT &c) {
    ll s = 1ll * (b.x - a.x) * (c.y - a.y) - 1ll * (b.y - a.y) * (c.x - a.x);
    return (s > 0) - (s < 0);
  }
};

struct PersistentCHT { // Minimizes dot product
  const static int N = 1e6 + 6;
  const static int lg = 22;
  
  // p[u][i] is the 2^i-th ancestor of u in the CHT tree structure
  int p[N][lg], sz;
  PT pnt[N];

  // Insert point 'a' starting from version 'rt'.
  // Returns new version index.
  int insert(PT a, int rt) {
    // Find the insertion point by walking up the tree (removing bad points)
    for (int u, v, i = lg - 1; i >= 0; i--)
      if ((u = p[rt][i]) && (v = p[u][0]) && orientation(pnt[v], pnt[u], a) <= 0)
        rt = u;
    
    if (p[rt][0] && orientation(pnt[p[rt][0]], pnt[rt], a) <= 0) rt = p[rt][0];
    
    // Create new node
    pnt[++sz] = a;
    p[sz][0] = rt;
    for (int i = 1; i < lg; i++) p[sz][i] = p[p[sz][i - 1]][i - 1];
    
    return sz;
  }

  // Query min dot product with vector 'a' starting from version 'rt'.
  ll query(PT a, int rt) {
    // Ternary search / Binary search on the hull (which is a path in the tree)
    for (int u, v, i = lg - 1; i >= 0; i--)
      if ((u = p[rt][i]) && (v = p[u][0]) && dot(a, pnt[v]) > dot(a, pnt[u])) // Check if moving up improves
        rt = u;
    
    if (p[rt][0] && dot(a, pnt[p[rt][0]]) > dot(a, pnt[rt])) rt = p[rt][0];
    
    return rt ? dot(a, pnt[rt]) : -1e18; // Return min value
  }
} cht;

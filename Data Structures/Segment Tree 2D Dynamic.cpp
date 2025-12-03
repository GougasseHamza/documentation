/**
 * Dynamic 2D Segment Tree (GCD)
 * * * PROBLEMS SOLVED:
 * 1. 2D Range Queries (GCD, Sum, Max) on a huge sparse grid (e.g., 10^9 x 10^9).
 * 2. Point Update: Set A[x][y] = val.
 * * * COMPLEXITY:
 * Time: O(log N * log M) per operation.
 * Space: O(Q * log N). Each update affects O(log N) X-nodes, adding 1 node to the Treap at each level.
 * * * LOGIC:
 * - Outer Layer (ST): Dynamic Segment Tree partitioning the X-axis [0, N].
 * - Inner Layer (treap): For every node in the X-SegTree (which represents a range of rows),
 * we maintain a Treap. This Treap stores the aggregated GCD of that specific row range 
 * for active Y-columns.
 * - Query: Standard 2D SegTree logic. If X-range matches, query the Y-Treap.
 * - Update: Update the leaf (b==e), then walk up. For internal nodes, recompute the value 
 * at Y by taking GCD(left_child_val_at_Y, right_child_val_at_Y).
 * * * TWEAKS:
 * - To change operation to SUM:
 * 1. Change `__gcd` to `+` in `node::pull` and `ST::query` and `ST::fix`.
 * 2. Change return 0 to return 0.
 * - To change operation to MAX:
 * 1. Change `__gcd` to `max`.
 * 2. Change return 0 to return -INF.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 // Random engine for Treap priorities
 mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
 
 const int N = 3e5 + 9;
 
 // --- INNER TREAP NODE ---
 struct node {
   node *l, *r;
   int pos, key; // pos = Y-coordinate, key = Random priority
   int mn, mx;   // Min/Max Y-coordinates in this subtree (for range check optimization)
   long long val; // Actual value at A[x][pos] (where x is determined by parent SegTree node)
   long long g;   // Aggregate GCD of this subtree
   
   node(int position, long long value) {
     l = r = nullptr;
     mn = mx = pos = position;
     key = rnd();
     val = g = value;
   }
   
   // Recalculate aggregates from children
   void pull() {
     g = val;
     if(l) g = __gcd(g, l->g); // TWEAK: Change to + for Sum, max for Max
     if(r) g = __gcd(g, r->g); // TWEAK: Change to + for Sum, max for Max
     
     mn = (l ? l->mn : pos);
     mx = (r ? r->mx : pos);
   }
 };
 
 // --- INNER TREAP STRUCTURE ---
 // Acts as a sparse array for the Y-dimension
 struct treap {
   node *root;
   treap() { root = nullptr; }
 
   // Standard split by Y-coordinate 'pos'
   void split(node *t, int pos, node *&l, node *&r) {
     if (t == nullptr) { l = r = nullptr; return; }
     if (t->pos < pos) {
       split(t->r, pos, l, r);
       t->r = l; l = t;
     } else {
       split(t->l, pos, l, r);
       t->l = r; r = t;
     }
     t->pull();
   }
 
   // Standard merge based on priority
   node* merge(node *l, node *r) {
     if (!l || !r) return l ? l : r;
     if (l->key < r->key) {
       l->r = merge(l->r, r);
       l->pull();
       return l;
     }
     r->l = merge(l, r->l);
     r->pull();
     return r;
   }
 
   // Check if a Y-coordinate exists in this treap
   bool find(int pos) {
     node *t = root;
     while (t) {
       if (t->pos == pos) return true;
       if (t->pos > pos) t = t->l;
       else t = t->r;
     }
     return false;
   }
 
   // Point Update helper: Finds node with 'pos' and updates val
   void upd(node *t, int pos, long long val) {
     if (t->pos == pos) {
       t->val = val;
       t->pull();
       return;
     }
     if (t->pos > pos) upd(t->l, pos, val);
     else upd(t->r, pos, val);
     t->pull();
   }
 
   // Insert or Update value at Y = pos
   void insert(int pos, long long val) { 
     if (find(pos)) upd(root, pos, val);
     else {
       node *l, *r;
       split(root, pos, l, r);
       root = merge(merge(l, new node(pos, val)), r);
     }
   }
 
   // Range Query on Treap [st, en]
   long long query(node *t, int st, int en) {
     // Optimization: If subtree range is disjoint from query, return Identity
     if (!t || t->mx < st || en < t->mn) return 0; // TWEAK: 0 for GCD/Sum, -INF for Max
     
     // Optimization: If subtree fully inside query, return aggregate
     if (st <= t->mn && t->mx <= en) return t->g;
     
     // Standard recursion
     long long ans = (st <= t->pos && t->pos <= en ? t->val : 0); // TWEAK
     if (t->l) ans = __gcd(ans, query(t->l, st, en)); // TWEAK
     if (t->r) ans = __gcd(ans, query(t->r, st, en)); // TWEAK
     return ans;
   }
 
   long long query(int l, int r) { 
     if (!root) return 0; // TWEAK
     return query(root, l, r);
   }
 };
 
 // --- OUTER SEGMENT TREE ---
 // Dynamic Segment Tree on X-axis
 struct ST {
   ST *l, *r;
   treap t;   // Stores aggregated values for Y-range at this X-node
   int b, e;  // X-range [b, e]
 
   ST() { l = r = nullptr; }
   ST(int st, int en) {
     l = r = nullptr;
     b = st, e = en;
   }
 
   // Recalculate value for Y-coordinate 'pos' based on children X-nodes
   // This effectively merges the Y-columns from the left X-child and right X-child
   void fix(int pos) {
     long long val = 0; // TWEAK: Identity element (0 for GCD/Sum)
     if (l) val = __gcd(val, l->t.query(pos, pos)); // TWEAK: op(val, left_val)
     if (r) val = __gcd(val, r->t.query(pos, pos));
     t.insert(pos, val);
   }
 
   // 2D Point Update: A[x][y] = val
   void upd(int x, int y, long long val) { 
     if (e < x || x < b) return;
     
     // Leaf X-node: Directly update the Treap
     if (b == e) {
       t.insert(y, val);
       return;
     }
     
     // Internal X-node: Recurse and then fix
     int mid = b + (e - b) / 2; // Safe mid calculation
     if (x <= mid) {
       if (!l) l = new ST(b, mid);
       l->upd(x, y, val);
     } else {
       if (!r) r = new ST(mid + 1, e);
       r->upd(x, y, val);
     }
     fix(y); // Re-aggregate GCD for this specific Y coordinate from children
   }
 
   // 2D Range Query: X in [i, j], Y in [st, en]
   long long query(int i, int j, int st, int en) { 
     if (e < i || j < b) return 0; // TWEAK
     
     // If X-range fully covered, query the inner Treap for Y-range
     if (i <= b && e <= j) return t.query(st, en);
     
     long long ans = 0; // TWEAK
     if (l) ans = __gcd(ans, l->query(i, j, st, en)); // TWEAK
     if (r) ans = __gcd(ans, r->query(i, j, st, en)); // TWEAK
     return ans;
   }
 };
 
 int32_t main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   int n, m;
   if (cin >> n >> m) {
       // Initialize Dynamic SegTree covering X range [0, n-1]
       ST t(0, n - 1);
       
       int q; cin >> q;
       while(q--) {
         int ty; cin >> ty;
         if(ty == 1) { // Update: 1 X Y Val
           int x, y; long long v;
           cin >> x >> y >> v;
           t.upd(x, y, v);
         } else { // Query: 2 X1 Y1 X2 Y2
           int x1, y1, x2, y2;
           cin >> x1 >> y1 >> x2 >> y2;
           cout << t.query(x1, x2, y1, y2) << '\n';
         }
       }
   }
   return 0;
 }
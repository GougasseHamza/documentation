/**
 * Normal (Static) 2D Segment Tree
 *
 * * PROBLEMS SOLVED:
 * 1. 2D Range Queries (Sum, Min, Max, GCD) on small, dense grids (N, M <= 2000).
 * 2. Point Update: A[x][y] = v.
 *
 * * COMPLEXITY:
 * Time: O(log N * log M) per update/query.
 * Space: O(16 * N * M). High memory usage!
 *
 * * TWEAKS:
 * - Change 'merge' function to support Max, Min, GCD, or Sum.
 * - Current implementation: MAX.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1024; // Max Grid Size (Must be small!)
 const int INF = 1e9;
 
 int a[N][N]; // Input grid
 
 struct SegTree2D {
     // T[x_node][y_node]
     // We flatten the 4*N size for X and 4*M size for Y into a 2D array.
     int t[4 * N][4 * N];
     int n, m;
 
     // Operation: Change this for Min/Sum/GCD
     int merge(int a, int b) {
         return max(a, b); 
     }
 
     // --- Y-AXIS FUNCTIONS ---
     
     // Build the Inner Y-tree for a specific X-node (vx)
     // lx, rx: Range of rows covered by vx
     // ly, ry: Range of columns covered by vy
     void build_y(int vx, int lx, int rx, int vy, int ly, int ry) {
         if (ly == ry) {
             // Base case for Y:
             // If X is also a leaf (lx == rx), take value from array.
             // If X is internal, combine values from X-children.
             if (lx == rx) 
                 t[vx][vy] = a[lx][ly];
             else 
                 t[vx][vy] = merge(t[2 * vx][vy], t[2 * vx + 1][vy]);
         } else {
             int my = (ly + ry) / 2;
             build_y(vx, lx, rx, 2 * vy, ly, my);
             build_y(vx, lx, rx, 2 * vy + 1, my + 1, ry);
             t[vx][vy] = merge(t[vx][2 * vy], t[vx][2 * vy + 1]);
         }
     }
 
     // Update Y-tree at X-node (vx)
     void update_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int new_val) {
         if (ly == ry) {
             if (lx == rx) 
                 t[vx][vy] = new_val; // Leaf X, Leaf Y -> Base update
             else 
                 t[vx][vy] = merge(t[2 * vx][vy], t[2 * vx + 1][vy]); // Internal X -> Recombine X-children
         } else {
             int my = (ly + ry) / 2;
             if (y <= my) update_y(vx, lx, rx, 2 * vy, ly, my, x, y, new_val);
             else update_y(vx, lx, rx, 2 * vy + 1, my + 1, ry, x, y, new_val);
             t[vx][vy] = merge(t[vx][2 * vy], t[vx][2 * vy + 1]);
         }
     }
 
     // Query Y-tree
     int query_y(int vx, int vy, int ly, int ry, int qly, int qry) {
         if (qly > ry || qry < ly) return -INF; // TWEAK: Identity (0 for sum, INF for min)
         if (qly <= ly && ry <= qry) return t[vx][vy];
         int my = (ly + ry) / 2;
         return merge(query_y(vx, 2 * vy, ly, my, qly, qry),
                      query_y(vx, 2 * vy + 1, my + 1, ry, qly, qry));
     }
 
     // --- X-AXIS FUNCTIONS ---
 
     void build_x(int vx, int lx, int rx) {
         if (lx != rx) {
             int mx = (lx + rx) / 2;
             build_x(2 * vx, lx, mx);
             build_x(2 * vx + 1, mx + 1, rx);
         }
         // Whether leaf or internal, we build the Y-tree for this node
         build_y(vx, lx, rx, 1, 1, m);
     }
 
     void update_x(int vx, int lx, int rx, int x, int y, int new_val) {
         if (lx != rx) {
             int mx = (lx + rx) / 2;
             if (x <= mx) update_x(2 * vx, lx, mx, x, y, new_val);
             else update_x(2 * vx + 1, mx + 1, rx, x, y, new_val);
         }
         // Update the Y-tree at this X-node
         update_y(vx, lx, rx, 1, 1, m, x, y, new_val);
     }
 
     int query_x(int vx, int lx, int rx, int qlx, int qrx, int qly, int qry) {
         if (qlx > rx || qrx < lx) return -INF; // TWEAK: Identity
         if (qlx <= lx && rx <= qrx) {
             return query_y(vx, 1, 1, m, qly, qry);
         }
         int mx = (lx + rx) / 2;
         return merge(query_x(2 * vx, lx, mx, qlx, qrx, qly, qry),
                      query_x(2 * vx + 1, mx + 1, rx, qlx, qrx, qly, qry));
     }
 
     // --- API ---
     void init(int _n, int _m) {
         n = _n; m = _m;
         build_x(1, 1, n);
     }
     void update(int x, int y, int val) {
         update_x(1, 1, n, x, y, val);
     }
     int query(int x1, int y1, int x2, int y2) {
         return query_x(1, 1, n, x1, x2, y1, y2);
     }
 } st;
 
 int main() {
     int n, m;
     cin >> n >> m;
     for(int i=1; i<=n; ++i) 
         for(int j=1; j<=m; ++j) 
             cin >> a[i][j];
             
     st.init(n, m);
     
     // Update (2, 3) to 10
     st.update(2, 3, 10);
     
     // Query Max in rectangle (1, 1) to (3, 3)
     cout << st.query(1, 1, 3, 3) << endl;
 }
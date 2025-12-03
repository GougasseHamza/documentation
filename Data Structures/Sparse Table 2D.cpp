/**
 * 2D Sparse Table
 * * PROBLEM: 2D Range RMQ on static grid.
 * * LOGIC:
 * - st[i][j][a][b] stores result for rectangle starting at (i, j)
 * with height 2^a and width 2^b.
 * - Query combines 4 overlapping rectangles.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 505, LG = 10;
 
 int st[N][N][LG][LG];
 int a[N][N], lg2[N];
 
 // Query Rectangle (x1, y1) top-left to (x2, y2) bottom-right
 int query(int x1, int y1, int x2, int y2) {
   x2++; y2++; // Convert to exclusive check if needed, or adjust 
   // Note: Input usually inclusive. Code uses length.
   // Let's assume input is inclusive 0-based.
   // length_x = x2 - x1 + 1.
   // Adjusting code to standard inclusive logic:
   int len_x = x2 - x1 + 1;
   int len_y = y2 - y1 + 1;
   int a = lg2[len_x];
   int b = lg2[len_y];
   
   // Max of 4 sub-rectangles
   return max({
     st[x1][y1][a][b],                                       // Top-Left
     st[x2 - (1 << a) + 1][y1][a][b],                        // Bottom-Left
     st[x1][y2 - (1 << b) + 1][a][b],                        // Top-Right
     st[x2 - (1 << a) + 1][y2 - (1 << b) + 1][a][b]          // Bottom-Right
   });
 }
 
 void build(int n, int m) { // 0 indexed
   for (int i = 2; i < N; i++) lg2[i] = lg2[i >> 1] + 1;
   
   // Base Layer (1x1)
   for (int i = 0; i < n; i++) {
     for (int j = 0; j < m; j++) st[i][j][0][0] = a[i][j];
   }
   
   // DP
   for (int a = 0; a < LG; a++) {
     for (int b = 0; b < LG; b++) {
       if (a + b == 0) continue;
       
       for (int i = 0; i + (1 << a) <= n; i++) {
         for (int j = 0; j + (1 << b) <= m; j++) {
           if (!a) { 
             // Merge horizontally (width grows)
             st[i][j][a][b] = max(st[i][j][a][b - 1], st[i][j + (1 << (b - 1))][a][b - 1]);
           } else {
             // Merge vertically (height grows)
             // Note: We use [a-1][b] which are already computed (width 2^b is fixed)
             st[i][j][a][b] = max(st[i][j][a - 1][b], st[i + (1 << (a - 1))][j][a - 1][b]);
           }
         }
       }
     }
   }
 }
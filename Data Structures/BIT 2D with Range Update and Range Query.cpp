/**
 * 2D BIT with Range Update and Range Query
 * * Description:
 * Supports adding value to a sub-rectangle (x1, y1, x2, y2) and querying sum of sub-rectangle.
 * Uses Inclusion-Exclusion with 4 auxiliary BITs.
 * * Complexity:
 * Time: O(log N * log M) per operation.
 * Space: O(N * M) * 4 * 8 bytes. (~32MB for N=1000).
 * * WARNING:
 * - Space consumption is HIGH. Do not use for N > 2048 without coordinate compression.
 * - Coordinate compression is required if coordinates are large (10^9).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1010; // Max Grid Size
 
 struct BIT2D {
   long long M[N][N][2], A[N][N][2];
   
   BIT2D() {
     memset(M, 0, sizeof M);
     memset(A, 0, sizeof A);
   }
 
   void upd2(long long t[N][N][2], int x, int y, long long mul, long long add) {
     for(int i = x; i < N; i += i & -i) {
       for(int j = y; j < N; j += j & -j) {
         t[i][j][0] += mul;
         t[i][j][1] += add;
       }
     }
   }
 
   void upd1(int x, int y1, int y2, long long mul, long long add) {
     upd2(M, x, y1, mul, -mul * (y1 - 1));
     upd2(M, x, y2, -mul, mul * y2);
     upd2(A, x, y1, add, -add * (y1 - 1));
     upd2(A, x, y2, -add, add * y2);
   }
 
   // RANGE UPDATE: Add val to rectangle [(x1,y1) ... (x2,y2)]
   void upd(int x1, int y1, int x2, int y2, long long val) {
     upd1(x1, y1, y2, val, -val * (x1 - 1));
     upd1(x2, y1, y2, -val, val * x2);
   }
 
   long long query2(long long t[N][N][2], int x, int y) {
     long long mul = 0, add = 0;
     for(int i = y; i > 0; i -= i & -i) {
       mul += t[x][i][0];
       add += t[x][i][1];
     }
     return mul * y + add;
   }
 
   long long query1(int x, int y) {
     long long mul = 0, add = 0;
     for(int i = x; i > 0; i -= i & -i) {
       mul += query2(M, i, y);
       add += query2(A, i, y);
     }
     return mul * x + add;
   }
 
   // RANGE QUERY: Sum of rectangle [(x1,y1) ... (x2,y2)]
   long long query(int x1, int y1, int x2, int y2) {
     return query1(x2, y2) - query1(x1 - 1, y2) - query1(x2, y1 - 1) + query1(x1 - 1, y1 - 1);
   }
 } t;
 
 int main() {
   // Usage Example
   int n, m;
   cin >> n >> m;
   // Initialize grid
   for(int i = 1; i <= n; i++) {
     for(int j = 1; j <= m; j++) {
       int k; cin >> k;
       t.upd(i, j, i, j, k);
     }
   }
   
   // Update Region
   t.upd(1, 1, 2, 2, 5); // Add 5 to top-left 2x2 square
   
   // Query Region
   cout << t.query(1, 1, n, m) << '\n';
   return 0;
 }
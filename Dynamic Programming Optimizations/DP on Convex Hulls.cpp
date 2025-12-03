/**
 * Description: DP on Convex Hulls (Angular Sweep).
 * Solves problems where we need to find an optimal convex polygon using a subset of points.
 *
 * Logic:
 * 1. Iterate through all pairs of points (i, j) to form edges.
 * 2. Sort edges/pairs based on angle to perform an angular sweep.
 * 3. Maintain DP states: dp[l][r] = max/best value ending at edge (l->r).
 * 4. The angular sort ensures we build the polygon in convex order.
 *
 * Complexity: O(N^3) generally, or O(N^2 log N) for sorting.
 * Input: Points P[1..N].
 * Output: Max area/perimeter/score.
 * Problem: https://toph.co/p/profitable-home
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 205;

int sign(int x) { return (x > 0) - (x < -0); }

struct PT {
  int x, y;
  PT() { x = 0, y = 0; }
  PT(int x, int y) : x(x), y(y) {}
  PT operator + (const PT &a) const { return PT(x + a.x, y + a.y); }
  PT operator - (const PT &a) const { return PT(x - a.x, y - a.y); }
  int norm2() { return x * x + y * y; }
};

inline int cross(PT a, PT b) { return a.x * b.y - a.y * b.x; }

int n;
PT p[N];

// Helper for angular sort
bool half(PT p) {
  return p.y > 0 || (p.y == 0 && p.x < 0);
}

double dp[N][N]; // dp[first_vertex][last_vertex]
vector<pair<int, int>> v;

// Example Check Function for Binary Search
bool yo(double x) {
  for (int l = 1; l <= n; l++) {
    for (int r = 1; r <= n; r++) {
      dp[l][r] = -1e9;
    }
  }
  
  // Process edges in angular order
  for (auto [mid, r]: v) {
    // Calculate gain of adding triangle (origin/first, mid, r) or edge (mid, r)
    // Here logic seems to be: Area - cost * x
    double cur = cross(p[mid], p[r]) * 0.5 - (p[r] - p[mid]).norm2() * x;
    
    for (int l = 1; l <= n; l++) {
      // Extend polygon ending at 'mid' to 'r'
      // Transition: dp[l][r] = dp[l][mid] + contribution
      // If l == mid, it's a starting edge.
      dp[l][r] = max(dp[l][r], (l == mid ? 0 : dp[l][mid]) + cur);
    }
  }
  
  // Check if any closed polygon (l -> ... -> l) is non-negative
  for (int i = 1; i <= n; i++) {
    if (dp[i][i] >= 0) return true;
  }
  return false;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; 
  if (cin >> t) {
      while (t--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
          cin >> p[i].x >> p[i].y;
        }
        
        v.clear();
        for (int i = 1; i <= n; i++) {
          for (int j = 1; j <= n; j++) {
            if (i != j) v.push_back({i, j});
          }
        }
        
        // Angular sort of all vectors (p[j] - p[i])
        sort(v.begin(), v.end(), [&](pair<int, int> x, pair<int, int> y) {
          PT a = p[x.second] - p[x.first], b = p[y.second] - p[y.first];
          return make_tuple(half(a), 0, a.norm2()) < make_tuple(half(b), cross(a, b), b.norm2());
        });

        // Binary search on answer (if needed)
        double l = 0, r = 1e7, ans = 0;
        int it = 50; // Iterations for precision
        while (it--) {
          double mid = (l + r) / 2;
          if (yo(mid)) {
            ans = mid;
            l = mid;
          } else {
            r = mid;
          }
        }
        cout << fixed << setprecision(10) << ans << '\n';
      }
  }
  return 0;
}

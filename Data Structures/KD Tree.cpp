/*
 * KD Tree (Nearest Neighbor)
 *
 * Description:
 * Multidimensional binary search tree. Splits space by alternating axes.
 *
 * Tricks & Tweaks:
 * 1. Dimensions: Change `const int d = 2;` for 3D/4D.
 * 2. Metric: Change `squared_distance` to Manhattan distance (sum of abs diffs)
 * if problem requires it.
 * 3. k-Nearest: Maintain a priority queue of size k in search to find k-closest.
 *
 * What Not To Forget:
 * - `nth_element` modifies the array order.
 * - The splitting logic relies on the median.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 const long long INF = 2e18; // Safety
 const int d = 2; /// DIMENSION
 
 struct point {
   int p[d];
   bool operator !=(const point &a) const {
     for(int i = 0; i < d; i++) if(p[i] != a.p[i]) return true;
     return false;
   }
 };
 
 struct kd_node {
   int axis, value;
   point p;
   kd_node *left, *right;
 };
 
 // Comparator for nth_element
 struct cmp_points {
   int axis;
   cmp_points(int x): axis(x) {}
   bool operator () (const point &a, const point &b) const {
     return a.p[axis] < b.p[axis];
   }
 };
 
 point arr[N]; // Input buffer
 kd_node* root;
 
 long long squared_distance(point a, point b) {
   long long ans = 0;
   for(int i = 0; i < d; i++) ans += 1LL * (a.p[i] - b.p[i]) * (a.p[i] - b.p[i]);
   return ans;
 }
 
 void build_tree(kd_node* &node, int from, int to, int axis) {
   if(from > to) { node = NULL; return; }
   node = new kd_node();
   if(from == to) {
     node->p = arr[from]; node->left = NULL; node->right = NULL;
     return;
   }
   int mid = (from + to) / 2;
   // Partitions array such that element at mid is in correct sorted position
   nth_element(arr + from, arr + mid, arr + to + 1, cmp_points(axis));
   
   node->value = arr[mid].p[axis];
   node->axis = axis;
   node->p = arr[mid];
   build_tree(node->left, from, mid - 1, (axis + 1) % d);
   build_tree(node->right, mid + 1, to, (axis + 1) % d);
 }
 
 void nearest_neighbor(kd_node* node, point q, long long &ans) {
   if(node == NULL) return;
   long long dist = squared_distance(node->p, q);
   if(dist > 0) ans = min(ans, dist); // > 0 check avoids self-match if needed
 
   int axis = node->axis;
   long long diff = q.p[axis] - node->value;
   
   // Heuristic: Search the side the point lies on first
   if(diff <= 0) {
     nearest_neighbor(node->left, q, ans);
     // Pruning: Only check other side if the circle intersects the splitting plane
     if(diff * diff < ans) nearest_neighbor(node->right, q, ans);
   } else {
     nearest_neighbor(node->right, q, ans);
     if(diff * diff < ans) nearest_neighbor(node->left, q, ans);
   }
 }

int main() {
  int tests ; point *  pts ; int n ; point *arr ; long long  ans ; 
  int i, j, k, m;
  scanf("%d", &tests);
  while(tests--) {
    scanf("%d", &n);
    for(i = 1; i <= n; i++) {
      for(j = 0; j < d; j++) scanf("%d", &arr[i].p[j]);
      pts[i] = arr[i];
    }

    build_tree(root, 1, n, 0);

    for(i = 1; i <= n; i++) {
      ans = INF;
      nearest_neighbor(root, pts[i], ans);
      printf("%lld\n", ans);
    }
  }
  return 0;
}

/**
 * Wavelet Tree
 * * * PROBLEMS SOLVED:
 * 1. K-th smallest element in range [L, R].
 * 2. Count of numbers <= K in range [L, R].
 * 3. Count of numbers equal to K in range [L, R].
 * 4. Sum of numbers <= K in range [L, R].
 * * * * COMPLEXITY:
 * Time: 
 * - Build: O(N log(Sigma)) where Sigma = max_val - min_val.
 * - Query: O(log(Sigma)).
 * Space: O(N log(Sigma)). Can be optimized using pointers.
 *
 * * * LOGIC:
 * A Wavelet Tree partitions the values based on a range [lo, hi].
 * - Root handles [lo, hi]. 
 * - Left child handles [lo, mid]. Right child handles [mid+1, hi].
 * - 'b[i]' stores how many elements in the prefix i went to the left child.
 * - 'c[i]' stores the prefix sum of elements.
 * * To query range [L, R]:
 * - We calculate how many elements from [L, R] went left ('cnt').
 * - If we need elements <= K and K <= mid, we recurse left with adjusted range.
 * - If K > mid, we take all 'cnt' elements from left (sum/count them) and recurse right.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int MAXN = (int)3e5 + 9;
 const int MAXV = (int)1e9 + 9; 
 
 struct wavelet_tree {
   int lo, hi;
   wavelet_tree *l, *r;
   int *b; // b[i] = count of elements in A[0...i-1] that went to left child
   int *c; // c[i] = prefix sum of original array values reordered at this node
   int bsz, csz; 
 
   wavelet_tree() {
     lo = 1; hi = 0; bsz = 0; csz = 0; l = NULL; r = NULL;
   }
 
   // Init: Builds the tree for range [from, to) with value range [x, y]
   void init(int *from, int *to, int x, int y) {
     lo = x, hi = y;
     if(from >= to) return;
     int mid = lo + (hi - lo) / 2; // Avoid overflow
     
     // Lambda to decide if element goes left
     auto f = [mid](int val) { return val <= mid; };
     
     // Allocate memory for prefix arrays
     b = (int*)malloc((to - from + 2) * sizeof(int));
     c = (int*)malloc((to - from + 2) * sizeof(int));
     bsz = 0; csz = 0;
     
     b[bsz++] = 0;
     c[csz++] = 0;
     
     for(auto it = from; it != to; it++) {
       b[bsz] = b[bsz - 1] + f(*it);
       c[csz] = c[csz - 1] + (*it);
       bsz++; csz++;
     }
     
     if(hi == lo) return; // Leaf node
     
     // Stable partition reorders array: elements <= mid come first
     auto pivot = stable_partition(from, to, f);
     
     l = new wavelet_tree();
     l->init(from, pivot, lo, mid);
     
     r = new wavelet_tree();
     r->init(pivot, to, mid + 1, hi);
   }
 
   // 1. K-th Smallest in [L, R]
   int kth(int L, int R, int k) {
     if(L > R) return 0;
     if(lo == hi) return lo;
     
     // Count how many elements in [L, R] go to left child
     int inLeft = b[R] - b[L - 1];
     int lb = b[L - 1]; // Elements before L that went left
     int rb = b[R];     // Elements before R+1 that went left
     
     if(k <= inLeft) return this->l->kth(lb + 1, rb, k);
     else return this->r->kth(L - lb, R - rb, k - inLeft);
   }
 
   // 2. Count elements <= K in [L, R]
   int LTE(int L, int R, int k) {
     if(L > R || k < lo) return 0;
     if(hi <= k) return R - L + 1;
     
     int lb = b[L - 1], rb = b[R];
     return this->l->LTE(lb + 1, rb, k) + this->r->LTE(L - lb, R - rb, k);
   }
 
   // 3. Count elements == K in [L, R]
   int count(int L, int R, int k) {
     if(L > R || k < lo || k > hi) return 0;
     if(lo == hi) return R - L + 1;
     
     int lb = b[L - 1], rb = b[R];
     int mid = lo + (hi - lo) / 2;
     
     if(k <= mid) return this->l->count(lb + 1, rb, k);
     else return this->r->count(L - lb, R - rb, k);
   }
 
   // 4. Sum of elements <= K in [L, R]
   int sum(int L, int R, int k) {
     if(L > R || k < lo) return 0;
     if(hi <= k) return c[R] - c[L - 1]; // Use stored prefix sums
     
     int lb = b[L - 1], rb = b[R];
     return this->l->sum(lb + 1, rb, k) + this->r->sum(L - lb, R - rb, k);
   }
 
   ~wavelet_tree() { delete l; delete r; free(b); free(c); }
 };
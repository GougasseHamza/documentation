/**
 * BIT with Range Update and Range Query
 * * Description:
 * Maintains prefix sums to support:
 * 1. Add val to all elements in range [l, r].
 * 2. Query sum of elements in range [l, r].
 * It uses two internal BITs to handle the algebraic expansion of arithmetic progressions.
 * * Complexity:
 * Time: O(log N) per update/query.
 * Space: O(N).
 * * Usage:
 * BIT t; // Global instance handles 0 initialization
 * t.upd(l, r, val); // Add val to [l, r]
 * long long sum = t.query(l, r); // Get sum of [l, r]
 * * Tricks & Tweaks:
 * - 1-based indexing is strictly enforced.
 * - The 'kth' function added below finds the smallest index i such that prefix_sum(i) >= k.
 * (Note: kth only works monotonically if values are non-negative).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 
 struct BIT {
   long long M[N], A[N];
   
   // Standard BIT update (internal use)
   void update(int i, long long mul, long long add) {
     while (i < N) {
       M[i] += mul;
       A[i] += add;
       i |= (i + 1);
     }
   }
 
   // RANGE UPDATE: Add x to [l, r]
   void upd(int l, int r, long long x) {
     if(l > r) return;
     update(l, x, -x * (l - 1));
     update(r + 1, -x, x * r);
   }
 
   // PREFIX QUERY: Sum from [1, i]
   long long query(int i) {
     long long mul = 0, add = 0;
     int st = i;
     while (i >= 0) {
       mul += M[i];
       add += A[i];
       i = (i & (i + 1)) - 1;
     }
     return (mul * st + add);
   }
 
   // RANGE QUERY: Sum from [l, r]
   long long query(int l, int r) {
     if(l > r) return 0;
     return query(r) - query(l - 1);
   }
   
   // OPTIONAL TRICK: Binary lifting to find k-th element (if monotonic)
   // Returns smallest index idx such that query(1, idx) >= k
   // NOTE: Only valid if range updates preserve monotonicity (e.g. all updates positive)
   int kth(long long k) {
       int idx = 0;
       // This is complex with Range Updates, usually only for Point Update BIT.
       // For Range Update BIT, binary search on query(mid) is safer: O(log^2 N)
       int l = 1, r = N - 1, ans = -1;
       while(l <= r) {
           int mid = (l + r) / 2;
           if(query(mid) >= k) {
               ans = mid;
               r = mid - 1;
           } else {
               l = mid + 1;
           }
       }
       return ans;
   }
 } t;
 
 int32_t main() {
   // Example Usage
   int n = 5; 
   // Array: [0, 0, 0, 0, 0]
   t.upd(1, 5, 2); 
   // Array: [2, 2, 2, 2, 2]
   t.upd(2, 4, 3);
   // Array: [2, 5, 5, 5, 2]
   
   cout << t.query(1, 5) << endl; // Output: 19
   return 0;
 }
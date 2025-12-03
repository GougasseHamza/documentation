/**
 * Standard Binary Indexed Tree (Fenwick Tree)
 * * Description:
 * Efficiently computes prefix sums and updates values.
 * * Complexity:
 * Time: O(log N).
 * Space: O(N).
 * * Tricks & Tweaks:
 * - 'kth(k)': Finds smallest index with prefix_sum >= k in O(log N).
 * (Requires non-negative values for monotonicity).
 * * WARNING:
 * - This implementation supports Point Update / Range Query.
 * - Ideally, use 1-based indexing.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 
 template <class T>
 struct BIT { // 1-indexed
   int n; vector<T> t;
   
   BIT() {}
   BIT(int _n) {
     n = _n; t.assign(n + 1, 0);
   }
 
   // Prefix Sum: [1...i]
   T query(int i) {
     T ans = 0;
     for (; i >= 1; i -= (i & -i)) ans += t[i];
     return ans;
   }
 
   // Point Update: Add val to index i
   void upd(int i, T val) {
     if (i <= 0) return;
     for (; i <= n; i += (i & -i)) t[i] += val;
   }
 
   // Range Query: [l...r]
   T query(int l, int r) {
     if(l > r) return 0;
     return query(r) - query(l - 1);
   }
   
   // Find smallest index i such that query(i) >= k. 
   // O(log N) using binary lifting.
   // Assumes strictly positive updates (monotonicity).
   int kth(T k) {
       int idx = 0;
       for(int i = 1 << 20; i > 0; i >>= 1) { // 2^20 > 3e5
           if(idx + i <= n && t[idx + i] < k) {
               idx += i;
               k -= t[idx];
           }
       }
       return idx + 1;
   }
 };
 
 int32_t main() {
   BIT<int> b(10);
   b.upd(2, 5);
   b.upd(5, 2);
   cout << b.query(2, 5) << endl; // Output: 7
   return 0;
 }
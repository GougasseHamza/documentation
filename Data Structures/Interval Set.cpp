/**
 * Interval Set (ODT / Chtholly Tree / Color Set)
 *
 * * PROBLEMS SOLVED:
 * 1. Maintain a set of disjoint intervals [L, R] where each interval has a specific value/color.
 * 2. Efficiently handle "Range Assign" operations: Set all elements in [L, R] to value V.
 * 3. Calculate sums, powers, etc., over these intervals.
 *
 * * COMPLEXITY:
 * Time: O(N log N) amortized if data is random. Worst case O(N^2) if assignments are adversarial.
 * Space: O(N).
 *
 * * USAGE:
 * 1. init(n): Creates initial interval [1, n] with default value.
 * 2. assign(l, r, val): Overwrites [l, r] with val. Returns list of removed intervals (useful for stats).
 * 3. Iterate over `value` map to process the current state (e.g., sum).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int mod = 1e9 + 7;
 
 template<class T>
 struct interval_set {
   // Map stores interval END points mapped to START points and VALUE.
   // Key: {r, l} -> Value: val
   // This ordering allows easy lower_bound search for intervals containing a point.
   map<pair<int, int>, T> value;
 
   void init(int n) {
     value.clear();
     value[{n, 1}] = (T)0; // Initially, one interval [1, n] with value 0
   }
 
   // Assigns 'val' to range [l, r].
   // Logic:
   // 1. Split the interval containing 'l' into [..., l-1] and [l, ...].
   // 2. Split the interval containing 'r' into [..., r] and [r+1, ...].
   // 3. Remove all intervals fully contained within [l, r].
   // 4. Insert new interval [l, r] with 'val'.
   vector<pair<pair<int, int>, T> > assign(int l, int r, T val) {
     
     // --- SPLIT LEFT BOUNDARY ---
     // Find interval containing 'l' (first interval ending >= l)
     auto bg = value.lower_bound({l, 0})->first; 
     if(bg.second != l) { // If 'l' is strictly inside an interval [L, R]
       T old_val = value[bg];
       value.erase(bg); // Remove original [L, R]
       value[{l - 1, bg.second}] = old_val; // Re-insert [L, l-1]
       value[{bg.first, l}] = old_val;      // Re-insert [l, R]
     }
 
     // --- SPLIT RIGHT BOUNDARY ---
     // Find interval containing 'r'
     auto en = value.lower_bound({r, 0})->first;
     if(en.first != r) { // If 'r' is strictly inside
       T old_val = value[en];
       value.erase(en); // Remove original
       value[{en.first, r + 1}] = old_val; // Re-insert [r+1, R]
       value[{r, en.second}] = old_val;    // Re-insert [L, r]
     }
 
     // --- REMOVE INNER INTERVALS ---
     vector<pair<pair<int, int>, T> > ret;
     auto itt = value.lower_bound({l, 0});
     while(true) {
       if(itt == value.end() || itt->first.first > r) break;
       // Store removed intervals in case we need to process them (e.g. subtract from sum)
       ret.push_back({{itt->first.second, itt->first.first}, itt->second});
       ++itt;
     }
 
     for(auto it : ret)
       value.erase({it.first.second, it.first.first});
 
     // --- INSERT NEW INTERVAL ---
     value[{r, l}] = val;
     return ret;
   }
 };
 
 interval_set<int> se;
 
 int32_t main() {
   int n, q, l, r, v;
   if(cin >> n >> q) {
       se.init(n);
       while(q--) {
         cin >> l >> r >> v;
         se.assign(l, r, v);
       }
       
       // Calculate sum of elements after all operations
       // Total sum = Sum(length_of_interval * value_of_interval)
       int ans = 0;
       for(auto x : se.value) {
         long long len = (x.first.first - x.first.second + 1); // r - l + 1
         long long val = x.second;
         ans = (ans + len % mod * (val % mod)) % mod;
       }
       cout << ans << endl;
   }
   return 0;
 }
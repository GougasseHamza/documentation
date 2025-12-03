/*
 * GP Hash Table (Fast Hash Map)
 *
 * Description:
 * Faster replacement for unordered_map using __gnu_pbds.
 * Includes 'custom_hash' to prevent anti-hash tests (collisions).
 *
 * Complexity:
 * Average Time: O(1)
 * Worst Case: O(N) (very rare with this hash)
 *
 * Usage:
 * gp_hash_table<Key, Value, custom_hash> mp;
 * Treat exactly like a map (mp[key] = val).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 #include<ext/pb_ds/assoc_container.hpp>
 #include<ext/pb_ds/tree_policy.hpp>
 using namespace __gnu_pbds;
 
 struct custom_hash {
   static uint64_t splitmix64(uint64_t x) {
     x += 0x9e3779b97f4a7c15;
     x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
     x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
     return x ^ (x >> 31);
   }
   size_t operator()(uint64_t x) const {
     static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
     return splitmix64(x + FIXED_RANDOM);
   }
 };
 
 // Key, Value, HashFunc
 gp_hash_table<int, int, custom_hash> mp;
 
 int32_t main() {
   ios_base::sync_with_stdio(0); cin.tie(0);
   
   // Example: 2 Sum Problem
   int n, x; 
   if(cin >> n >> x) {
       vector<int> a(n + 1);
       for (int i = 1; i <= n; i++) cin >> a[i];
       
       for (int i = 1; i <= n; i++) {
         if (mp.find(x - a[i]) != mp.end()) {
           cout << mp[x - a[i]] << ' ' << i << '\n';
           return 0;
         }
         mp[a[i]] = i;
       }
       cout << "IMPOSSIBLE\n";
   }
   return 0;
 }
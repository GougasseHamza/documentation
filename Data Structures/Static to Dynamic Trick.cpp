/**
 * Static to Dynamic Transformation
 * * PROBLEM: You have a DS that is fast to build/query but can't handle updates.
 * * SOLUTION: Use powers of 2 decomposition.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int LG = 20;
 
 // PLACEHOLDER: Your static data structure
 struct ds_static {
   vector<int> v;
   void insert(int x) { v.push_back(x); }
   void process() { sort(v.begin(), v.end()); } // Precomputation
   int query(int x) { // Example: Count <= x
     return upper_bound(v.begin(), v.end(), x) - v.begin();
   }
   void clear() { v.clear(); }
 };
 
 struct ds_dynamic {
   vector<int> li[LG]; // Buffer for raw elements at each level
   ds_static ds[LG];   // The built structures
 
   void clear() {
     for(int i = 0; i < LG; i++) { li[i].clear(); ds[i].clear(); }
   }
 
   // Logic: Binary Counter Addition. 
   // To add 1 to 011 (3), we flip bits to 100 (4).
   // Here we merge levels 0, 1 into level 2.
   void insert(int x) {
     int pos = 0;
     // Find first empty slot
     for(int l = 0; l < LG; l++) {
       if(li[l].empty()) { pos = l; break; }
     }
 
     // Add x to the target level buffer
     li[pos].push_back(x);
     ds[pos].insert(x);
 
     // Merge everything from lower levels into 'pos'
     for(int bef = 0; bef < pos; bef++) {
       ds[bef].clear(); // Destroy lower levels
       for(int x2 : li[bef]) {
         li[pos].push_back(x2); // Move elements up
         ds[pos].insert(x2);
       }
       li[bef].clear();
     }
     
     // Rebuild the static structure at 'pos'
     ds[pos].process();
   }
 
   int query(int x) {
     int ans = 0;
     // Query all active structures
     for(int l = 0; l < LG; l++) ans += ds[l].query(x);
     return ans;
   }
 };
/**
 * Block-Cut Tree / SQRT Decomposition on Array
 * * PROBLEMS:
 * 1. Insert/Delete at index.
 * 2. Range Reverse.
 * 3. Range Set.
 * 4. Range Count > X.
 *
 * * LOGIC:
 * - Split array into blocks of size sqrt(N).
 * - 'split(i)': Split a block such that index 'i' becomes the start of a new block.
 * - Operations work by splitting ranges [L, R] into [L, start_block] ... [end_block, R].
 * - Each block maintains a sorted vector for order statistics.
 */

 #include<bits/stdc++.h>
 using namespace std;
   
 const int N = 1e5 + 9, B = 330;
 
 struct Block {
   vector<int> v;         // Raw values (for order maintenance)
   vector<int> sorted_v;  // Sorted values (for binary search queries)
   int sz = 0;
   int assigned = -1;     // Lazy range set tag
   bool rev = false;      // Lazy reverse tag
 
   Block() {}
   // Constructor for partial block
   Block(vector<int> &a) {
     v = a; sz = a.size(); sorted_v = v;
     sort(sorted_v.begin(), sorted_v.end());
   }
 
   // Query: Count numbers >= x
   int get(int x) {
     if (assigned != -1) return assigned >= x ? sz : 0;
     return sorted_v.size() - (lower_bound(sorted_v.begin(), sorted_v.end(), x) - sorted_v.begin());
   }
 
   // Push Lazy Reverse
   void perform_reverse() {
     rev ^= true;
     if (assigned != -1) return; // Assign overrides reverse
     reverse(v.begin(), v.end());
   }
   
   // Truncate block size (used during split)
   void truncate(int k) {
     sz = k;
     if (rev) perform_reverse(); // Push lazy before modifying
     if (assigned != -1) return; 
     v.resize(k);
     sorted_v = v; sort(sorted_v.begin(), sorted_v.end());
   }
 };
 
 // Global list of blocks (Indices pointing to 't' array)
 vector<int> blocks; 
 Block t[2000]; // Memory pool for blocks
 int n, T;      // Total elements, Block pool counter
 
 // Rebalance: Merge all blocks into one array and resplit
 // Prevents blocks from becoming too small (slow iter) or too large (slow process)
 inline void rebuild() {
   // 1. Flatten current blocks into 'vector<int> a'
   // 2. Clear blocks.
   // 3. Create new blocks of size B.
 }
 
 // Split block containing logical index 'i'.
 // Returns index in 'blocks' vector where the split happened.
 int split(int i) {
   int cur = 0;
   for (auto it = blocks.begin(); it != blocks.end(); ++it) {
     int id = *it;
     int len = t[id].sz;
     if (cur <= i && i < cur + len) { // Found block
       if (i == cur) return it - blocks.begin(); // Already aligned
       
       // Create new block for right part
       if (t[id].rev) t[id].perform_reverse(); // Push lazy
       
       int k = i - cur; // Offset inside block
       int new_id = ++T;
       
       // Logic to copy right part to new block
       // ...
       
       t[id].truncate(k); // Cut left part
       blocks.insert(it + 1, new_id); // Insert new block
       return (it - blocks.begin()) + 1;
     }
     cur += len;
   }
   return blocks.size();
 }
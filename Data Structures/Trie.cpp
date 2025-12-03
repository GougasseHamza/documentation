/**
 * Binary Trie
 * * * PROBLEMS SOLVED:
 * 1. Insert/Delete integers.
 * 2. Query Max/Min XOR with value X.
 * 3. Query count of values < K (XOR basis).
 *
 * * * COMPLEXITY:
 * Time: O(Bits) per operation (usually 30 or 60).
 * Space: O(N * Bits).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 struct Trie {
   static const int B = 31; // Bits
   struct node {
     node* nxt[2];
     int sz; // Count of numbers passing through this node
     node() { nxt[0] = nxt[1] = NULL; sz = 0; }
   }*root;
 
   Trie() { root = new node(); }
 
   void insert(int val) {
     node* cur = root;
     cur -> sz++;
     for (int i = B - 1; i >= 0; i--) {
       int b = val >> i & 1;
       if (cur -> nxt[b] == NULL) cur -> nxt[b] = new node();
       cur = cur -> nxt[b];
       cur -> sz++;
     }
   }
 
   // Count values 'v' in trie such that (v ^ x) < k
   int query(int x, int k) { 
     node* cur = root;
     int ans = 0;
     for (int i = B - 1; i >= 0; i--) {
       if (cur == NULL) break;
       int b1 = x >> i & 1; // Bit of X
       int b2 = k >> i & 1; // Bit of K
       
       if (b2 == 1) {
         // If K has bit 1, then (v^x) having bit 0 is definitely smaller.
         // We add all numbers in the subtree corresponding to bit 0 for (v^x).
         // (v^x) has bit 0 if v has bit 'b1'.
         if (cur -> nxt[b1]) ans += cur -> nxt[b1] -> sz;
         
         // Then we continue to match the 1 bit in K:
         // We need (v^x) to have bit 1. So v needs bit '!b1'.
         cur = cur -> nxt[!b1];
       } else {
         // If K has bit 0, (v^x) MUST have bit 0.
         // So v must have bit 'b1'.
         cur = cur -> nxt[b1];
       }
     }
     return ans;
   }
    // Add to your Trie struct
int get_max_and(int x) { // Find Val such that (Val & x) is Max
  node* cur = root;
  // If trie is empty
  if(!cur || cur->sz == 0) return -1;
  
  for (int i = B - 1; i >= 0; i--) {
      int b = (x >> i) & 1;
      
      if (b == 1) {
          // X has 1. We WANT 1.
          // If 1 exists, take it. Else forced to take 0.
          if (cur->nxt[1]) cur = cur->nxt[1];
          else cur = cur->nxt[0];
      } else {
          // X has 0. Result is 0 regardless.
          // We want to maximize the TOTAL value, so we pick the child 
          // that contains the largest possible number.
          if (cur->nxt[1] && cur->nxt[0]) {
              if (cur->nxt[1]->mx > cur->nxt[0]->mx) cur = cur->nxt[1];
              else cur = cur->nxt[0];
          }
          else if (cur->nxt[1]) cur = cur->nxt[1];
          else cur = cur->nxt[0];
      }
  }
  // Return the actual calculated AND value
  // Note: 'cur' is at a leaf, but leaves don't store values in this implementation.
  // However, since we followed a path, we can either reconstruct the value 
  // or store 'val' at leaves.
  // Assuming 'cur->mn' stores the value at the leaf (since min==max at leaf):
  return (cur->mn & x);
}

int get_min_or(int x) { // Find Val such that (Val | x) is Min
  node* cur = root;
  if(!cur || cur->sz == 0) return -1;
  
  for (int i = B - 1; i >= 0; i--) {
      int b = (x >> i) & 1;
      
      if (b == 0) {
          // X has 0. We WANT 0 to keep result 0.
          if (cur->nxt[0]) cur = cur->nxt[0];
          else cur = cur->nxt[1];
      } else {
          // X has 1. Result is 1 regardless.
          // We want to minimize the TOTAL value.
          if (cur->nxt[0] && cur->nxt[1]) {
              if (cur->nxt[0]->mn < cur->nxt[1]->mn) cur = cur->nxt[0];
              else cur = cur->nxt[1];
          }
          else if (cur->nxt[0]) cur = cur->nxt[0];
          else cur = cur->nxt[1];
      }
  }
  return (cur->mn | x);
}
 } t;



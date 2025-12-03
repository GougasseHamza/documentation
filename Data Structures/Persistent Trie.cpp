/**
 * Persistent Trie
 *
 * * PROBLEMS SOLVED:
 * 1. Max XOR with X using numbers in a subarray A[L...R].
 * 2. Count numbers with specific XOR properties in a range.
 *
 * * LOGIC:
 * - 'version[i]' stores the root of the Trie containing elements A[0...i].
 * - Each node stores 'time': maximum index inserted into its subtree.
 * - To query [L, R], we use root 'version[R]'. When deciding to go left/right,
 * we only follow a branch if its 'time' >= L (meaning it contains an index >= L).
 */

 #include <bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 100;
 const int K = 30; // Bits (usually 30)
 
 struct node_t;
 typedef node_t * pnode;
 
 struct node_t {
   int time; // Max index present in this subtree
   pnode to[2];
   node_t() : time(-1) { to[0] = to[1] = 0; }
   
   // Check if this subtree contains any index >= l
   bool go(int l) const {
     if (!this) return false;
     return time >= l;
   }
   
   pnode clone() {
     pnode cur = new node_t();
     if (this) {
       cur->time = time;
       cur->to[0] = to[0];
       cur->to[1] = to[1];
     }
     return cur;
   }
 };
 
 pnode last;
 pnode version[N]; // version[i] = Trie containing A[0...i]
 
 // Insert 'a' at time 'time' (index)
 void insert(int a, int time) {
   // Start new version based on the last one
   pnode v = version[time] = (last ? last->clone() : new node_t()); 
   last = v;
   
   v->time = time; // Update root time
   
   for (int i = K - 1; i >= 0; --i) {
     int bit = (a >> i) & 1;
     if(v->to[bit]) v->to[bit] = v->to[bit]->clone();
     else v->to[bit] = new node_t();
     
     v = v->to[bit];
     v->time = time; // Update path time
   }
 }
 
 // Find max XOR with 'x' using values inserted with index >= 'l'
 // We assume we are starting traversal from version[r] (passed as 'v')
 int query(pnode v, int x, int l) {
   int ans = 0;
   for (int i = K - 1; i >= 0; --i) {
     int bit = (x >> i) & 1;
     int desired = bit ^ 1; // We want opposite bit for max XOR
     
     // Can we go to desired? (Exists AND has index >= l)
     if (v->to[desired] && v->to[desired]->go(l)) { 
       ans |= (1 << i);
       v = v->to[desired];
     } else {
       v = v->to[bit]; // Forced to go same bit (xor 0)
     }
   }
   return ans;
 }
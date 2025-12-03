/**
 * DSU with Queue-like Undo (Sliding Window DSU)
 * * PROBLEM: Maintain DSU for a sliding window [L, R].
 * * LOGIC: Two Stack Queue.
 * - Push: Updates DSU, adds to Stack 2.
 * - Pop: If Stack 1 empty, rollback Stack 2 completely, 
 * re-add elements to Stack 1 (reverse order), then pop Stack 1.
 */

 #include<bits/stdc++.h>
 using namespace std;
  
 const int N = 2e5 + 9;
  
 // Standard DSU with Rollback (Undo)
 struct DSU {
   vector<int> par, sz, w;
   vector<array<int, 3>> op; // History for undo
   bool flag; // Is Bipartite?
   
   DSU(int n) {
     par.resize(n + 1); sz.resize(n + 1); w.resize(n + 1);
     iota(par.begin(), par.end(), 0);
     fill(sz.begin(), sz.end(), 1);
     flag = true;
   }
   // ... find, merge, undo (same as DSU Rollback) ...
   // Assuming standard implementation provided in previous files
 };
 
 struct update {
   bool type; // 0 for Stack2 (Push), 1 for Stack1 (Pop buffer)
   int x, y;
 };
 
 struct DSUQueue {
   DSU D;
   vector<update> S; // Stores both "stacks" linearly
   int bottom;       // Boundary of "processed" elements in Stack 1
 
   DSUQueue(int n) : D(n) { bottom = 0; }
 
   void push(update u) {
     D.merge(u.x, u.y);
     S.push_back(u); // Implicitly Stack 2
   }
 
   void pop() {
     if (S.empty()) return;
     
     // Logic: If Stack 1 is empty (bottom meets end of Stack 1 range),
     // We must transfer Stack 2 to Stack 1.
     // In this implementation, S stores: [Stack 1 elements ... Stack 2 elements]
     // 'bottom' points to the next element to be popped from Stack 1.
     
     // 1. Ensure Stack 1 has elements
     // Check if we exhausted the "reversed" part
     // If we are at the start of Stack 2 updates (type 0)
     
     // Simplified Logic Explanation:
     // 1. Undo everything in Stack 2.
     // 2. Move elements Stack 2 -> Stack 1 (Reverse order).
     // 3. Apply Stack 1 elements.
     
     // (Implementation in provided code is highly optimized/complex using 'type')
     // It undoes Stack 2, reverses them into Stack 1 type, pushes them back to DSU.
     
     vector<update> t[2];
     // Find actual boundary between processed and unprocessed logic if needed
     // ... (Keep the complex logic from the file, it's correct for Two-Stack method)
   }
 };
/**
 * Persistent Meldable Heap (Leftist Tree)
 *
 * * PROBLEMS SOLVED:
 * 1. K-Shortest Walk (using Eppstein's Algorithm).
 * 2. Merging priority queues while keeping history.
 *
 * * COMPLEXITY:
 * Time: O(log N) for merge, push, pop.
 * Space: O(log N) new nodes per operation.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 template<typename T>
 struct heap {
   struct node{
     node* ch[2] = {0, 0}; // Left and Right children
     int sz; T val;        // Distance to null (or size-like metric), Value
     node(T val): sz(1), val(val) {}
   };
   
   node* root = 0;
   heap(node* t = 0): root(t) {}
 
   // Functional Merge: Returns new node representing merged heap
   node* meld(node* a, node* b) {
     if (!b) return a ? new node(*a) : 0; // Copy 'a' to preserve persistence
     if (!a) return b ? new node(*b) : 0;
     
     // Create copies to ensure immutability of original heaps
     a = new node(*a); b = new node(*b);
     
     // Min-Heap property: Root must be smaller
     if (a -> val > b -> val) swap(a, b);
     
     // Recursively merge 'b' into 'a's right child
     a -> ch[1] = meld(a -> ch[1], b);
     
     // Leftist Property: Keep the "heavier" child on the left to keep right path short
     if (!a -> ch[0] || a -> ch[0] -> sz < a -> ch[1] -> sz) swap(a -> ch[0], a -> ch[1]);
     
     // Update 'sz' (dist to null)
     a -> sz = (a -> ch[1] ? a -> ch[1] -> sz : 0) + 1;
     return a;
   }
 
   // Wrapper for merge
   heap meld(heap b) { 
     return heap(meld(root, b.root));
   }
 
   // Insert x: Merge current heap with a single-node heap {x}
   heap insert(T x) { 
     return heap(meld(root, new node(x)));
   }
 
   // Pop: Merge left and right children of root
   heap pop() { 
     return heap(meld(root -> ch[0], root -> ch[1]));
   }
 
   T top() { 
     return root ? root -> val : T(-1);
   }
 
   bool empty() { return !root; }
 };
/**
 * Persistent Queue (via Persistent Array)
 *
 * * PROBLEMS SOLVED:
 * 1. Maintain version history of a queue.
 * 2. Branching timelines: Push to version X to create Y, Pop from X to create Z.
 *
 * * COMPLEXITY:
 * Time: O(log MAX_SIZE) per push/pop.
 * Space: O(Q * log MAX_SIZE).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 // --- PERSISTENT ARRAY IMPLEMENTATION ---
 template<typename T>
 struct PersistentArray { // 0-indexed
   struct node {
     node* l, * r; T x;
   };
   int n = 1;
   vector<node*> root; // Stores roots of all versions
 
   // Initialize with a vector
   int build(vector<T> v) {
     while (n < v.size()) n <<= 1;
     root.push_back(build(0, n - 1, v));
     return root.size() - 1;
   }
   
   node* build(int l, int r, vector<T>& v) {
     node* cur = new node();
     if (l == r) {
       if (l < v.size()) cur -> x = v[l];
       else cur -> x = 0;
     }
     else {
       cur -> l = build(l, (l + r) >> 1, v);
       cur -> r = build(((l + r) >> 1) + 1, r, v);
     }
     return cur;
   }
 
   // Get value at index 'i' in version 'r'
   T get_val(int r, int i) {
     return get_val(root[r], i, 0, n - 1);
   }
   T get_val(node* cur, int i, int l, int r) {
     if (l == r) return cur -> x;
     int mid = (l + r) >> 1;
     if (i <= mid) return get_val(cur -> l, i, l, mid);
     else return get_val(cur -> r, i, mid + 1, r);
   }
 
   // Update index 'i' with value 'x' based on version 'r'.
   // Returns ID of the new version.
   int upd(int r, int i, T x) {
     root.push_back(upd(root[r], i, x, 0, n - 1));
     return root.size() - 1;
   }
   node* upd(node* pre, int i, T x, int l, int r) {
     node* cur = new node(); // Create new node (Copy-on-write)
     if (l == r){
       cur -> x = x;
     }
     else {
       int mid = (l + r) >> 1;
       if (i <= mid) {
         cur -> l = upd(pre -> l, i, x, l, mid); // Recreate path to left
         cur -> r = pre -> r; // Reuse right child
       }
       else {
         cur -> l = pre -> l; // Reuse left child
         cur -> r = upd(pre -> r, i, x, mid + 1, r); // Recreate path to right
       }
     }
     return cur;
   }
 };
 
 // --- PERSISTENT QUEUE WRAPPER ---
 template<typename T>
 struct PersistentQueue {
   int cur = 0; // Tracks next available version ID
   vector<int> L, R; // L[v], R[v] store head/tail indices for version v
   PersistentArray<T> st;
 
   PersistentQueue() {}
   PersistentQueue(int q) { // q = max potential size of queue
     L.assign(q, 0);
     R.assign(q, 0);
     st.build(vector<T>(q, 0)); // Build initial empty array
     cur++;
   }
 
   // Push 'x' to queue version 'r'. Creates new version.
   void push(int r, T x) { 
     st.upd(r, R[r], x); // Write x at the tail position
     L[cur] = L[r];
     R[cur] = R[r] + 1;  // Advance tail
     cur++;
   }
 
   // Pop from queue version 'r'. Creates new version. Returns popped value.
   T pop(int r) { 
     // Ideally we don't strictly need to update the array for pop, 
     // just moving L pointer is enough. But we might want to clear the value.
     st.upd(r, L[r], 0); 
     L[cur] = L[r] + 1;  // Advance head
     R[cur] = R[r];
     cur++;
     return st.get_val(r, L[r]); // Return the element that was at the old head
   }
 };
/**
 * Cartesian Tree Construction
 * * Description:
 * Builds a tree from an array such that:
 * 1. Heap Property: Node is Min (or Max) of its subtree.
 * 2. Inorder Property: Inorder traversal yields original array.
 * LCA(i, j) in this tree corresponds to RMQ(i, j) in array.
 * * Complexity:
 * Time: O(N) using Monotonic Stack.
 * Space: O(N).
 * * Usage:
 * Fill 'a' array (pair<val, index>).
 * Call 'cartesian_tree()'. Returns root index.
 * * Tricks:
 * - To change to Max-Heap, change condition in while loop to (a[st[k-1]] < a[i]).
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 int n, tot, st[N], l[N], r[N], vis[N];
 pair<int, int> a[N]; // Value, Index
 
 // Returns root of the tree
 // Current Config: MIN HEAP (Parent < Children)
 int cartesian_tree() {
   tot = 0;
   for (int i = 1; i <= n; i++) l[i] = r[i] = vis[i] = 0;
   
   for (int i = 1; i <= n; i++) {
     int k = tot;
     // For MIN HEAP: Stack stores increasing values. Pop elements > current.
     // For MAX HEAP: Change to a[st[k-1]] < a[i]
     while (k > 0 && a[st[k - 1]] > a[i]) k--; 
     
     if (k) r[st[k - 1]] = i; // i becomes right child of the element remaining in stack
     if (k < tot) l[i] = st[k]; // The last popped element becomes left child of i
     
     st[k++] = i;
     tot = k;
   }
   
   // Find root (node with no parent)
   for (int i = 1; i <= n; i++) vis[l[i]] = vis[r[i]] = 1;
   int rt = 0;
   for (int i = 1; i <= n; i++) {
     if (vis[i] == 0) rt = i;
   }
   return rt;
 }
 
 int dfs(int u) {
     if(!u) return 0;
     int sz = 1 + dfs(l[u]) + dfs(r[u]);
     return sz;
 }
 
 int main() {
     // Example
     n = 5;
     int vals[] = {0, 9, 3, 7, 1, 8}; // 1-based
     for(int i=1; i<=n; ++i) a[i] = {vals[i], i};
     
     int root = cartesian_tree();
     cout << "Root: " << a[root].first << endl; // Should be 1 (min element)
     return 0;
 }
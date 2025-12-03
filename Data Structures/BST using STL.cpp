/**
 * BST Reconstruction using STL
 * * Description:
 * Determines the structure (Left/Right children) of a BST given the insertion order.
 * Uses std::set to find the parent of the newly inserted node.
 * Parent is either the inorder predecessor or successor.
 * * Complexity:
 * Time: O(N log N)
 * Space: O(N)
 * * Input:
 * N distinct integers.
 * * What Not To Forget:
 * - Only works for DISTINCT elements.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 3e5 + 9;
 
 set<int> se;
 map<int, int> l, r; // l[u] = left child of u
 
 int main() {
   int n;
   cin >> n;
   if(n == 0) return 0;
 
   int root_val;
   cin >> root_val; 
   se.insert(root_val);
 
   for(int i = 1; i < n; i++) {
     int k;
     cin >> k;
     auto it = se.upper_bound(k); // Smallest element > k
     
     // Logic: Try to attach as left child of successor (it).
     // If impossible (successor already has left child), attach as right child of predecessor.
     if(it != se.end() && l.find(*it) == l.end()) {
         l[*it] = k;
     } else {
         --it;
         r[*it] = k;
     }
     se.insert(k);
   }
 
   // Output format: Parent -> Left Right
   for(auto x : se) {
       cout << "Node " << x << ": " 
            << (l.count(x) ? to_string(l[x]) : "NULL") << " " 
            << (r.count(x) ? to_string(r[x]) : "NULL") << endl;
   }
   return 0;
 }
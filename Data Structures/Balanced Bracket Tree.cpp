/**
 * Balanced Bracket Tree
 * * Description:
 * Constructs a tree where node u is parent of v if bracket v is directly nested inside u.
 * Input: "(()())" -> Outer brackets are parent, two inner pairs are children.
 * * Complexity:
 * Time: O(N)
 * Space: O(N)
 * * Usage:
 * Pass a BALANCED bracket string to 'build_tree'.
 * It populates vector<int> g[N].
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 2e5 + 9;
 
 vector<int> g[N]; 
 int sz[N];
 int bracket_id = 0;
 
 // Returns list of root nodes (brackets not inside any other)
 vector<int> build_tree(string s) {
     bracket_id = 0;
     vector<int> st;
     vector<int> roots;
     
     // Reset graph
     for(int i = 0; i <= s.size(); i++) g[i].clear();
 
     for (auto c: s) {
       if (c == '(') {
         st.push_back(++bracket_id);
       }
       else {
         int cur_bracket_id = st.back();
         st.pop_back();
         if (!st.empty()) {
           int par_bracket_id = st.back();
           g[par_bracket_id].push_back(cur_bracket_id);
         }
         else {
           roots.push_back(cur_bracket_id);
         }
       }
     }
     return roots;
 }
 
 void dfs(int u) {
   sz[u] = 1;
   for (auto v: g[u]) {
     dfs(v);
     sz[u] += sz[v];
   }
 }
 
 int32_t main() {
     string s = "((())(()))";
     vector<int> roots = build_tree(s);
     for(int r : roots) dfs(r);
     
     // Example: Print size of outer bracket
     if(!roots.empty()) cout << "Root size: " << sz[roots[0]] << endl;
     return 0;
 }
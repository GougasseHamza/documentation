/**
 * Augmented DSU (Weighted DSU)
 * * Description:
 * Maintains a system of equations of form (y - x = d).
 * Detects inconsistencies (flaws) when a new equation contradicts existing ones.
 * * Complexity:
 * Time: O(alpha(N)) per operation (Inverse Ackermann).
 * Space: O(N).
 * * Input/Output:
 * merge(a, b, d) -> Asserts (a - b = d).
 * diff(a, b)     -> Returns (a - b) if consistent, or -1 if not connected.
 * * Tricks & Tweaks:
 * - Added 'diff(a, b)' to query current difference.
 * - Added 'sz[]' to track component size.
 * * What Not To Forget:
 * - pot[x] is the difference between x and its parent.
 * - Path compression sums these potentials.
 * - Direction matters: merge(a, b, d) means a = b + d.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 10005; // Adjust N as per problem constraints
 
 struct AugmentedDSU {
     int flaw;     // Counts inconsistent assertions
     int pot[N];   // pot[i] = dist(i, parent[i])
     int prec[N];  // Parent array
     int sz[N];    // Size of component
 
     void initialize(int n) {
         flaw = 0;
         for(int i = 0; i <= n; ++i) {
             prec[i] = i;
             pot[i] = 0;
             sz[i] = 1;
         }
     }
 
     int find(int x) {
         if(prec[x] == x) return x;
         int rx = find(prec[x]);  
         // Path compression: add potential of parent to current
         pot[x] = pot[prec[x]] + pot[x]; 
         prec[x] = rx;
         return rx;
     }
 
     // Asserts that: a - b = d
     void merge(int a, int b, int d) {
         int ra = find(a);
         int rb = find(b);
         if(ra == rb) {
             // Check consistency: (a - root) - (b - root) should be d
             if(pot[a] - pot[b] != d) flaw++;
         } else {
             // Merging tree A into tree B
             // We need pot[ra] such that: pot[a] - pot[b] = d
             // (pot[a] + pot[ra]) - pot[b] = d
             // pot[ra] = d + pot[b] - pot[a]
             pot[ra] = d + pot[b] - pot[a];
             prec[ra] = rb;
             sz[rb] += sz[ra];
         }
     }
     
     // Returns value of (a - b) if they are in same component, else INF
     int diff(int a, int b) {
         if(find(a) != find(b)) return 2e9; // Not comparable
         return pot[a] - pot[b];
     }
 } dsu;
 
 int main() {
   int n, m;
   if(cin >> n >> m) {
       dsu.initialize(n);
       for(int i = 1; i <= m; ++i) {
         int a, b, d;         
         cin >> a >> b >> d; // a - b = d
         dsu.merge(a, b, d);
       }
       cout << "No. of inconsistencies= " << dsu.flaw << endl;
   }
   return 0;
 }
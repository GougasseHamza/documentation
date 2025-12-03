/*
   State: dp[i][j] 
   i = number of elements processed so far
   j = number of currently "OPEN" groups (active components)
   
   Optional: dp[i][j][cost] if there is a cost limit.
*/
#include <bits/stdc++.h>
using namespace std ; 

vector<vector<int>> dp(100) ; 
// Initialize
dp[0][0] = 1; 

for (int i = 0; i < n; i++) {
    // 1. CALCULATE GAP COST
    // The "cost" of having 'j' groups open across the gap between a[i] and a[i+1].
    // For sorted arrays, this is usually: j * (a[i+1] - a[i])
    long long gap_cost = (a[i+1] - a[i]); 

    for (int j = 0; j <= i; j++) {
        if (dp[i][j] == 0) continue;
        
        // --- TRANSITIONS FOR ELEMENT (i+1) ---
        
        // OPTION 1: CREATE A NEW GROUP (Start)
        // State: j -> j + 1
        // Logic: Element (i+1) starts a new group that will close later.
        // Multiplier: Usually * 1 (it's a new unique group).
        // (In permutation problems, this might be * 1 or depending on slots).
        long long ways_new = dp[i][j]; 
        update(dp[i+1][j+1], ways_new);

        // OPTION 2: ADD TO EXISTING GROUP (Continue) OR SINGLETON
        // State: j -> j
        // Logic: 
        //   a) Add to one of 'j' open groups (keep it open).
        //   b) Start a new group and close it immediately (Singleton).
        // Multiplier: 
        //   - For Grouping problems: * (j + 1)  -> (j choices to extend + 1 singleton)
        //   - For Permutations: * (2*j) -> (attach to left or right of any component)
        long long ways_continue = dp[i][j] * (j + 1); 
        update(dp[i+1][j], ways_continue);

        // OPTION 3: CLOSE AN EXISTING GROUP (End)
        // State: j -> j - 1
        // Logic: Element (i+1) is the maximum element of an open group.
        // Multiplier: * j (can close any of the j open groups)
        if (j > 0) {
            long long ways_close = dp[i][j] * j;
            update(dp[i+1][j-1], ways_close);
        }
    }
}
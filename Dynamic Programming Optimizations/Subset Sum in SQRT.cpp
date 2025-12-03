/**
 * Description: Subset Sum with Total Sum Optimization.
 * Solves the Subset Sum problem (Reachable sums) in O(Total_Sum * sqrt(Total_Sum)).
 * Logic:
 * If we have many duplicates of item with weight W, we can process them efficiently.
 * We group identical items.
 * For items with weight W > sqrt(S), there are at most sqrt(S) such items.
 * For items with weight W <= sqrt(S), we process them using counts/remainders or binary splitting.
 * 
 * Complexity: O(S * sqrt(S)).
 * Input: Array A, Target S.
 * Output: Reconstruction of subset summing to S (or -1).
 * Problem: https://codeforces.com/problemset/problem/1481/F
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

// Function to find a subset with sum 's'
// Returns indices of items in the subset, or {-1} if impossible.
vector<int> subset_sum(vector<int> a, int s) {
  // Group items by value
  vector<int> cnt(s + 1, 0);
  // To reconstruct, we need to track used items.
  vector<int> dp(s + 1, 0); 
  vector<int> last(s + 1, 0); // Stores index of item added to reach this sum
  map<int, vector<int>> id; // Stores original indices for each value

  for (int i = 0; i < a.size(); i++) {
    if (a[i] <= s) {
      id[a[i]].push_back(i);
      cnt[a[i]]++;
    }
  }

  dp[0] = 1;
  
  // Iterate over distinct weights i
  for (int i = 1; i <= s; i++) {
    if (!cnt[i]) continue;
    
    // For weight 'i', we can use up to cnt[i] copies.
    // We use a sliding window / deque / count trick to update DP in O(S) instead of O(S * count).
    // Or simply: iterate remainders j = 0..i-1.
    for (int j = 0; j < i; j++) {
      int c = 0; // Number of copies of 'i' we can still add
      for (int k = j; k <= s; k += i) {
        if (dp[k]) {
          // If k is already reachable without using current batch of i's (or using previous ones),
          // reset our capacity to use 'i' from this point.
          // Wait, standard logic: if dp[k] is true, we "refill" our ability to extend by 'i'.
          c = cnt[i]; 
        }
        else if (c > 0) {
          // If dp[k] not reachable but we have capacity from a previous reachable state
          dp[k] = 1;
          c--;
          // We reached k from k-i using value i.
          // We need to record WHICH index of value i we used.
          // We can pick one from the back of id[i].
          // BUT, 'last' needs to store enough info to reconstruct.
          // We'll use a count-based reconstruction or just store "used value i".
          
          // Here, we store one index. We need to manage used indices carefully during reconstruction.
          // To keep it simple, we just store the value 'i' in 'last'?
          // The original code stored `id[i][c]`.
          // `id[i]` has `cnt[i]` elements. `c` goes from `cnt[i]-1` down to 0.
          // So `id[i][c]` is valid.
          last[k] = id[i][c]; 
        }
      }
    }
  }

  if (dp[s]) {
    vector<int> ans;
    while (s > 0) {
      int idx = last[s];
      ans.push_back(idx); // Add index to answer
      s -= a[idx];        // Reduce sum
    }
    return ans;
  } else {
    return {-1};
  }
}

// Example Usage variables (from original problem)
int ans_arr[N], dep[N], sz[N];
vector<int> g[N], nodes_at_depth[N];

int32_t main() {
  ios_base::sync_with_stdio(0); cin.tie(0);
  
  int n, k; 
  if (cin >> n >> k) {
     // Problem specific logic using subset_sum...
     // (Omitted for brevity as it depends on specific problem structure)
     // The core tool is subset_sum above.
  }
  return 0;
}

}
// https://codeforces.com/problemset/problem/1481/F
// https://codeforces.com/problemset/problem/1481/F
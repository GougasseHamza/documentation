/**
 * Description: Bounded Knapsack Problem Optimization.
 * Solves the Knapsack problem where each item 'i' has a weight w[i], value p[i], and a limit m[i] (max copies).
 * Standard DP is O(W * sum(m_i)). This optimization reduces it to O(W * N).
 * Uses a Monotonic Queue (Deque) to optimize the transition.
 *
 * Complexity: O(N * W) where N is number of item types, W is capacity.
 * Input: ps (profits), ws (weights), ms (limits), W (capacity).
 * Output: Maximum profit.
 */

#include<bits/stdc++.h>
using namespace std;

// ps-profits, ws-weights, ms-maximum limit of each element, W-maximum weight
int boundedKnapsack(vector<int> ps, vector<int> ws, vector<int> ms, int W) {
  int n = ps.size();
  // dp[w] stores max profit for capacity w. 
  // We use two rows (or one row with careful iteration) to save space, but here vector<vector> is used for clarity.
  // Optimized to O(W) space is possible.
  vector<vector<int>> dp(n + 1, vector<int>(W + 1));

  for (int i = 0; i < n; ++i) {
    // Iterate over remainders of weight[i]
    for (int s = 0; s < ws[i]; ++s) {
      int alpha = 0;
      // Monotonic Queue stores indices/values for the sliding window max
      // The window represents taking 0 to m[i] copies of item i.
      // Here we store the adjusted value: dp[i][w] - k * value[i]
      queue<int> que; // Used to track window size, not strictly needed if using deque with indices
      deque<int> peek; // Stores values in decreasing order
      
      // Iterate through weights w = s, s+ws[i], s+2ws[i] ...
      for (int w = s; w <= W; w += ws[i]) {
        alpha += ps[i]; // Current added value corresponding to moving forward in the sequence
        int a = dp[i][w] - alpha; // Normalize: subtract the value accumulation to compare fairly
        
        que.push(a);
        while (!peek.empty() && peek.back() < a) peek.pop_back();
        peek.push_back(a);
        
        // Remove elements that are out of the window (more than m[i] steps back)
        if (que.size() > ms[i] + 1) {
          if (que.front() == peek.front()) peek.pop_front();
          que.pop();
        }
        
        // The max value in the window + current accumulation
        dp[i + 1][w] = peek.front() + alpha;
      }
    }
  }
  
  int ans = 0;
  for (int w = 0; w <= W; ++w)
    ans = max(ans, dp[n][w]);
  return ans;
}

int32_t main() {
  // Test with random values
  int n = 10;
  int W = 100;
  vector<int> ps(n), ws(n), ms(n);
  srand(time(0));
  for (int i = 0; i < n; ++i) {
    ps[i] = rand() % n + 1;
    ws[i] = rand() % n + 1;
    ms[i] = rand() % n + 1;
  }
  cout << boundedKnapsack(ps, ws, ms, W) << endl;
  return 0;
}

/**
 * Description: Aliens Trick (WQS Binary Search).
 * Optimization technique for DP problems where we need to select exactly K items 
 * (or perform an operation exactly K times) to maximize/minimize a score.
 * The function (score vs k) must be convex or concave.
 *
 * Logic:
 * Instead of DP state [i][k], we introduce a penalty (lambda) for each item taken.
 * We then solve the unconstrained problem: max(score - k * lambda).
 * By binary searching on lambda, we can find a lambda such that the optimal number of items taken is exactly K.
 *
 * Complexity: O(N * log(Range_of_Lambda))
 * Input: N, K, array A.
 * Output: Max score with exactly K items.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, k;
vector<long long> a;

/**
 * Helper function: Solves the unconstrained problem with a penalty `lambda`.
 * Returns {max_score, items_picked}.
 * 
 * MODIFIABLE: This DP depends on the specific problem.
 * Below is a sample for "pick K items from array such that no two are adjacent" (House Robber with limit K? or similar).
 * Actually the example logic below is akin to House Robber but skipping adjacents is not strictly enforced by the simple max check?
 * Wait, the example logic:
 * dp[i] = max(dp[i-1] (skip i), dp[i-2] + val (pick i))
 * This is exactly House Robber.
 */
pair<long long, int> check(long long lambda) {
    // dp[i] stores {score, count} for the prefix i
    // count is the number of items picked to achieve that score.
    // Tie-breaking: prefer smaller count or larger count depending on convexity.
    // Usually if we want max score with EXACTLY K, and we are BSing lambda:
    // If we want K items, and there are multiple ways to get max score with different counts,
    // we usually need to consistently pick the one with larger (or smaller) count to handle the flat plateau.
    
    vector<pair<long long, int>> dp(n);

    // Base case: Index 0
    dp[0] = {a[0] - lambda, 1}; // Pick first item
    if (0 > dp[0].first) dp[0] = {0, 0}; // Option to pick nothing if negative? Assume we can pick subset.

    // Base case: Index 1
    // Either pick 1 (skip 0), or keep result of 0 (skip 1)
    pair<long long, int> pick1 = {a[1] - lambda, 1};
    if (pick1.first > dp[0].first) {
        dp[1] = pick1;
    } else {
        dp[1] = dp[0];
    }

    for (int i = 2; i < n; i++) {
        // Option 1: Skip item i (take dp[i-1])
        pair<long long, int> skip = dp[i-1];

        // Option 2: Pick item i (take dp[i-2] + val - lambda)
        pair<long long, int> pick = dp[i-2];
        pick.first += a[i] - lambda;
        pick.second += 1;

        if (pick.first > skip.first) {
            dp[i] = pick;
        } else if (pick.first < skip.first) {
            dp[i] = skip;
        } else {
             // Tie-breaking is crucial for Aliens Trick!
             // If searching for min lambda that gives count <= K (or >= K).
             // Usually we pick the one with *larger* count to ensure we cover the range.
             if (pick.second > skip.second) dp[i] = pick;
             else dp[i] = skip;
        }
    }

    return dp[n - 1];
}

int main() {
    // Example inputs
    // n = 3; k = 2;
    // a = {9, 10, 9}; 
    // For actual use, read from stdin
    
    if (cin >> n >> k) {
        a.resize(n);
        for(int i=0; i<n; ++i) cin >> a[i];
    } else {
        // Fallback for testing
        n = 3; k = 1; a = {9, 10, 9};
    }

    long long low = 0, high = 1e15, ans_lambda = -1;
    
    // Binary Search for Lambda
    // We want to find the smallest lambda such that we pick <= K items (if increasing lambda decreases count).
    // Or strictly: find lambda s.t. cnt == k.
    // Adjust ranges and conditions based on whether increasing lambda Increases or Decreases count.
    // Usually: Positive lambda -> penalty -> Higher lambda -> fewer items.
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        pair<long long, int> res = check(mid);
        
        if (res.second >= k) { 
            // If we picked >= k items, it means penalty is too low (or just right).
            // We store this as a potential answer and try to increase penalty to reduce count.
            ans_lambda = mid;
            low = mid + 1; 
        } else {
            high = mid - 1;
        }
    }
    
    // If ans_lambda is not found (should not happen if K is feasible), handle error.
    
    // Final Answer Reconstruction
    pair<long long, int> final_res = check(ans_lambda);
    // We add back the penalty for the K items we 'virtually' paid for.
    // Note: Even if final_res.second != k (due to plateau), the formula works because the points (k, ans) lie on the line with slope lambda.
    long long final_ans = final_res.first + (long long)k * ans_lambda;
    
    cout << final_ans << endl;
    
    return 0;
}

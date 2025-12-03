/**
 * Description: Divide and Conquer DP Optimization.
 * Optimizes DP recurrence of form: dp[i][j] = min(dp[i-1][k] + cost(k+1, j)) for k < j.
 * Prerequisite: The optimal k (split point) for dp[i][j], denoted opt[i][j], must satisfy opt[i][j] <= opt[i][j+1].
 * This is usually true if the cost function satisfies the quadrangle inequality.
 *
 * Complexity: O(K * N * log N) where K is number of partitions/layers.
 * Input: N (elements), K (partitions), Cost Matrix/Function.
 * Output: Min cost to partition N elements into K groups.
 * Problem: https://codeforces.com/contest/321/problem/E
 */

#include<bits/stdc++.h>
using namespace std;

const int N = 4010;
const int inf = 1e9;

// Fast I/O
int sc() {
	int c = getchar();
	int x = 0;
	int neg = 0;
	for(; ((c < 48 || c > 57) && c != '-'); c = getchar());
	if(c == '-') {
		neg = 1;
		c = getchar();
	}
	for(; c > 47 && c < 58; c = getchar()) {
		x = (x << 1) + (x << 3) + c - 48;
	}
	if(neg) x = -x;
	return x;
}

int a[N][N], c[N][N], dp[810][N]; 
// dp[i][j] = minimum cost for dividing prefix [1...j] into i parts

// Cost function: cost of segment [i, j]
// Can be O(1) after precomputation
int cost(int i, int j) {
	return c[j][j] - c[i - 1][j] - c[j][i - 1] + c[i - 1][i - 1];
}

// Recursive function to compute one layer of DP
// i: current partition count (layer)
// l, r: range of current DP states to compute (dp[i][l]...dp[i][r])
// optl, optr: range of possible optimal k values
void yo(int i, int l, int r, int optl, int optr) {
	if(l > r) return;
	
	int mid = (l + r) / 2;
	dp[i][mid] = inf; 
	int opt = -1;

    // Iterate through valid split points
	for(int k = optl; k <= min(mid - 1, optr); k++) {
		int val = dp[i - 1][k] + cost(k + 1, mid);
		if(val < dp[i][mid]) { 
			dp[i][mid] = val;
			opt = k;
		}
	}
    
    // If no valid k found (should not happen in valid range), fallback or handle
    if (opt == -1) opt = optl;

	// Solve for left and right halves
    // Optimization: opt[mid] bounds the search for left and right
    // for j < mid, opt[j] <= opt[mid]
    // for j > mid, opt[j] >= opt[mid]
	yo(i, l, mid - 1, optl, opt);
	yo(i, mid + 1, r, opt, optr);
}

int main() {
    // Use sc() for fast I/O if provided, otherwise cin/cout
	// ios_base::sync_with_stdio(0); cin.tie(0);

	int i, j, k, n;
	// Input handling
    // n = sc(); k = sc();
    // Example fallback:
    if (scanf("%d %d", &n, &k) != 2) return 0;

	for(i = 1; i <= n; i++) 
        for(j = 1; j <= n; j++) 
            a[i][j] = sc();

    // Precompute costs (2D Prefix Sums)
	for(i = 1; i <= n; i++) {
		for(j = 1; j <= n; j++) {
			c[i][j] = a[i][j] + c[i - 1][j] + c[i][j - 1] - c[i - 1][j - 1];
		}
	}

    // Initialize DP
    // Base case: 1 partition
	for(i = 1; i <= n; i++) dp[1][i] = cost(1, i);
    
    // Compute subsequent layers
	for(i = 2; i <= k; i++) yo(i, 1, n, 1, n);

	cout << dp[k][n] / 2 << endl; // Divide by 2 if cost matrix was symmetric/doubled
	return 0;
}

/**
 * Problem: Calculate minimum steps to sort a permutation using 4 different rules.
 * * Input: Permutation of numbers 1 to N.
 * Output: 4 integers representing costs for the 4 methods.
 * * Logic Breakdown:
 * 1. Adjacent Swaps -> Count Inversions (using Fenwick Tree).
 * 2. Any Swap -> Cycle Decomposition (N - Number of Cycles).
 * 3. Move Anywhere -> N - Longest Increasing Subsequence (LIS).
 * 4. Move to Front -> N - Length of suffix (k, k+1... n) already in order.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ==========================================
// Method 1 Helper: Fenwick Tree (BIT)
// ==========================================
// We use this to count inversions efficiently in O(N log N).
// An inversion is a pair (i, j) where i < j but arr[i] > arr[j].
// This corresponds to the number of swaps in Bubble Sort.
struct FenwickTree {
    int size;
    vector<int> tree;

    FenwickTree(int n) : size(n), tree(n + 1, 0) {}

    // Add 'val' to the element at 'idx'
    void add(int idx, int val) {
        for (; idx <= size; idx += idx & -idx)
            tree[idx] += val;
    }

    // Return the prefix sum up to 'idx'
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx)
            sum += tree[idx];
        return sum;
    }
};

// ==========================================
// Method 1: Adjacent Swaps
// ==========================================
long long solveAdjacentSwaps(int n, const vector<int>& a) {
    long long inversions = 0;
    FenwickTree ft(n);
    
    // We iterate through the array. For each element x:
    // 1. We count how many numbers larger than x we have seen so far? 
    //    Actually, it's easier to count how many we've seen total (i) 
    //    minus how many were smaller (ft.query(x)).
    //    The remainder is the count of numbers larger than x that appeared before x.
    for (int i = 0; i < n; ++i) {
        int val = a[i];
        int smaller_count = ft.query(val);
        int total_seen = i;
        
        inversions += (total_seen - smaller_count);
        
        // Mark this value as seen in the Fenwick Tree
        ft.add(val, 1);
    }
    return inversions;
}

// ==========================================
// Method 2: Any Two Elements Swap
// ==========================================
// Logic: If we view the permutation as a graph where i -> correct_pos[i],
// the graph forms disjoint cycles.
// To sort a cycle of size k, we need (k-1) swaps.
// Total Swaps = Sum(cycle_len - 1) for all cycles
//             = N - (Total Number of Cycles)
int solveAnySwaps(int n, const vector<int>& a) {
    int cycles = 0;
    vector<bool> visited(n + 1, false);
    
    // Note: Input 'a' is 0-indexed, but values are 1..N.
    // Value v belongs at index v-1.
    for (int i = 0; i < n; ++i) {
        if (visited[i]) continue;
        
        // Start tracing a new cycle
        cycles++;
        int curr = i;
        while (!visited[curr]) {
            visited[curr] = true;
            
            // Where should the element at 'curr' actually go?
            // The value is a[curr]. It belongs at index a[curr] - 1.
            int next_idx = a[curr] - 1;
            curr = next_idx;
        }
    }
    return n - cycles;
}

// ==========================================
// Method 3: Move Element to Another Position
// ==========================================
// Logic: We want to keep as many elements as possible untouched.
// The elements we DON'T move must already be in increasing order relative to each other.
// This is exactly the Longest Increasing Subsequence (LIS).
// Result = N - Length of LIS.
int solveMoveAnywhere(int n, const vector<int>& a) {
    vector<int> tails; // Helper for O(N log N) LIS
    
    for (int x : a) {
        // Find the first element in 'tails' that is >= x
        auto it = lower_bound(tails.begin(), tails.end(), x);
        
        if (it == tails.end()) {
            tails.push_back(x); // x extends the longest subsequence found so far
        } else {
            *it = x; // x replaces a larger ending element (standard LIS optimization)
        }
    }
    return n - tails.size();
}

// ==========================================
// Method 4: Move Element to Front
// ==========================================
// Logic: We are building the sorted array 1, 2, ..., N.
// Since we can only move to the FRONT, the elements we DO NOT move must be at the very end
// and must essentially form a suffix like k, k+1, ..., N.
// We look for the longest chain (k...N) that appears in the array in relative order.
// Result = N - Length of that chain.
int solveMoveToFront(int n, const vector<int>& a) {
    // Map value -> index to quickly check relative order
    vector<int> pos(n + 1);
    for (int i = 0; i < n; ++i) {
        pos[a[i]] = i;
    }
    
    int chain_len = 0;
    if (n > 0) {
        chain_len = 1; // We can always keep the number N
        int last_idx = pos[n];
        
        // Check N-1, then N-2, etc.
        for (int v = n - 1; v >= 1; --v) {
            // Does v appear BEFORE the element we just verified (v+1)?
            if (pos[v] < last_idx) {
                chain_len++;
                last_idx = pos[v];
            } else {
                // The chain is broken. 'v' appears after 'v+1', so 'v' must be moved.
                // Consequently, everything smaller than 'v' must also be moved.
                break;
            }
        }
    }
    return n - chain_len;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (cin >> n) {
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }

        cout << solveAdjacentSwaps(n, a) << " "
             << solveAnySwaps(n, a) << " "
             << solveMoveAnywhere(n, a) << " "
             << solveMoveToFront(n, a) << endl;
    }
    return 0;
}
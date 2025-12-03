/**
 * @struct mitm_sos
 * @brief Dynamic Sum Over Subsets with Square Root Decomposition and Lazy Memory allocation.
 * @time_complexity
 * - Insert/Erase: O(2^(N/2))
 * - Query: O(2^(N/2) + Buffer_Flush) roughly amortized O(2^(N/2))
 * @space_complexity O(Q * 2^(N/2)) worst case, where Q is number of queries.
 */
#include <bits/stdc++.h>
using namespace std ; 
 struct mitm_sos {
    int l_size, r_size; 
    
    // The main DP table. a[lower_mask][upper_mask]
    // Only allocated when needed to save memory.
    vector<vector<int>> a; 

    // Lazy buffer. push_val[lower_mask] stores pairs of {upper_mask, value_to_add}
    // pending to be added to the main table 'a'.
    vector<vector<pair<int, int>>> push_val; 

    /**
     * @brief Constructor
     * @param k Total number of bits (N).
     */
    mitm_sos(int k) {
        l_size = k / 2;             // Split point (e.g., if k=20, l_size=10)
        r_size = k - l_size;        // Remaining bits
        a.resize(1 << l_size);      // Resize rows (lower bits)
        push_val.resize(1 << l_size);
    }

    /**
     * @brief Adds a value to the set at index x.
     * Logic: We update SUPERMASKS of the lower bits immediately.
     * @param x The mask to insert.
     */
    void insert(int x) {
        // extract lower bits (row index)
        int left_part = x & ((1 << l_size) - 1); 
        // extract upper bits (column index)
        int right_part = ((x - left_part) >> l_size); 

        // Iterate over all SUPERMASKS of the lower bits
        for(int mask = 0; mask < (1 << l_size); mask++) {
            if((mask & left_part) == left_part) {
                // Store that this specific 'right_part' needs to be added to this row
                push_val[mask].push_back({right_part, 1});
            }
        }
    }

    /**
     * @brief Removes a value (inverse of insert).
     * @param x The mask to remove.
     */
    void erase(int x) {
        int left_part = x & ((1 << l_size) - 1);
        int right_part = ((x - left_part) >> l_size);
        
        for(int mask = 0; mask < (1 << l_size); mask++) {
            if((mask & left_part) == left_part) {
                push_val[mask].push_back({right_part, -1}); // Push -1
            }
        }
    }

    /**
     * @brief Calculates sum of all subsets of x.
     * Logic: We query SUBMASKS of the upper bits.
     * @param x The query mask.
     * @return Sum of values of all subsets of x.
     */
    int sos_sum(int x) {
        int ans = 0;
        int left_part = x & ((1 << l_size) - 1);
        int right_part = ((x - left_part) >> l_size);

        // 1. Memory Lazy Allocation: If this row is empty, resize it now.
        if(a[left_part].size() == 0) a[left_part].resize(1 << r_size);

        // 2. Flush Buffer: Apply all pending updates to this row.
        for(auto &p : push_val[left_part]) {
            a[left_part][p.first] += p.second;
        }
        push_val[left_part].clear(); // Clear buffer after flushing

        // 3. Query: Iterate over all SUBMASKS of the upper bits.
        // We look at the row 'left_part' (which contains sums for our lower bits)
        // and sum up the valid upper bit combinations.
        for(int mask = 0; mask < (1 << r_size); mask++) {
            if((mask & right_part) == mask) {
                ans += a[left_part][mask];
            }
        }
        return ans;
    }
};
/*
 * Problem it Solves: Max Convolution of two convex sequences.
 * Usage: multiply(a, b).
 * Input/Output: convex vectors a, b -> max convolution vector.
 * Time Complexity: O(N).
 * Tweaks: Min convolution (reverse logic).
 * Other Use Cases: DP optimization (Knapsack-like with convex functions).
 * Warnings: Input must be convex.
 */

#include <bits/stdc++.h>
using namespace std;

// a and b must be convex: a[i+1] - a[i] >= a[i] - a[i-1]
// compute ans[k] = max_{i+j=k} (a[i] + b[j])
vector<int> multiply(vector<int> a, vector<int> b) {
    int n = a.size() - 1, m = b.size() - 1;
    vector<int> ans(n + m + 1);
    int sum = a[0] + b[0]; ans[0] = sum;
    int l = 0, r = 0;
    while (l < n && r < m) {
        // Merge sorted slopes
        if (a[l + 1] - a[l] > b[r + 1] - b[r]) {
            sum += a[l + 1] - a[l];
            l++;
        } else {
            sum += b[r + 1] - b[r];
            r++;
        }
        ans[l + r] = sum;
    }
    while (l < n) {
        sum += a[l + 1] - a[l];
        l++;
        ans[l + r] = sum;
    }
    while (r < m) {
        sum += b[r + 1] - b[r];
        r++;
        ans[l + r] = sum;
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    vector<int> a({1, 2, 4, 8, 20});
    vector<int> b({4, 6, 10, 18});
    auto ans = multiply(a, b);
    for (int i = 0; i < ans.size(); i++) {
        cout << ans[i] << ' ';
    }
    return 0;
}

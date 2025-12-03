/*
 * Problem it Solves: Computes Inverse of a Matrix modulo 2 using bitset (Gauss-Jordan).
 * Usage: inverse(n, mat) returns inverse matrix.
 * Input/Output: n, bitset matrix -> bitset matrix.
 * Time Complexity: O(N^3 / 64).
 * Tweaks: N constant.
 * Other Use Cases: Linear Algebra over GF(2).
 * Warnings: Checks for singularity internally (maybe?).
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 2005;

bitset<N> z; // Zero bitset

vector<bitset<N>> inverse(int n, vector<bitset<N>> mat) {
    vector<bitset<N>> inv(n, z);
    for (int i = 0; i < n; i++) inv[i][i] = 1; // Identity matrix
    
    // Gauss-Jordan
    for (int i = 0; i < n; i++) {
        // Find pivot
        int pivot = -1;
        for (int j = i; j < n; j++) {
            if (mat[j][i]) {
                pivot = j;
                break;
            }
        }
        
        if (pivot == -1) return {}; // Singular matrix
        
        swap(mat[i], mat[pivot]);
        swap(inv[i], inv[pivot]);
        
        // Eliminate other rows
        for (int j = 0; j < n; j++) {
            if (i != j && mat[j][i]) {
                mat[j] ^= mat[i];
                inv[j] ^= inv[i];
            }
        }
    }
    return inv;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m; cin >> n >> m;
    vector<bitset<N>> mat(n);
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        --u; --v;
        mat[u][v] = 1;
        edges[i] = {u, v};
    }
    auto inv = inverse(n, mat);
    if (inv.empty()) {
        // Handle singular case
    }
    // Example usage check
    for (int i = 0; i < m; i++) {
        // if inv[b][a] is 1...
    }
    return 0;
}

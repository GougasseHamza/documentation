/*
 * Problem it Solves: Maintains a Linear Basis where we want to maximize the sum of weights of chosen basis vectors.
 * Usage: insert(vector, weight). query() returns max weight.
 * Input/Output: Vectors and weights -> Max weight sum.
 * Time Complexity: O(Bits) per insert.
 * Tweaks: B = 127 for __int128. Change to 63 for long long.
 * Other Use Cases: Matroid greedy problems (Linear Basis is a matroid).
 * Warnings: T needs to support bitwise operations.
 */

#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct Basis {
    static const int B = 127; // KEY: Number of bits. TWEAK: Set to 60-62 for long long.
    T a[B];
    long long wt[B]; // Weights associated with basis vectors

    Basis() {
        memset(a, 0, sizeof a);
        memset(wt, 0, sizeof wt);
    }

    // Greedy insertion: if we can insert x, we do it.
    // If x conflicts with existing basis vector a[i], but x has higher weight,
    // we replace a[i] with x and continue trying to insert the old a[i] (with lower weight).
    void insert(T x, long long w) {
        for (int i = B - 1; i >= 0; i--) {
            if (x >> i & 1) {
                if (a[i] == 0) {
                    a[i] = x;
                    wt[i] = w;
                    break;
                }
                // KEY: Greedy swap if new vector has higher weight
                if (wt[i] < w) {
                    swap(wt[i], w);
                    swap(a[i], x);
                }
                x ^= a[i];
            }
        }
    }

    // Maximum sum of weights of linearly independent vectors
    long long query() {
        long long ans = 0;
        for (int i = 0; i < B; i++) {
            ans += (wt[i]);
        }
        return ans;
    }
};

Basis<__int128> t; // TWEAK: Change to long long if input fits in 64 bits

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, q; cin >> n >> q;
    while (q--) {
        int u, v;
        long long x, w; cin >> u >> v >> x >> w;
        __int128 cur = x, b = 1;
        cur |= b << (62 + u);
        cur |= b << (62 + v);
        t.insert(cur, w);
        cout << t.query() << '\n';
    }
    return 0;
}

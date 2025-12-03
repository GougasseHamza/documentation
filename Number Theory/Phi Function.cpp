/*
 * Problem it Solves: Computes Euler's Totient Function phi(n) for all n <= N.
 * Usage: totient() precomputes phi array.
 * Input/Output: N/A.
 * Time Complexity: O(N log log N).
 * Tweaks: N size.
 * Other Use Cases: Modular arithmetic, group theory.
 * Warnings: 1-based indexing.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
int phi[N];

void totient() {
    for (int i = 1; i < N; i++) phi[i] = i;
    for (int i = 2; i < N; i++) {
        if (phi[i] == i) {
            for (int j = i; j < N; j += i) phi[j] -= phi[j] / i;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    totient();
    return 0;
}

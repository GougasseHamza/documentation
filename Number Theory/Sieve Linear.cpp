/*
 * Problem it Solves: Linear Sieve to find smallest prime factor (spf) and primes.
 * Usage: sieve() populates `spf` and `primes`.
 * Input/Output: N/A.
 * Time Complexity: O(N).
 * Tweaks: N size.
 * Other Use Cases: Fast factorization.
 * Warnings: Memory usage N integers.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

int spf[N];
vector<int> primes;

void sieve() {
    for (int i = 2; i < N; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
        }
        int sz = primes.size();
        for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    sieve();
    if (!primes.empty()) cout << primes.back() << '\n';
    return 0;
}

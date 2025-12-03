/*
 * Problem it Solves: Computes sum_{k=1}^n floor(n/k).
 * Usage: floor_sum(n) returns the sum.
 * Input/Output: n (long long) -> sum (long long).
 * Time Complexity: O(sqrt(n)).
 * Tweaks: None.
 * Other Use Cases: Divisor sum problems (Dirichlet hyperbola method).
 * Warnings: n must be positive.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// \sum{k=1}^{n}{floor(n/k)}
// count of numbers such that n/i = k -> n/k - n/(k+1)
ll floor_sum(ll n) {
    ll sum = 0;
    for (ll i = 1, last; i <= n; i = last + 1) {
        last = n / (n / i);
        sum += (n / i) * (last - i + 1);
    }
    return sum;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout << floor_sum(10) << '\n';
    return 0;
}

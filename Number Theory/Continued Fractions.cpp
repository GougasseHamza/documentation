/*
 * Problem it Solves: Finds continued fraction representation of p/q and its convergents.
 * Usage: get_frac(p, q) returns coefficients. convergents(a) returns convergents.
 * Input/Output: p, q (integers) -> vector of coefficients / pairs of convergents.
 * Time Complexity: O(log(min(p, q))) which is the Euclidean algorithm steps.
 * Tweaks: Use __int128 if numbers exceed long long range.
 * Other Use Cases: Best rational approximation, solving Pell's equation.
 * Warnings: p/q must be valid.
 */

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// returns the continued fraction of p / q
// [a[0]; a[1], ..., a[n]] = a[0] + 1 / (a[1] + 1 / (a[2] + ...) + 1 / (a[n - 1] + 1 / a[n]))))
vector<ll> get_frac(ll p, ll q) {
    vector<ll> a;
    while (q) {
        a.push_back(p / q);
        p %= q;
        swap(p, q);
    }
    return a;
}

// ans[k] = [a[0]; a[1], ..., a[k]] numerator/denominator
vector<pair<ll, ll>> convergents(const vector<ll>& a) {
    ll lp = 1, lq = 0;
    vector<pair<ll, ll>> ans({{a[0], 1}});
    for (size_t i = 1; i < a.size(); i++) {
        ll p = a[i] * ans.back().first + lp;
        ll q = a[i] * ans.back().second + lq;
        lp = ans.back().first;
        lq = ans.back().second;
        ans.push_back({p, q});
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ll i = 10, j = 23;
    auto a = get_frac(i, j);
    for (auto x : a) cout << x << ' '; cout << '\n';
    auto p = convergents(a);
    for (auto [x, y] : p) {
        cout << x << ' ' << y << '\n';
    }
    return 0;
}

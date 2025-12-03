/*
 * Problem it Solves: Computes Prime Basis of a set of numbers.
 * Usage: PrimeBasis<long long> B; B.add_element(x); B.factorize(x);
 * Input/Output: array of numbers -> valid/invalid representation.
 * Time Complexity: O(N^2 log A) to build basis.
 * Tweaks: Template type T.
 * Other Use Cases: Representing numbers as product of basis elements efficiently.
 * Warnings: N*loglogA basis size.
 */

#include <bits/stdc++.h>
using namespace std;

template<typename T>
T gcd(T a, T b) {
    return b == 0 ? a : gcd(b, a % b);
}

template<typename T>
struct PrimeBasis {
    void reduce_pair(T& x, T& y) {
        bool to_swap = 0;
        if (x > y) {
            to_swap ^= 1;
            swap(x, y);
        }
        while (x > 1 && y % x == 0) {
            y /= x;
            if (x > y) {
                to_swap ^= 1;
                swap(x, y);
            }
        }
        if (to_swap) swap(x, y);
    }
    
    vector<T> basis;
    
    void solve_inner(int pos, T &val) {
        while (basis[pos] % val == 0) basis[pos] /= val;
        vector<T> curr_basis = {basis[pos], val};
        int ptr = 1;
        while (ptr < curr_basis.size()) {
            for (int i = 0; i < ptr; i++) {
                reduce_pair(curr_basis[i], curr_basis[ptr]);
                if (curr_basis[ptr] == 1) break;
                if (curr_basis[i] == 1) break;
                T g = gcd(curr_basis[ptr], curr_basis[i]);
                if (g > 1) {
                    curr_basis[ptr] /= g;
                    curr_basis[i] /= g;
                    curr_basis.push_back(g);
                }
            }
            ++ptr;
        }
        basis[pos] = curr_basis[0];
        val = curr_basis[1];
        for (size_t i = 2; i < curr_basis.size(); i++)
            if (curr_basis[i] > 1) basis.push_back(curr_basis[i]);
        if (basis[pos] == 1) {
            swap(basis[pos], basis.back());
            basis.pop_back();
        }
    }
    
    void add_element(T val) {
        for (size_t i = 0; i < basis.size(); i++) {
            reduce_pair(val, basis[i]);
            if (basis[i] == 1) {
                swap(basis[i], basis.back());
                basis.pop_back();
                break;
            }
            if (val == 1) return;
            if (gcd(basis[i], val) > 1) solve_inner(i, val);
        }
        if (val > 1) basis.push_back(val);
    }
    
    vector<int> factorize(T x) {
        vector<int> factors(basis.size());
        for (size_t i = 0; i < basis.size(); i++) {
            while (x % basis[i] == 0) {
                factors[i]++;
                x /= basis[i];
            }
        }
        return factors;
    }
};

vector<int> lcm(vector<int> a, vector<int> b) {
    for (size_t i = 0; i < a.size(); i++) {
        a[i] = max(a[i], b[i]);
    }
    return a;
}

using ll = long long;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<ll> a(n);
    PrimeBasis<ll> B;
    for (int i = 0; i < n; i++) {
        ll x; cin >> x;
        B.add_element(x);
        a[i] = x;
    }
    vector<pair<vector<int>, ll>> v;
    for (int i = 0; i < n; i++) {
        v.push_back({B.factorize(a[i]), a[i]});
    }
    vector<ll> ans;
    while (!v.empty()) {
        bool found = false;
        for (size_t i = 0; i < v.size(); i++) {
            auto [fac, x] = v[i];
            vector<int> cur = vector<int>(B.basis.size(), 0);
            for (size_t j = 0; j < v.size(); j++) {
                if (i == j) continue;
                cur = lcm(cur, v[j].first);
            }
            if (cur != lcm(cur, fac)) {
                ans.push_back(x);
                v.erase(find(v.begin(), v.end(), v[i]));
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "No\n";
            return 0;
        }
    }
    reverse(ans.begin(), ans.end());
    cout << "Yes\n";
    for (auto x : ans) {
        cout << x << ' ';
    }
    cout << '\n';
    return 0;
}

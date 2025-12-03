/*
 * Problem it Solves: Factoradic Number System (Factorial Number System) operations.
 * Usage: kth_perm(n, k) gives the k-th lexicographical permutation. order(p) gives the rank.
 * Input/Output: n (int), k (int) -> vector<int> (permutation). vector<int> -> int (rank).
 * Time Complexity: O(N log N) using Policy Based Data Structures (Ordered Set).
 * Tweaks: Requires PBDS headers.
 * Other Use Cases: Ranking and Unranking permutations.
 * Warnings: k can be large, fitting in int might be insufficient (use long long if needed).
 */

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

template <typename T> using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

vector<int> decimal_to_factoradic(int n) {
    vector<int> v;
    for (int i = 1; n; i++, n /= i) {
        v.push_back(n % i);
    }
    reverse(v.begin(), v.end());
    return v;
}

int factoradic_to_decimal(vector<int> v) {
    int n = v.size();
    int ans = 0;
    for (int i = 0, mul = n; i < n; i++, mul--) {
        ans = ans * mul + v[i];
    }
    return ans;
}

vector<int> get_perm(int n, vector<int> f) {
    o_set<int> se;
    int sz = f.size();
    vector<int> p;
    for (int i = 0; i < n - sz; i++) p.push_back(i);
    for (int i = n - sz; i < n; i++) se.insert(i);
    for (int i = 0; i < sz; i++) {
        int nw = *se.find_by_order(f[i]);
        p.push_back(nw);
        se.erase(nw);
    }
    return p;
}

// returns the k-th lexicographically smallest permutation of size n
// 0-th permutation is the unit permutation i.e. 0, 1, 2, .... n-1
vector<int> kth_perm(int n, int k) { // k < n!
    return get_perm(n, decimal_to_factoradic(k));
}

vector<int> factoradic_order(vector<int> p) {
    o_set<int> se;
    int n = p.size();
    for (int i = 0; i < n; i++) se.insert(p[i]);
    vector<int> fac;
    for (int i = 0; i < n; i++) {
        int x = se.order_of_key(p[i]);
        fac.push_back(x);
        se.erase(p[i]);
    }
    return fac;
}

// ?-th lexicographically smallest permutation of size n
int order(vector<int> p) {
    return factoradic_to_decimal(factoradic_order(p));
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; i++) cin >> p[i];
    cout << order(p)<< endl; 
    return 0;
}

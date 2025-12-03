/*
 * Problem it Solves: Computes k-th term of linear recurrence S[n] = sum(C[i]*S[n-i-1]).
 * Usage: LinearRecurrence(S, tr, k).
 * Input/Output: S (initial), tr (coefficients), k (index) -> value.
 * Time Complexity: O(K^2 log K). Naive multiplication.
 * Tweaks: MOD constant.
 * Other Use Cases: Fibonacci, sequences.
 * Warnings: S and tr must be same size.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 1e9 + 7;

vector<int> combine(int n, vector<int> &a, vector<int> &b, vector<int> &tr) {
    vector<int> res(n * 2 + 1, 0);
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            res[i + j] = (res[i + j] + 1LL * a[i] * b[j]) % MOD;
        }
    }
    for (int i = 2 * n; i > n; --i) {
        for (int j = 0; j < n; j++) {
            res[i - 1 - j] = (res[i - 1 - j] + 1LL * res[i] * tr[j]) % MOD;
        }
    }
    res.resize(n + 1);
    return res;
}

// transition -> for(i = 0; i < x; i++) f[n] += tr[i] * f[n-i-1]
// S contains initial values, k is 0 indexed
int LinearRecurrence(vector<int> &S, vector<int> &tr, long long k) {
    int n = S.size(); assert(n == (int)tr.size());
    if (n == 0) return 0;
    if (k < n) return S[k];
    
    vector<int> pol(n + 1), e(pol);
    pol[0] = e[1] = 1; // pol = 1, e = x
    
    for (++k; k; k /= 2) {
        if (k % 2) pol = combine(n, pol, e, tr);
        e = combine(n, e, e, tr);
    }
    
    int res = 0;
    for (int i = 0; i < n; i++) res = (res + 1LL * pol[i + 1] * S[i]) % MOD;
    return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> S(n); for (int i = 0; i < n; i++) cin >> S[i];
        vector<int> tr(n); for (int i = 0; i < n; i++) cin >> tr[i];
        int k; cin >> k; --k;
        cout << LinearRecurrence(S, tr, k) << '\n';
    }
    return 0;
}

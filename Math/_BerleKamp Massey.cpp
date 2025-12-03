/*
 * Problem it Solves: Finds the shortest linear recurrence (Berlekamp-Massey) for a sequence and computes the k-th term.
 * Usage: BerlekampMassey(S) returns recurrence coeffs. LinearRecurrence(S, tr, k) returns k-th term.
 * Input/Output: Sequence S -> recurrence vector.
 * Time Complexity: BM is O(N^2), LinearRecurrence is O(N^2 log K) here.
 * Tweaks: MOD constant. Use FFT based polynomial multiplication in `combine` for O(N log N log K).
 * Other Use Cases: Guessing formulas from output values.
 * Warnings: MOD must be prime.
 */

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
using ll = long long;

int power(long long base, long long exp) {
    int res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (1LL * res * base) % MOD;
        base = (1LL * base * base) % MOD;
        exp /= 2;
    }
    return res;
}

int modInverse(int n) {
    return power(n, MOD - 2);
}

// Returns the coefficients C of the recurrence relation:
// S[i] = sum(C[j] * S[i-j])
// KEY: Berlekamp-Massey Algorithm
vector<int> BerlekampMassey(vector<int> S) {
    int n = (int)S.size(), L = 0, m = 0;
    vector<int> C(n), B(n), T;
    C[0] = B[0] = 1;
    int b = 1;
    for (int i = 0; i < n; i++) {
        ++m;
        int d = S[i];
        for (int j = 1; j <= L; j++) 
            d = (d + 1LL * C[j] * S[i - j]) % MOD;
        
        if (d == 0) continue;
        
        T = C;
        int coef = 1LL * d * modInverse(b) % MOD;
        
        for (int j = m; j < n; j++) 
            C[j] = (C[j] - 1LL * coef * B[j - m] % MOD + MOD) % MOD;
            
        if (2 * L > i) continue;
        
        L = i + 1 - L;
        B = T;
        b = d;
        m = 0;
    }
    C.resize(L + 1);
    C.erase(C.begin());
    for (auto &x : C) x = (MOD - x) % MOD; // Adjust sign: S[i] = sum(C[j]*S[i-j])
    return C;
}

// Multiplies two polynomials modulo the characteristic polynomial (defined by tr)
// O(N^2)
vector<int> combine(int n, vector<int> &a, vector<int> &b, vector<int> &tr) {
    vector<int> res(n * 2 + 1, 0);
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            res[i + j] = (res[i + j] + 1LL * a[i] * b[j]) % MOD;
        }
    }
    // Modulo by the recurrence relation (characteristic poly)
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
// KEY: Computes x^k mod P(x) where P(x) is characteristic poly. Result is sum(pol[i+1] * S[i])
int LinearRecurrence(vector<int> &S, vector<int> &tr, long long k) {
    int n = S.size();
    if (n == 0) return 0;
    if (k < n) return S[k];
    
    vector<int> pol(n + 1), e(pol);
    pol[0] = e[1] = 1; // pol = 1 (unity), e = x (variable)
    
    // Binary exponentiation for polynomials
    for (++k; k; k /= 2) {
        if (k % 2) pol = combine(n, pol, e, tr);
        e = combine(n, e, e, tr);
    }
    
    int res = 0;
    for (int i = 0; i < n; i++) res = (res + 1LL * pol[i + 1] * S[i]) % MOD;
    return res;
}

int prime[] = {2, 3, 5, 7, 11, 13, 17, 19}, ok[20];
int dp[2000][20];

int yo(int i, int last) {
    if (i == 0) return 1;
    int &ret = dp[i][last];
    if (ret != -1) return ret;
    ret = 0;
    for (int k = 1; k <= 9; k++) {
        if (!last || ok[last + k]) ret = (ret + yo(i - 1, k)) % MOD;
    }
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    memset(dp, -1, sizeof dp);
    for (int i = 0; i < 8; i++) ok[prime[i]] = 1;
    
    vector<int> S; 
    S.push_back(4); 
    int sum = 4;
    for (int i = 2; i < 100; i++) {
        sum = (sum + yo(i, 0)) % MOD;
        S.push_back(sum);
    }
    
    auto tr = BerlekampMassey(S);
    S.resize((int)tr.size());
    
    int q; cin >> q;
    while (q--) {
        int n; cin >> n; --n;
        cout << LinearRecurrence(S, tr, n) << '\n';
    }
    return 0;
}

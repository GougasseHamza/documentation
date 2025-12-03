/*
 * Problem it Solves: Standard Fast Walsh-Hadamard Transform (OR, AND, XOR convolutions).
 * Usage: t.multiply(n, A, B, flag) returns convolution.
 * Input/Output: n (size, power of 2), A, B vectors.
 * Time Complexity: O(N log N).
 * Tweaks: M size.
 * Other Use Cases: Subset sum problems (OR), XOR basis problems.
 * Warnings: N must be power of 2.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, MOD = 1e9 + 7;

int power(long long n, long long k) {
    int ans = 1 % MOD; n %= MOD; if (n < 0) n += MOD;
    while (k) {
        if (k & 1) ans = (long long) ans * n % MOD;
        n = (long long) n * n % MOD;
        k >>= 1;
    }
    return ans;
}

const int inv2 = (MOD + 1) >> 1;
#define M (1 << 20)
#define OR 0
#define AND 1
#define XOR 2

struct FWHT {
    int P1[M], P2[M];
    
    void wt(int *a, int n, int flag = XOR) {
        if (n == 0) return;
        int m = n / 2;
        wt(a, m, flag); wt(a + m, m, flag);
        for (int i = 0; i < m; i++) {
            int x = a[i], y = a[i + m];
            if (flag == OR) a[i] = x, a[i + m] = (x + y) % MOD;
            if (flag == AND) a[i] = (x + y) % MOD, a[i + m] = y;
            if (flag == XOR) a[i] = (x + y) % MOD, a[i + m] = (x - y + MOD) % MOD;
        }
    }
    
    void iwt(int* a, int n, int flag = XOR) {
        if (n == 0) return;
        int m = n / 2;
        iwt(a, m, flag); iwt(a + m, m, flag);
        for (int i = 0; i < m; i++) {
            int x = a[i], y = a[i + m];
            if (flag == OR) a[i] = x, a[i + m] = (y - x + MOD) % MOD;
            if (flag == AND) a[i] = (x - y + MOD) % MOD, a[i + m] = y;
            if (flag == XOR) a[i] = 1LL * (x + y) * inv2 % MOD, a[i + m] = 1LL * (x - y + MOD) * inv2 % MOD;
        }
    }
    
    vector<int> multiply(int n, vector<int> A, vector<int> B, int flag = XOR) {
        assert(__builtin_popcount(n) == 1); // Ensure power of 2
        A.resize(n); B.resize(n);
        for (int i = 0; i < n; i++) P1[i] = A[i];
        for (int i = 0; i < n; i++) P2[i] = B[i];
        wt(P1, n, flag); wt(P2, n, flag);
        for (int i = 0; i < n; i++) P1[i] = 1LL * P1[i] * P2[i] % MOD;
        iwt(P1, n, flag);
        return vector<int>(P1, P1 + n);
    }
    
    vector<int> pow(int n, vector<int> A, long long k, int flag = XOR) {
        assert(__builtin_popcount(n) == 1);
        A.resize(n);
        for (int i = 0; i < n; i++) P1[i] = A[i];
        wt(P1, n, flag);
        for(int i = 0; i < n; i++) P1[i] = power(P1[i], k);
        iwt(P1, n, flag);
        return vector<int>(P1, P1 + n);
    }
} t;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<int> a(M, 0);
    for(int i = 0; i < n; i++) {
        int k; cin >> k; a[k]++;
    }
    vector<int> v = t.pow(M, a, n, AND);
    int ans = 1;
    for(int i = 1; i < M; i++) ans += v[i] > 0;
    cout << ans << '\n';
    return 0;
}

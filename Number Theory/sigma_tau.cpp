#include <bits/stdc++.h>
using namespace std;

// ===================== Case 1: Precompute for all 1..N (O(N log N)) =====================
const int N = 1e6; // adjust if needed
vector<int> d(N+1, 0);          // number of divisors
vector<long long> sigma(N+1, 0);// sum of divisors

void precompute_divisors() {
    for (int i = 1; i <= N; i++) {
        for (int j = i; j <= N; j += i) {
            d[j]++;         // count divisors
            sigma[j] += i;  // sum of divisors
        }
    }
}

// ===================== Case 2: SPF sieve for factorization up to 1e12 =====================
const int MAXN = 1e6; // sqrt(1e12)
int spf[MAXN+1];

void build_spf() {
    for (int i = 2; i <= MAXN; i++) {
        if (!spf[i]) {
            spf[i] = i;
            if ((long long)i * i <= MAXN) {
                for (long long j = 1LL * i * i; j <= MAXN; j += i) {
                    if (!spf[j]) spf[j] = i;
                }
            }
        }
    }
}

vector<pair<long long,int>> factorize(long long n) {
    vector<pair<long long,int>> f;
    while (n > 1 && n <= MAXN && spf[n]) {
        int p = spf[n], cnt = 0;
        while (n % p == 0) n /= p, cnt++;
        f.push_back({p,cnt});
    }
    if (n > 1) f.push_back({n,1}); // leftover prime
    return f;
}

long long num_divisors(long long n) {
    auto f = factorize(n);
    long long res = 1;
    for (auto [p,a] : f) res *= (a+1);
    return res;
}

long long sum_divisors(long long n) {
    auto f = factorize(n);
    long long res = 1;
    for (auto [p,a] : f) {
        long long term = 1, pw = 1;
        for (int i=0; i<a; i++) pw *= p, term += pw;
        res *= term;
    }
    return res;
}

// ===================== Case 3: One-off query, trial division =====================
vector<pair<long long,int>> factorize_single(long long n) {
    vector<pair<long long,int>> f;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) n /= p, cnt++;
            f.push_back({p, cnt});
        }
    }
    if (n > 1) f.push_back({n, 1});
    return f;
}

long long num_divisors_single(long long n) {
    auto f = factorize_single(n);
    long long res = 1;
    for (auto [p,a] : f) res *= (a+1);
    return res;
}

long long sum_divisors_single(long long n) {
    auto f = factorize_single(n);
    long long res = 1;
    for (auto [p,a] : f) {
        long long term = 1, pw = 1;
        for (int i=0; i<a; i++) pw *= p, term += pw;
        res *= term;
    }
    return res;
}


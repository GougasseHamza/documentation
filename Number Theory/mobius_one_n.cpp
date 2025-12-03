#include <bits/stdc++.h>
using namespace std;

vector<int> primes;

void sieve_primes(int limit = 31622) { // sqrt(1e9)
    vector<bool> is_prime(limit+1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= limit; i++) if (is_prime[i]) {
        for (int j = i * i; j <= limit; j += i) is_prime[j] = false;
    }
    for (int i = 2; i <= limit; i++) if (is_prime[i]) primes.push_back(i);
}

// Compute Möbius μ(n)
int mobius(long long n) {
    int mu = 1;
    for (int p : primes) {
        if (1LL * p * p > n) break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0; // squared prime factor
            mu = -mu;                 // distinct prime
        }
        while (n % p == 0) n /= p;
    }
    if (n > 1) mu = -mu; // leftover prime factor
    return mu;
}


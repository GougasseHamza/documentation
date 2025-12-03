#include <bits/stdc++.h>
using namespace std;

//factor from miller rabin +pollarh
// ---------------------- Sum of Squares ----------------------

// Fermat's 2-square theorem: check if n is sum of 2 squares
bool can_two_squares(long long n) {
    auto f = factor(n);
    sort(f.begin(), f.end());
    for (size_t i=0;i<f.size();) {
        int cnt=0; u64 p=f[i];
        while(i<f.size() && f[i]==p) i++,cnt++;
        if (p%4==3 && (cnt%2==1)) return false;
    }
    return true;
}

// Naive search to construct 2-squares representation if exists
pair<long long,long long> two_squares(long long n) {
    for (long long a=0; 1LL*a*a<=n; a++) {
        long long b2=n-1LL*a*a;
        long long b=sqrtl(b2);
        if (b*b==b2) return {a,b};
    }
    return {-1,-1}; // should not happen if check passes
}

// Legendre's 3-square theorem: check if n is sum of 3 squares
bool can_three_squares(long long n) {
    while (n%4==0) n/=4;
    return (n%8 != 7);
}

// Construct 3-squares representation (simple search, not optimal)
tuple<long long,long long,long long> three_squares(long long n) {
    for (long long a=0; 1LL*a*a<=n; a++) {
        for (long long b=0; 1LL*b*b<=n-1LL*a*a; b++) {
            long long c2=n-1LL*a*a-1LL*b*b;
            long long c=sqrtl(c2);
            if (c>=0 && c*c==c2) return {a,b,c};
        }
    }
    return {-1,-1,-1}; // should not happen if check passes
}

// Lagrange: always possible with 4 squares
array<long long,4> four_squares(long long n) {
    // reduce to 3-squares case by brute force on one square
    for (long long a=0; 1LL*a*a<=n; a++) {
        long long m=n-1LL*a*a;
        if (can_three_squares(m)) {
            auto [b,c,d]=three_squares(m);
            return {a,b,c,d};
        }
    }
    return {-1,-1,-1,-1}; // never happens
}


#include<bits/stdc++.h>
using namespace std;

const int N = 1010, mod = 1e9 + 7;

template <const int32_t MOD>
struct modint {
  int32_t value;
  modint() = default;
  modint(int32_t value_) : value(value_) {}
  inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
  inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c <    0 ? c + MOD : c); }
  inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
  inline modint<MOD> & operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
  inline modint<MOD> & operator -= (modint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
  inline modint<MOD> & operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
  inline modint<MOD> operator - () const { return modint<MOD>(this->value ? MOD - this->value : 0); }
  modint<MOD> pow(uint64_t k) const { modint<MOD> x = *this, y = 1; for (; k; k >>= 1) { if (k & 1) y *= x; x *= x; } return y; }
  modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
  inline modint<MOD> operator /  (modint<MOD> other) const { return *this *  other.inv(); }
  inline modint<MOD> operator /= (modint<MOD> other)       { return *this *= other.inv(); }
  inline bool operator == (modint<MOD> other) const { return value == other.value; }
  inline bool operator != (modint<MOD> other) const { return value != other.value; }
  inline bool operator < (modint<MOD> other) const { return value < other.value; }
  inline bool operator > (modint<MOD> other) const { return value > other.value; }
};
template <int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <int32_t MOD> istream & operator >> (istream & in, modint<MOD> &n) { return in >> n.value; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;
/**
 * Segment Tree with Arithmetic Progression Updates
 *
 * * PROBLEMS SOLVED:
 * 1. Range Add AP: Add {a, a+d, a+2d...} to A[L...R].
 * 2. Range Sum Query.
 * 3. Useful for problems involving polynomial updates (since quadratic can be decomposed into APs of linear functions).
 *
 * * COMPLEXITY:
 * Time: O(log N) per update/query.
 * Space: O(N).
 *
 * * LOGIC:
 * - Lazy tag stores {a, d} representing the AP to be added.
 * - When merging lazy tags (push), we simply add the 'a' and 'd' components (vector addition).
 * - When applying lazy to a range of length Len, we add the sum of the AP series to the node's total.
 */

 #include<bits/stdc++.h>
 using namespace std;
 
 
 // (ModInt struct omitted for brevity - same as before)
 using mint = modint<mod>;
 
 struct ST {
   struct Int { // Represents an AP: a, a+d, a+2d ...
     mint a = 0, d = 0;
     Int() {};
     Int(mint _a, mint _d) {a = _a, d = _d;}
     
     // Get the term at index n (1-based relative to start of this AP)
     mint at(mint n) { return (a + (n - 1) * d); }
     
     // Return a new AP shifted by n positions
     Int shift(mint n) { return Int((a + (n - 1) * d), d); }
     
     // Sum of first n terms
     mint get_sum(mint n) { return (((a * 2) + (n - 1) * d) * n) * ((mod + 1) / 2); }
     
     // Combine two APs (Vector addition)
     const Int operator + (const Int &b) const {
       return Int(a + b.a, d + b.d);
     }
   };
 
   mint t[N << 2];
   Int lazy[N << 2]; // Lazy tags storing APs
 
   ST() {
     memset(t, 0, sizeof t);
     // memset doesn't work for non-trivial structs, rely on build/lazy reset
   }
 
   void push(int n, int b, int e) {
     if (lazy[n].a == 0 && lazy[n].d == 0) return;
     
     // Update current node's sum
     t[n] += lazy[n].get_sum(e - b + 1);
     
     if (b != e) {
       int mid = (b + e) >> 1;
       // Left child inherits AP starting at 1
       lazy[n << 1] = lazy[n << 1] + lazy[n];
       
       // Right child inherits AP starting at (mid - b + 1) + 1
       // Distance from b to mid+1 is (mid + 1 - b). So it's the (mid - b + 2)-th term.
       lazy[n << 1 | 1] = lazy[n << 1 | 1] + lazy[n].shift(mid - b + 2);
     }
     lazy[n] = Int(0, 0);
   }
 
   void build(int n, int b, int e) {
     lazy[n] = Int(0, 0);
     if (b == e) { t[n] = 0; return; }
     int m = b + e >> 1;
     build(n << 1, b, m);
     build(n << 1 | 1, m + 1, e);
     t[n] = t[n << 1] + t[n << 1 | 1];
   }
 
   // Update range [i, j] with AP starting at 'v.first' with diff 'v.second'
   void upd(int n, int b, int e, int i, int j, pair<mint, mint> v) {
     push(n, b, e);
     if (b > j || e < i) return;
     if (i <= b && e <= j) {
       // We need the AP term corresponding to the start of this node's range relative to 'i'.
       // Start of this node is 'b'. Distance from 'i' is (b - i + 1).
       Int temp(v.first, v.second);
       lazy[n] = lazy[n] + temp.shift(b - i + 1);
       push(n, b, e);
       return;
     }
     int m = b + e >> 1;
     upd(n << 1, b, m, i, j, v);
     upd(n << 1 | 1, m + 1, e, i, j, v);
     t[n] = t[n << 1] + t[n << 1 | 1];
   }
 
   mint query(int n, int b, int e, int i, int j) {
     push(n, b, e);
     if (e < i || b > j) return 0;
     if (i <= b && e <= j) return t[n];
     int m = b + e >> 1;
     return query(n << 1, b, m, i, j) + query(n << 1 | 1, m + 1, e, i, j);
   }
 };
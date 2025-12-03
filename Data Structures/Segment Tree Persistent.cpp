/**
 * Persistent Segment Tree (Standard)
 * * PROBLEM: K-th smallest number in range A[L...R].
 * * LOGIC:
 * - Build a SegTree of Value Counts.
 * - root[i] = SegTree containing counts of values in A[1...i].
 * - Count in A[L...R] = count in root[R] - count in root[L-1].
 * - Walk down the tree: if LeftChild has >= k elements (net count), go Left. Else go Right.
 */
 #include<bits/stdc++.h>
 using namespace std;
 
 const int N = 1e5 + 9;
 
 struct PST {
 #define lc t[cur].l
 #define rc t[cur].r
   struct node { int l, r, val; } t[20 * N];
   int T = 0;
 
   int build(int b, int e) {
     int cur = ++T;
     if(b == e) return cur;
     int mid = b + e >> 1;
     lc = build(b, mid); rc = build(mid + 1, e);
     return cur;
   }
 
   int upd(int pre, int b, int e, int i, int v) {
     int cur = ++T; t[cur] = t[pre];
     if(b == e) { t[cur].val += v; return cur; }
     int mid = b + e >> 1;
     if(i <= mid) lc = upd(t[pre].l, b, mid, i, v);
     else rc = upd(t[pre].r, mid + 1, e, i, v);
     t[cur].val = t[lc].val + t[rc].val;
     return cur;
   }
 
   int query(int pre, int cur, int b, int e, int k) {
     if(b == e) return b;
     // Net count in left child for the range [L, R]
     int cnt = t[lc].val - t[t[pre].l].val;
     int mid = b + e >> 1;
     if(cnt >= k) return query(t[pre].l, lc, b, mid, k);
     else return query(t[pre].r, rc, mid + 1, e, k - cnt);
   }
 } t;
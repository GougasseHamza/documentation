/**
 * Persistent Li Chao Tree
 * Fully persistent version - can query any past version
 * Useful for tree DP problems where you need ancestor versions
 * 
 * Usage:
 *   PersistentLiChaoTree tree(min_x, max_x);
 *   int v0 = 0;  // Initial version (empty tree)
 *   int v1 = tree.add_line(v0, {m, c});  // Returns new version index
 *   int v2 = tree.add_line(v1, {m2, c2});
 *   ll ans = tree.query(v1, x);  // Query version v1 at point x
 * 
 * Complexity: O(log C) per update/query, O(Q log C) space
 */

#include<bits/stdc++.h>
using namespace std;

#define ll long long

const ll INF = 2e18;

struct Line {
    ll k, d;
    ll eval(ll x) { return k * x + d; }
};

struct LiChaoNode {
    Line line;
    int l, r;
    LiChaoNode() {
        line = {0, INF};
        l = 0; r = 0;
    }
    LiChaoNode(Line line) : line(line), l(0), r(0) {}
} t[6000005]; // ~60 nodes per query * 10^5 queries

int T = 0;

int upd(int pre, Line nw, ll l, ll r) {
    int id = ++T;
    t[id] = t[pre]; // Copy previous node
    
    ll m = l + (r - l) / 2;
    
    bool lef = nw.eval(l) < t[id].line.eval(l);
    bool mid = nw.eval(m) < t[id].line.eval(m);
    
    if (mid) swap(t[id].line, nw);
    
    if (l == r) return id;
    
    if (lef != mid) {
        t[id].l = upd(t[pre].l, nw, l, m);
    } else {
        t[id].r = upd(t[pre].r, nw, m + 1, r);
    }
    return id;
}

ll Query(int cur, ll x, ll l, ll r) {
    if (!cur) return INF;
    ll val = t[cur].line.eval(x);
    ll m = l + (r - l) / 2;
    if (l < r) {
        if (x <= m) val = min(val, Query(t[cur].l, x, l, m));
        else val = min(val, Query(t[cur].r, x, m + 1, r));
    }
    return val;
}

struct PersistentLiChaoTree {
    ll L, R;
    vector<int> roots;
    
    PersistentLiChaoTree() {}
    PersistentLiChaoTree(ll L, ll R) : L(L), R(R) {
        roots = {0}; // Version 0 is empty tree
    }
    
    // Add line to version p_ver, returns new version index
    int add_line(int p_ver, ll k, ll d) {
        return add_line(p_ver, {k, d});
    }
    
    int add_line(int p_ver, Line line) {
        int old_root = roots[p_ver];
        int new_root = upd(old_root, line, L, R);
        roots.push_back(new_root);
        return roots.size() - 1;
    }
    
    // Query version p_ver at point x
    ll query(int p_ver, ll x) {
        return Query(roots[p_ver], x, L, R);
    }
};

/**
 * Example Usage:
 * 
 * // Basic usage
 * PersistentLiChaoTree tree(-1e9, 1e9);
 * int v0 = 0;  // Initial empty version
 * int v1 = tree.add_line(v0, 2, 5);      // Add y = 2x + 5
 * int v2 = tree.add_line(v1, -1, 10);   // Add y = -x + 10
 * 
 * ll ans1 = tree.query(v1, 5);  // Query version 1 at x=5
 * ll ans2 = tree.query(v2, 5);  // Query version 2 at x=5
 * 
 * // Tree DP example
 * vector<vector<int>> g;
 * vector<ll> dp, s, p;
 * 
 * void dfs(int u, int par, PersistentLiChaoTree& tree, int ver) {
 *     if (u != 1) {
 *         dp[u] = tree.query(ver, p[u]) + s[u];
 *     }
 *     int new_ver = tree.add_line(ver, -s[u], dp[u]);
 *     for (int v : g[u]) {
 *         if (v != par) {
 *             dfs(v, u, tree, new_ver);
 *         }
 *     }
 * }
 */
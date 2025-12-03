/*
 * Problem it Solves: Solves system of linear equations modulo 2 (XOR) using bitset.
 * Usage: Gauss(n, m, A, ans).
 * Input/Output: n equations, m variables, Augmented bitset.
 * Time Complexity: O(N * M * N / 64).
 * Tweaks: N constant.
 * Other Use Cases: XOR basis, Lights Out puzzles.
 * Warnings: Bitset size N must be known at compile time.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 2010;

// n = number of equations, m = number of variables
int Gauss(int n, int m, vector<bitset<N>> a, bitset<N> &ans) {
    // reversing for lexicographically largest solution? 
    // Standard Gauss doesn't need reverse unless specific requirement.
    // Keeping logic as is.
    for (int i = 0; i < n; i++) {
        bitset<N> tmp;
        for (int j = 0; j < m; j++) tmp[j] = a[i][m - j - 1];
        tmp[m] = a[i][m];
        a[i] = tmp;
    }
    
    int rank = 0;
    vector<int> pos(N, -1);
    
    int row = 0;
    for (int col = 0; col < m && row < n; ++col) {
        int mx = row;
        for (int i = row; i < n; ++i) if (a[i][col]) { mx = i; break; }
        if (!a[mx][col]) continue;
        
        swap(a[mx], a[row]);
        pos[col] = row;
        
        for (int i = row + 1; i < n; ++i) if (i != row && a[i][col]) a[i] ^= a[row];
        ++row, ++rank;
    }
    
    ans.reset();
    // backward substitution
    for (int i = m - 1; i >= 0; i--) {
        if (pos[i] == -1) ans[i] = true; // Free variable, set to 1 for max/min logic?
        else {
            int k = pos[i];
            for (int j = i + 1; j < m; j++) if (a[k][j]) ans[i] = ans[i] ^ ans[j];
            ans[i] = ans[i] ^ a[k][m];
        }
    }
    
    for (int i = rank; i < n; ++i) if (a[i][m]) return -1; // no solution
    
    // reversing back
    bitset<N> tmp;
    for (int j = 0; j < m; j++) tmp[j] = ans[m - j - 1];
    ans = tmp;
    
    int free_var = 0;
    for (int i = 0; i < m; ++i) if (pos[i] == -1) free_var++;
    return free_var;
}

string read() {
    string t;
    if (!(cin >> t)) return "";
    if (t.empty() || t == "and") return "";
    while (t[0] == '(') t.erase(t.begin());
    while (t.back() == ')') t.pop_back();
    return t;
}
bool is_var(string t) { return t.size() > 0 && t[0] == 'x'; }
int get_var(string t) { return atoi(t.substr(1).c_str()) - 1; }

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m; cin >> n >> m;
    vector<bitset<N>> bs(n, bitset<N>(0));
    for (int i = 0; i < n; i++) {
        string s;
        bool eq = 1;
        while ((s = read()).size() > 0) {
            if (is_var(s)) {
                int x = get_var(s);
                bs[i][x] = bs[i][x] ^ 1;
            } else if (s == "not") eq ^= 1;
        }
        bs[i][m] = eq;
    }
    bitset<N> ans;
    int ok = Gauss(n, m, bs, ans);
    if (ok == -1) cout << "impossible\n";
    else {
        for (int i = 0; i < m; i++) cout << "FT"[ans[i]]; cout << '\n';
    }
    return 0;
}

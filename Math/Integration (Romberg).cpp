/*
 * Problem it Solves: Numerical Integration using Romberg's Method (Richardson extrapolation).
 * Usage: integrate(l, r) returns area.
 * Input/Output: range [l, r] -> area.
 * Time Complexity: Adaptive.
 * Tweaks: eps, f(x).
 * Other Use Cases: Smooth functions integration.
 * Warnings: Precision limited by double.
 */

#include <bits/stdc++.h>
using namespace std;

#define double long double
const double eps = 1e-6;

int z, d;
double f(double x) {
    // Example function:
    double t1 = sqrt(x * x + z * z);
    double t2 = sqrt((x + d) * (x + d) + z * z);
    double w = (t2 - t1) + (d - (t2 - t1)) / 2.0;
    double ans = w * w * 0.5 + t1 * w;
    ans += (d - w) * (d - w) * 0.5 + t2 * (d - w);
    ans /= d;
    return ans;
}

double integrate(double l, double r) {
    vector<double> t;
    double h = r - l;
    double last, curr;
    int k = 1;
    int i = 1;
    t.push_back(h * (f(l) + f(r)) / 2);
    
    while (true) {
        last = t.back();
        curr = 0;
        double x = l + h / 2;
        for (int j = 0; j < k; j++) {
            curr += f(x);
            x += h;
        }
        curr = (t[0] + h * curr) / 2;
        
        double k1 = 4.0 / 3.0, k2 = 1.0 / 3.0;
        for (int j = 0; j < i; j++) {
            double temp = k1 * curr - k2 * t[j];
            t[j] = curr;
            curr = temp;
            k2 /= 4 * k1 - k2;
            k1 = k2 + 1;
        }
        t.push_back(curr);
        k *= 2; h /= 2; i++;
        if (fabs(last - curr) < eps) break;
    }
    return t.back();
}

int main() {
    int t_cases, cs = 0; cin >> t_cases;
    while (t_cases--) {
        int r, l; cin >> z >> r >> l >> d;
        r -= d;
        double ans;
        if (l == r) ans = f(l);
        else ans = integrate(l, r) / (r - l);
        cout << "Case " << ++cs << ": " << fixed << setprecision(10) << ans << '\n';
    }
    return 0;
}

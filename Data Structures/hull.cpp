#include <iostream>
#include <vector>

using namespace std;

typedef long long coord_t;

struct Point {
    coord_t x, y;
};

class LowerHull {
private:
    vector<Point> hull;

    // Cross Product: Determines relative orientation
    // > 0 : Left Turn (Counter-Clockwise) -> Good for Lower Hull
    // < 0 : Right Turn (Clockwise)
    // = 0 : Collinear
    double cross_product(Point a, Point b, Point c) {
        return (double)(b.x - a.x) * (c.y - a.y) - (double)(b.y - a.y) * (c.x - a.x);
    }

    // Returns true if Point 'b' is redundant and should be removed
    // For Lower Hull, we want a convex shape turning LEFT.
    // So, if a->b->c turns RIGHT or is STRAIGHT, 'b' is bad.
    bool is_bad(Point a, Point b, Point c) {
        return cross_product(a, b, c) <= 0;
    }

    double get_slope(Point a, Point b) {
        return (double)(b.y - a.y) / (b.x - a.x);
    }

public:
    // Call this strictly with increasing X
    void add_point(coord_t x, coord_t y) {
        Point p = {x, y};
        // Remove points from the back that destroy convexity
        while (hull.size() >= 2 && is_bad(hull[hull.size() - 2], hull.back(), p)) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Binary Search to find the point that MAXIMIZES slope with (qx, qy)
    // Time Complexity: O(log N)
    coord_t query_max_slope_index(coord_t qx, coord_t qy) {
        if (hull.empty()) return -1; // Error or handle separately
        if (hull.size() == 1) return hull[0].x;

        Point Q = {qx, qy};
        int l = 0, r = hull.size() - 1;

        // Ternary/Binary search on the derivative (slope)
        while (l < r) {
            int mid = l + (r - l) / 2;
            double s1 = get_slope(hull[mid], Q);
            double s2 = get_slope(hull[mid + 1], Q);

            // If slope is increasing (s1 < s2), the peak is to the right
            if (s1 < s2) {
                l = mid + 1;
            } else {
                // If s1 >= s2, the peak is at 'mid' or to the left.
                // NOTE: If s1 == s2, this logic picks 'mid' (the smaller X).
                // If you need the largest X in a tie, change to: if (s1 <= s2)
                r = mid;
            }
        }
        return hull[l].x;
    }
};


class UpperHull {
    private:
        vector<Point> hull;
    
        // Same cross product logic
        double cross_product(Point a, Point b, Point c) {
            return (double)(b.x - a.x) * (c.y - a.y) - (double)(b.y - a.y) * (c.x - a.x);
        }
    
        // Returns true if Point 'b' is redundant
        // For Upper Hull, we want a convex shape turning RIGHT.
        // So, if a->b->c turns LEFT or is STRAIGHT, 'b' is bad.
        bool is_bad(Point a, Point b, Point c) {
            return cross_product(a, b, c) >= 0; // The only difference is >=
        }
    
        double get_slope(Point a, Point b) {
            return (double)(b.y - a.y) / (b.x - a.x);
        }
    
    public:
        // Call this strictly with increasing X
        void add_point(coord_t x, coord_t y) {
            Point p = {x, y};
            // Remove points that make a Left Turn (or straight)
            while (hull.size() >= 2 && is_bad(hull[hull.size() - 2], hull.back(), p)) {
                hull.pop_back();
            }
            hull.push_back(p);
        }
    
        // Binary Search to find the point that MINIMIZES slope with (qx, qy)
        // Time Complexity: O(log N)
        coord_t query_min_slope_index(coord_t qx, coord_t qy) {
            if (hull.empty()) return -1;
            if (hull.size() == 1) return hull[0].x;
    
            Point Q = {qx, qy};
            int l = 0, r = hull.size() - 1;
    
            while (l < r) {
                int mid = l + (r - l) / 2;
                double s1 = get_slope(hull[mid], Q);
                double s2 = get_slope(hull[mid + 1], Q);
    
                // We are looking for the minimum (valley).
                // If slope is decreasing (s1 > s2), the minimum is to the right
                if (s1 > s2) {
                    l = mid + 1;
                } else {
                    r = mid;
                }
            }
            return hull[l].x;
        }
    };
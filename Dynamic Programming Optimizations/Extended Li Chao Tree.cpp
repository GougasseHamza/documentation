/**
 * Extended Li Chao Tree (Dynamic Pointer Version)
 * Supports: Negative coordinates, Large ranges (10^9), and Segments.
 */

#define ll long long 
 struct Node {
    Line line;
    Node *left = nullptr;
    Node *right = nullptr;

    Node(Line l) : line(l) {}
};

struct DynamicLiChao {
    Node* root;
    ll min_x, max_x;

    DynamicLiChao(ll min_x, ll max_x) : min_x(min_x), max_x(max_x) {
        root = new Node({0, INF});
    }

    // Helper to adds line to a specific node (creates children on demand)
    void add_line_internal(Line new_line, Node* &node, ll l, ll r) {
        if (!node) {
            node = new Node(new_line);
            return;
        }

        ll mid = l + (r - l) / 2;
        bool beats_mid = new_line.eval(mid) < node->line.eval(mid);

        if (beats_mid) {
            swap(node->line, new_line);
        }

        if (l == r) return;

        bool beats_l = new_line.eval(l) < node->line.eval(l);
        if (beats_l) {
            add_line_internal(new_line, node->left, l, mid);
        } else {
            add_line_internal(new_line, node->right, mid + 1, r);
        }
    }

    // Extended: Segment Insertion on Dynamic Tree
    void add_segment(Line new_line, Node* &node, ll l, ll r, ll qL, ll qR) {
        if (r < qL || l > qR) return;
        
        // Create node if it doesn't exist yet to store potential lines
        if (!node) node = new Node({0, INF});

        if (l >= qL && r <= qR) {
            add_line_internal(new_line, node, l, r);
            return;
        }

        ll mid = l + (r - l) / 2;
        add_segment(new_line, node->left, l, mid, qL, qR);
        add_segment(new_line, node->right, mid + 1, r, qL, qR);
    }

    ll query(int x, Node* node, ll l, ll r) {
        if (!node) return INF;

        ll val = node->line.eval(x);
        if (l == r) return val;

        ll mid = l + (r - l) / 2;
        if (x <= mid) {
            return min(val, query(x, node->left, l, mid));
        } else {
            return min(val, query(x, node->right, mid + 1, r));
        }
    }

    // Interface
    void add_segment(ll m, ll c, ll l, ll r) {
        add_segment({m, c}, root, min_x, max_x, l, r);
    }
    
    ll query(ll x) {
        return query(x, root, min_x, max_x);
    }
};
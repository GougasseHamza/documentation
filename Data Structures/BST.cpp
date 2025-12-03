/**
 * Pointer-based BST (With Memory Pool)
 * * Description:
 * Basic BST operations (Insert, Delete, Inorder).
 * Uses a static memory pool instead of malloc/free for speed.
 * * Complexity:
 * Time: O(H) (H=Height). Worst case O(N) if skewed.
 * Space: O(N).
 * * Tricks & Tweaks:
 * - Static 'pool' array prevents dynamic allocation overhead.
 * - 'new_node()' gets a node from the pool.
 * * What Not To Forget:
 * - This is NOT balanced (not AVL/Red-Black).
 * - Degenerates to linked list on sorted input.
 */

 #include <iostream>
 using namespace std;
 
 const int MAX_NODES = 2e5 + 5;
 
 struct node {
   int key;
   struct node *left, *right;
 } pool[MAX_NODES];
 
 int pool_ptr = 0;
 
 struct node *newNode(int item) {
   if (pool_ptr >= MAX_NODES) return NULL; // Safety
   struct node *temp = &pool[pool_ptr++];
   temp->key = item;
   temp->left = temp->right = NULL;
   return temp;
 }
 
 void inorder(struct node *root) {
   if (root != NULL) {
     inorder(root->left);
     cout << root->key << " -> ";
     inorder(root->right);
   }
 }
 
 struct node *insert(struct node *node, int key) {
   if (node == NULL) return newNode(key);
   if (key < node->key)
     node->left = insert(node->left, key);
   else
     node->right = insert(node->right, key);
   return node;
 }
 
 struct node *minValueNode(struct node *node) {
   struct node *current = node;
   while (current && current->left != NULL)
     current = current->left;
   return current;
 }
 
 struct node *deleteNode(struct node *root, int key) {
   if (root == NULL) return root;
 
   if (key < root->key)
     root->left = deleteNode(root->left, key);
   else if (key > root->key)
     root->right = deleteNode(root->right, key);
   else {
     // Node with only one child or no child
     if (root->left == NULL) return root->right;
     else if (root->right == NULL) return root->left;
 
     // Node with two children: Get inorder successor
     struct node *temp = minValueNode(root->right);
     root->key = temp->key;
     root->right = deleteNode(root->right, temp->key);
   }
   return root;
 }
 
 int main() {
   pool_ptr = 0; // Reset pool for test cases
   struct node *root = NULL;
   root = insert(root, 8);
   root = insert(root, 3);
   root = insert(root, 10);
   
   cout << "Inorder: ";
   inorder(root);
   cout << endl;
   
   root = deleteNode(root, 3);
   cout << "After deleting 3: ";
   inorder(root);
 }
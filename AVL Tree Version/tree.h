/*
   selman.yilmaz CS 300 HOMEWORK #2

  A class for the templated AVL tree, a structure for the templated tree nodes, and several functions
  for insertion, rotation, and deletion are all included in the code.

  A node in the AVL tree is represented by the `treeNode` struct, which also has a height,
  a left and right child pointer, a key, and a value.

  Common actions on the AVL tree, such inserting a key-value pair, locating a node with a given key,
  deleting a node, and rotating the tree to balance it, are all made possible by the `tree` class.

  With O(log n) height and effective search, insertion, and deletion processes, this method guarantees
  that the AVL tree stays balanced following each addition or deletion.

*/

#include<iostream>

using namespace std;

// TreeNode struct represents a node in the AVL tree
template <class Key, class Value>
struct treeNode {
    Key key;
    Value value;
    treeNode* left;
    treeNode* right;
    int height;

    // Constructor for initializing tree node
    treeNode(Key k = "", Value v = 0, int h = 1, treeNode* l = nullptr, treeNode* r = nullptr) :
            key(k), value(v), height(h), left(l), right(r) {}
};

// AVL Tree class with insertion, deletion, and rotation operations
template <class Key, class Value>
class tree {
private:
    treeNode<Key, Value>* root;

    // Helper function to recursively delete all nodes in the tree
    void makeEmpty(treeNode<Key, Value>* &t) {
        if (t != nullptr) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

public:
    // Constructor for initializing tree
    tree() : root(nullptr) {}

    // Public method to delete all nodes in the tree
    void makeEmpty() {
        makeEmpty(root);
    }

    //Finds the maximum among two number
    int max(int a, int b) {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }

    // Helper function to delete a node with the specified key
    treeNode<Key, Value>* deleteNode(Key k, treeNode<Key, Value>* t) {
        if (t == nullptr)
            return t;

        // If the key is smaller, go to the left subtree
        if (k < t->key)
            t->left = deleteNode(k, t->left);
            // If the key is larger, go to the right subtree
        else if (k > t->key)
            t->right = deleteNode(k, t->right);
        else {
            // Node with one or no child
            if (t->left == nullptr || t->right == nullptr) {
                treeNode<Key, Value>* temp = t->left ? t->left : t->right;

                // No child case
                if (temp == nullptr) {
                    temp = t;
                    t = nullptr;
                } else
                    *t = *temp;

                delete temp;
            }
                // Node with two children
            else {
                // Find the node with the minimum key in the right subtree
                treeNode<Key, Value>* temp = findMin(t->right);
                // Copy the minimum node's key and value
                t->key = temp->key;
                t->value = temp->value;
                // Delete the minimum node
                t->right = deleteNode(temp->key, t->right);
            }
        }

        // If the tree is not empty, perform balance check
        if (t != nullptr) {
            int balance = height(t->left) - height(t->right);

            // Left subtree is higher
            if (balance > 1) {
                if (height(t->left->left) >= height(t->left->right))
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
                // Right subtree is higher
            else if (balance < -1) {
                if (height(t->right->right) >= height(t->right->left))
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }

            // Update height
            t->height = max(height(t->left), height(t->right)) + 1;
        }

        return t;
    }

    // Public method to delete a node with the specified key using the tree instance
    void deleteKey(Key k, tree<Key, Value>& avlTree) {
        avlTree.root = deleteNode(k, avlTree.getRoot());
    }

    // Helper function to search for a key in the tree
    treeNode<Key, Value>* find(Key x, treeNode<Key, Value>* t) {
        if (t == nullptr || t->key == x)
            return t;

        else if (x < t->key)
            return find(x, t->left);
        else if (x > t->key)
            return find(x, t->right);
    }

    // Helper function to insert a key-value pair into the tree
    treeNode<Key, Value>* insertImp(Key k, Value v, treeNode<Key, Value>* t) {
        if (t == nullptr)
            t = new treeNode<Key, Value>(k, v, height(t), nullptr, nullptr);
        else if (k < t->key) {
            t->left = insertImp(k, v, t->left);
            if (height(t->left) - height(t->right) == 2) {
                if (k < t->left->key)
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        } else if (k > t->key) {
            t->right = insertImp(k, v, t->right);
            if (height(t->right) - height(t->left) == 2) {
                if (k > t->right->key)
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        // Update height
        t->height = max(height(t->left), height(t->right)) + 1;
        return t;
    }

    // Helper function to rotate the tree nodes to the right
    treeNode<Key, Value>* singleRightRotate(treeNode<Key, Value>*& t) {
        treeNode<Key, Value>* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right)) + 1;
        u->height = max(height(u->left), t->height) + 1;
        return u;
    }

    // Helper function to rotate the tree nodes to the left
    treeNode<Key, Value>* singleLeftRotate(treeNode<Key, Value>*& t) {
        treeNode<Key, Value>* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right)) + 1;
        u->height = max(height(t->right), t->height) + 1;
        return u;
    }

    // Helper function to perform double rotation: right child's left subtree, followed by the node itself
    treeNode<Key, Value>* doubleLeftRotate(treeNode<Key, Value>*& t) {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    // Helper function to perform double rotation: left child's right subtree, followed by the node itself
    treeNode<Key, Value>* doubleRightRotate(treeNode<Key, Value>*& t) {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    // Helper function to find the node with the minimum key value in the tree
    treeNode<Key, Value>* findMin(treeNode<Key, Value>* t) {
        if (t == nullptr)
            return nullptr;
        else if (t->left == nullptr)
            return t;
        else
            return findMin(t->left);
    }

    // Helper function to find the node with the maximum key value in the tree
    treeNode<Key, Value>* findMax(treeNode<Key, Value>* t) {
        if (t == nullptr)
            return nullptr;
        else if (t->right == nullptr)
            return t;
        else
            return findMax(t->right);
    }

    // Helper function to return the height of a given node
    int height(treeNode<Key, Value>* t) {
        if (t != nullptr)
            return (t->height);
        return -1;
    }

    // Public method to insert a key-value pair into the tree
    void insert(Key k, Value v) {
        root = insertImp(k, v, root);
    }

    // Public method to return the root of the tree
    treeNode<Key, Value>* getRoot() {
        return root;
    }
};

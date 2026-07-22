#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

template<typename K, typename V>
class BST {
private:
    struct TreeNode {
        K key;
        V value;
        TreeNode* left;
        TreeNode* right;
        int height;
        
        TreeNode(K key, V value) 
            : key(key), value(value), left(nullptr), right(nullptr), height(1) {}
    };
    
    TreeNode* root;
    int size;

    // In-order traversal
    void inOrder(TreeNode* node, std::vector<std::pair<K, V>>& result) const {
        if (node == nullptr) return;
        inOrder(node->left, result);
        result.push_back({node->key, node->value});
        inOrder(node->right, result);
    }

    // Pre-order traversal
    void preOrder(TreeNode* node, std::vector<std::pair<K, V>>& result) const {
        if (node == nullptr) return;
        result.push_back({node->key, node->value});
        preOrder(node->left, result);
        preOrder(node->right, result);
    }

    // Post-order traversal
    void postOrder(TreeNode* node, std::vector<std::pair<K, V>>& result) const {
        if (node == nullptr) return;
        postOrder(node->left, result);
        postOrder(node->right, result);
        result.push_back({node->key, node->value});
    }

    // Helper for deletion
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    TreeNode* insertNode(TreeNode* node, K key, V value) {
        if (node == nullptr) {
            return new TreeNode(key, value);
        }
        
        if (key < node->key) {
            node->left = insertNode(node->left, key, value);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key, value);
        } else {
            node->value = value; // Update
        }
        
        return node;
    }

    TreeNode* deleteNode(TreeNode* node, K key) {
        if (node == nullptr) return nullptr;
        
        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        } else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        } else {
            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                size--;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                size--;
                return temp;
            }
            
            TreeNode* temp = minValueNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->key);
        }
        
        return node;
    }

    TreeNode* searchNode(TreeNode* node, K key) const {
        if (node == nullptr || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return searchNode(node->left, key);
        }
        return searchNode(node->right, key);
    }

    void destroyTree(TreeNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr), size(0) {}

    ~BST() {
        destroyTree(root);
    }

    void insert(K key, V value) {
        root = insertNode(root, key, value);
        size++;
    }

    void remove(K key) {
        root = deleteNode(root, key);
    }

    V* search(K key) const {
        TreeNode* node = searchNode(root, key);
        if (node == nullptr) {
            return nullptr;
        }
        return &(node->value);
    }

    bool contains(K key) const {
        return searchNode(root, key) != nullptr;
    }

    // Binary Search (in-order gives sorted result)
    std::vector<std::pair<K, V>> inOrderTraversal() const {
        std::vector<std::pair<K, V>> result;
        inOrder(root, result);
        return result;
    }

    std::vector<std::pair<K, V>> preOrderTraversal() const {
        std::vector<std::pair<K, V>> result;
        preOrder(root, result);
        return result;
    }

    std::vector<std::pair<K, V>> postOrderTraversal() const {
        std::vector<std::pair<K, V>> result;
        postOrder(root, result);
        return result;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return root == nullptr; }

    void clear() {
        destroyTree(root);
        root = nullptr;
        size = 0;
    }
};

#endif // BST_H
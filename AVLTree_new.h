//
// Created by user on 07/01/2023.
//

#ifndef HASHTABLE_H_AVLTREE_NEW_H
#define HASHTABLE_H_AVLTREE_NEW_H

#include <iostream>
#include <stdlib.h>

using namespace std;

template<class K, class T>
class AVLTree {
private:
    struct Node {
        K key;
        T* data;
        int height;
        Node* left;
        Node* right;
        Node* parent;

        int getBalance() {
            int leftHeight = 0, rightHeight = 0;
            if (left) {
                leftHeight = left->height;
            }
            if (right) {
                rightHeight = right->height;
            }
            return leftHeight - rightHeight;
        }
    };

    // variables
    Node* root;
    Node* smallest;
    int size;

    // functions
    void RemoveNode(Node* node);
    void updateHeights(Node* node);
    void updateSmallest();
    void balance(Node* node);
    void llRotation(Node* node);
    void rrRotation(Node* node);
    void rlRotation(Node* node);
    void lrRotation(Node* node);
    Node* getNode(K key);
    void ParentPointTo(Node* child, Node* newChild);
    void print2(Node* nodeToPrint, int level);
    void destroy(Node* node);

public:
    AVLTree();
    virtual ~AVLTree();
    void Reset();
    int GetSize();
    void Insert(K key, T* data);
    void Remove(K key);
    T* getByKey(K key);
    bool DoesExist(K key);
    void print();
};

/********************************* Public Functions *******************************/

template<class K, class T>
AVLTree<K, T>::AVLTree() {
    root = NULL;
    smallest = NULL;
    size = 0;
}

template<class K, class T>
AVLTree<K, T>::~AVLTree() {
    destroy(root); // release allocated memory
    root = NULL;
    return;
}

template<class K, class T>
void AVLTree<K, T>::Reset() {
    destroy(root); // release allocated memory
    this->size = 0;
    root = NULL;
    return;
}

// Time complexity: O(log(n))
template<class K, class T>
void AVLTree<K, T>::Insert(K key, T* data) {
    if (DoesExist(key)) {
        return;
    }

    // Create the node
    Node* new_node = new Node();
    new_node->height = 1; // height of the subtree with this node as root.
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->key = key;
    new_node->data = data;

    // choose where to add the node and add it.
    if (root == NULL) { // tree is empty
        root = new_node;
        smallest = new_node;
        this->size++;
        return;
    } else { // tree is not empty.
        Node* current = root;
        Node* parent = NULL;
        while ((current != NULL) && (current->key != key)) {
            parent = current;
            if (key < current->key) { // left subtree
                current = current->left;
            } else { // right subtree
                current = current->right;
            }
        }

        // add to tree
        if (key < parent->key)
            parent->left = new_node;
        else
            parent->right = new_node;
        new_node->parent = parent;
        /////////////////////////////////////////////////////

        updateHeights(new_node);
        this->balance(new_node);

        this->size++;

        return;
    }
}

// Time complexity: O(log(n))
template<class K, class T>
void AVLTree<K, T>::Remove(K key) {
    Node* node = getNode(key);
    if (node)
        RemoveNode(node);
    return;
}

// Time complexity: log(n)
template<class K, class T>
T* AVLTree<K, T>::getByKey(K key) {
    Node* node = getNode(key);
    if (!node)
        return nullptr;
    return node->data;
}

// Time complexity: log(n)
template<class K, class T>
bool AVLTree<K, T>::DoesExist(K key) {
    return (getNode(key) != NULL);
}

// printing the tree. using the recursion print2
template<class K, class T>
void AVLTree<K, T>::print() {
    print2(root, 1);
}

/********************************* Private Functions *******************************/
template<class K, class T>
void AVLTree<K, T>::RemoveNode(Node* node) {
    // if leaf
    if (!(node->left) && !(node->right)) {
        ParentPointTo(node, NULL);
        if (node->parent) {
            updateHeights(node->parent);
            this->balance(node->parent);
        } else {
            this->smallest = NULL;
        }
        this->size--;
        delete node;
        return;
    }
        // if only right child
    else if (!(node->left) && (node->right)) {
        ParentPointTo(node, node->right);
        node->right->parent = node->parent;

        if (node->parent) {
            updateHeights(node->parent);
            this->balance(node->parent);
        } else {
            updateSmallest();
        }
        delete node;
        this->size--;
        return;
    }

        // if only left child
    else if ((node->left) && !(node->right)) {
        ParentPointTo(node, node->left);
        node->left->parent = node->parent;

        if (node->parent) {
            updateHeights(node->parent);
            this->balance(node->parent);
        } else {
            updateSmallest();
        }
        delete node;
        this->size--;
        return;
    }

        // if node has 2 childrens
    else {
        // find new root for subtree
        Node* current = node->right;
        while (current->left)
            current = current->left;

        // switch current and node.
        K backupK = current->key;
        current->key = node->key;
        node->key = backupK;

        T* backupT = current->data;
        current->data = node->data;
        node->data = backupT;

        RemoveNode(current); // recursive call. will happen only once, because now node doesn't have 2 children.
    }
}

// Updating the height property of the node and his ancestors.
template<class K, class T>
void AVLTree<K, T>::updateHeights(Node* node) {
    Node* current = node;
    while (current != NULL) {
        int leftHeight = 0, rightHeight = 0;
        if (current->left) {
            leftHeight = current->left->height;
        }
        if (current->right) {
            rightHeight = current->right->height;
        }
        current->height =
                ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
        current = current->parent;
    }
}

template<class K, class T>
void AVLTree<K, T>::balance(Node* node) {
    int balanceFactor = node->getBalance();
    if (balanceFactor >= 2) { //left Heavy
        //assert(node->left->getBalance() != 0);
        if (node->left->getBalance() >= 0) {
            llRotation(node);
        } else {
            lrRotation(node);
        }
    } else if (balanceFactor <= -2) { //left Heavy
        //assert(node->right->getBalance() != 0);
        if (node->right->getBalance() > 0) {
            rlRotation(node);
        } else {
            rrRotation(node);
        }
    }
////////////////
    if (node->parent) {
        balance(node->parent);
    }

    updateSmallest();
}

// node: the node with the |balance|>=2
template<class K, class T>
void AVLTree<K, T>::llRotation(Node* node) {
    Node* parent = node->parent;
    Node* lChild = node->left;

    // parent-node relation
    ParentPointTo(node, lChild);
    lChild->parent = parent;

    Node* lrChild = node->left->right; //backup
    // node-child relation
    lChild->right = node;
    node->parent = lChild;
    // restore lost chain
    node->left = lrChild;
    if (lrChild)
        lrChild->parent = node;

    updateHeights(node);
}

// node: the node with the |balance|>=2
template<class K, class T>
void AVLTree<K, T>::rrRotation(Node* node) {
    Node* parent = node->parent;
    Node* rChild = node->right;

    // parent-node relation
    ParentPointTo(node, rChild);
    rChild->parent = parent;

    Node* rlChild = node->right->left; //backup
    // node-child relation
    rChild->left = node;
    node->parent = rChild;
    // restore lost chain
    node->right = rlChild;
    if (rlChild)
        rlChild->parent = node;

    updateHeights(node);
}

// node: the node with the |balance|>=2
template<class K, class T>
void AVLTree<K, T>::lrRotation(Node* node) {
    rrRotation(node->left);
    llRotation(node);
}

// node: the node with the |balance|>=2
template<class K, class T>
void AVLTree<K, T>::rlRotation(Node* node) {
    llRotation(node->right);
    rrRotation(node);
}

template<class K, class T>
typename AVLTree<K, T>::Node* AVLTree<K, T>::getNode(K key) {
    Node* current = root;
    while ((current != NULL) && (current->key != key)) { // while wasn't placed yet
        if (key < current->key) { // left subtree
            current = current->left;
        } else { // right subtree
            current = current->right;
        }
    }
    return current;
}

template<class K, class T>
void AVLTree<K, T>::ParentPointTo(Node* child, Node* newChild) {
    if (child->parent == NULL)
        root = newChild;
    else {
        if (child->parent->left == child) {
            child->parent->left = newChild;
        } else {
            child->parent->right = newChild;
        }

    }
}

/************************************* Recursions ***********************************/

template<class K, class T>
void AVLTree<K, T>::print2(Node* nodeToPrint, int level) {
    int i;
    if (nodeToPrint) {
        print2(nodeToPrint->right, level + 1);
        cout << "\n";
        if (nodeToPrint == root) {
            cout << "Root ->";
        }

        for (i = 0; i < level && nodeToPrint != root; i++) {
            cout << "       ";
        }
        //cout << nodeToPrint->key;
        //cout << nodeToPrint->data;
        cout << nodeToPrint->height;
        //cout << nodeToPrint->getBalance();

        print2(nodeToPrint->left, level + 1);
    }
}

// destroying the array, post order
template<class K, class T>
void AVLTree<K, T>::destroy(Node* node) {
    if (node == NULL)
        return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

#endif //HASHTABLE_H_AVLTREE_NEW_H

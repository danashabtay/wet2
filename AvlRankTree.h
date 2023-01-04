//
// Created by user on 28/12/2022.
//

#ifndef WET2_AVLRANKTREE_H
#define WET2_AVLRANKTREE_H
//
// Created by user on 28/12/2022.
//

#ifndef AVLTREE_H_AVLRANKTREE_H
#define AVLTREE_H_AVLRANKTREE_H
#include <iostream>
#include <stdlib.h>

template<class T, class K>
class AVLRankTree {
    private:
        struct Node {
            K key;
            T data;
            int height;
            int rank;
            Node* left;
            Node* right;
            Node* parent;


            int getBalance() {
                int leftHeight = 0, rightHeight = 0;
                if (this->left) {
                    leftHeight = this->left->height;
                }
                if (this->right) {
                    rightHeight = this->right->height;
                }
                return leftHeight - rightHeight;
            }
        };

        // variables
        Node* root;
        Node* smallest; // most left in the tree
        int size;

        // functions
        void RemoveNode(Node* node);
        void updateHeights(Node* node);
        void updateRanks(Node* node);
        void updateSmallest();
        void setAllRanks(Node* subroot);
        void balance(Node* node);
        void llRotation(Node* node);
        void rrRotation(Node* node);
        void rlRotation(Node* node);
        void lrRotation(Node* node);
        Node* getNode(K key, T data);
        void ParentPointTo(Node* child, Node* newChild);
        void print2(Node* nodeToPrint, int level);
        void destroy2(Node* node);

        T Select(Node* node, int k);
    public:
        AVLRankTree();
        virtual ~AVLRankTree();
        void Reset();
        int GetSize();
        int getInRange(K min, K max);
        void Insert(K key, T data);
        void Remove(K key, T data);
        T getByKey(K key, T data);
        T getByRank(int i);
        void print();
};

/********************************* Public Functions *******************************/

template<class T, class K>
AVLRankTree<T,K>::AVLRankTree() {
    this->root = NULL;
    this->smallest = NULL;
    size = 0;
}

template<class T, class K>
AVLRankTree<T,K>::~AVLRankTree() {
    destroy2(this->root); // release allocated memory
    this->root = NULL;
}

template<class T, class K>
void AVLRankTree<T,K>::Reset() {
    destroy2(this->root); // release allocated memory
    this->size = 0;
    this->root = NULL;
    return;
}

// Time complexity: O(1)
template<class T, class K>
int AVLRankTree<T,K>::GetSize() {
    return this->size;
}

// Time complexity: O(log(n))
template<class T, class K>
void AVLRankTree<T,K>::Insert(K key, T data) {
    // Create the node
    Node* new_node = new Node();
    new_node->height = 1; // height of the subtree with this node as root.
    new_node->rank = 1;
    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = nullptr;
    new_node->key = key;
    new_node->data = data;

    // choose where to add the node and add it.
    if (this->root == nullptr) { // tree is empty
        this->root = new_node;
        this->smallest = new_node;
        this->size++;
        return;
    } else { // tree is not empty.

        Node* current = root;
        Node* parent = nullptr;

        //////////////////////////////////////////////////////
        while ((current != nullptr) && (current->key != key)) {
            parent = current;
            if (key < current->key) { // left subtree
                current = current->left;
            } else if(key > current->key)
            { // right subtree
                current = current->right;
            }
            else{ //keys are equal
                if(data< current->data){
                    current = current->left;
                }
                else{
                    current = current->right;
                }
            }
        }

        // add to tree
        if (key < parent->key)
            parent->left = new_node;
        else if(key > parent->key)
            parent->right = new_node;
        else{ //keys are equal
            if(data< parent->data){
                parent->left = new_node;
            }
            else{
                parent->right = new_node;
            }
        }
        new_node->parent = parent;
        /////////////////////////////////////////////////////

        updateHeights(new_node);
        updateRanks(new_node);
        this->balance(new_node);

        this->size++;

        return;
    }
}

// Time complexity: O(log(n))
template<class T, class K>
void AVLRankTree<T,K>::Remove(K key, T data) {
    Node* node = getNode(key, data);
    if (node)
        RemoveNode(node);
    return;
}

// Time complexity: log(n)
template<class T, class K>
T AVLRankTree<T,K>::getByKey(K key, T data) {
    Node* node = getNode(key, data);
    if (!node)
        return nullptr;
    return node->data;
}

// Time complexity: log(n)
template<class T, class K>
T AVLRankTree<T,K>::getByRank(int i) {
    return Select(this->root, i);
}

// printing the tree. using the recursion print2
template<class T, class K>
void AVLRankTree<T,K>::print() {
    print2(this->root, 1);
}

/********************************* Private Functions *******************************/
template<class T, class K>
T AVLRankTree<T,K>::Select(Node* node, int k) {
    if(node->left->rank==k+1 || (node->left==nullptr && k==1)){
        return node->data;
    }
    if(node->left->rank>k+1){
        return Select(node->left,k);
    }
    if(node->left->rank<k+1){
        return Select(node->left,k-(node->left->rank)-1);
    }
    return -1;
}

template<class T, class K>
void AVLRankTree<T,K>::RemoveNode(Node* node) {
    // if leaf
    if (!(node->left) && !(node->right)) {
        ParentPointTo(node, nullptr);
        if (node->parent) {
            updateHeights(node->parent);
            updateRanks(node->parent);
            this->balance(node->parent);
        } else {
            this->smallest = nullptr;
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
            updateRanks(node->parent);
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
            updateRanks(node->parent);
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

        T backupT = current->data;
        current->data = node->data;
        node->data = backupT;

        RemoveNode(current); // recursive call. will happen only once, because now node doesn't have 2 children.
    }
}

// Updating the height property of the node and his ancestors.
template<class T, class K>
void AVLRankTree<T,K>::updateHeights(Node* node) {
    Node* current = node;
    while (current != nullptr) {
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

// Updating the height property of the node and his ancestors.
template<class T, class K>
void AVLRankTree<T,K>::updateRanks(Node* node) {
    Node* current = node;
    while (current != nullptr) {
        int leftRank = 0, rightRank = 0;
        if (current->left) {
            leftRank = current->left->rank;
        }
        if (current->right) {
            rightRank = current->right->rank;
        }
        current->rank = leftRank + rightRank + 1;
        current = current->parent;
    }
}

// Updating the smallest key. O(log(k)).
template<class T, class K>

void AVLRankTree<T,K>::updateSmallest() {
    Node* current = root;
    if (!current) {
        this->smallest = nullptr;
        return; // empty tree
    }
    while (current->left != nullptr) {
        current = current->left;
    }
    this->smallest = current;
}

//running post order and setting the ranks of the entire tree
template<class T, class K>
void AVLRankTree<T,K>::setAllRanks(Node* subroot) {
    if (!subroot)
        return; // empty tree , only if root==null
    int leftRank = 0;
    int rightRank = 0;
    if (subroot->left) {
        setAllRanks(subroot->left);
        leftRank = subroot->left->rank;
    }
    if (subroot->right) {
        setAllRanks(subroot->right);
        rightRank = subroot->right->rank;
    }
    subroot->rank = leftRank + rightRank + 1;
}

template<class T, class K>
void AVLRankTree<T,K>::balance(Node* node) {
    int balanceFactor = node->getBalance();
    if (balanceFactor >= 2) { //left Heavy
        if (node->left->getBalance() >= 0) {
            llRotation(node);
        } else {
            lrRotation(node);
        }
    } else if (balanceFactor <= -2) { //left Heavy
        if (node->right->getBalance() > 0) {
            rlRotation(node);
        } else {
            rrRotation(node);
        }
    }

    if (node->parent) {
        balance(node->parent);
    }

}

// node: the node with the |balance|>=2
template<class T, class K>
void AVLRankTree<T,K>::llRotation(Node* node) {
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
    updateRanks(node);
}

// node: the node with the |balance|>=2
template<class T, class K>
void AVLRankTree<T,K>::rrRotation(Node* node) {
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
    updateRanks(node);
}

// node: the node with the |balance|>=2
template<class T, class K>
void AVLRankTree<T,K>::lrRotation(Node* node) {
    rrRotation(node->left);
    llRotation(node);
}

// node: the node with the |balance|>=2
template<class T, class K>
void AVLRankTree<T,K>::rlRotation(Node* node) {
    llRotation(node->right);
    rrRotation(node);
}

template<class T, class K>
typename AVLRankTree<T,K>::Node* AVLRankTree<T,K>::getNode(K key, T data) {
    Node* current = root;
    while ((current != nullptr) && (current->key != key) && (current->data != data)) { // while wasn't placed yet
        if (key < current->key) { // left subtree
            current = current->left;
        } else if(key > current->key) { // right subtree
            current = current->right;
        }
        else{ //keys are equal
            if(data< current->data){
                current = current->left;
            }
            else{
                current = current->right;
            }
        }
    }
    return current;
}

template<class T, class K>
void AVLRankTree<T,K>::ParentPointTo(Node* child, Node* newChild) {
    if (child->parent == nullptr)
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

template<class T, class K>
void AVLRankTree<T,K>::print2(Node* nodeToPrint, int level) {
    int i;
    if (nodeToPrint) {
        print2(nodeToPrint->right, level + 1);
        std::cout << "\n";
        if (nodeToPrint == root) {
            std::cout << "Root ->";
        }

        for (i = 0; i < level && nodeToPrint != root; i++) {
            std::cout << "       ";
        }
        std::cout << nodeToPrint->key;
        //cout << nodeToPrint->data;
        //cout << nodeToPrint->rank;
        //cout << nodeToPrint->getBalance();

        print2(nodeToPrint->left, level + 1);
    }
}

// destroying the array, post order
template<class T, class K>
void AVLRankTree<T,K>::destroy2(Node* node) {
    if (node == NULL)
        return;
    destroy2(node->left);
    destroy2(node->right);
    delete node;
}

#endif //AVLTREE_H_AVLRANKTREE_H

#endif //WET2_AVLRANKTREE_H

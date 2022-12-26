//
// Created by user on 25/12/2022.
//

#ifndef WET2_AVLTREE_H
#define WET2_AVLTREE_H
#include <iostream>
#include <memory>

template<class T, class K>
class AvlTree {
    class node
    {
    public:
        K key;
        std::shared_ptr<T> data;
        node *left;
        node *right;
        int height;
        node(std::shared_ptr<T> data, K key);
        ~node(){
            delete left;
            delete right;
        }
    };

    node<T, K>::node(std::shared_ptr<T> m_data, K m_key) : key(m_key), data(m_data), right(nullptr), left(nullptr), height(1){
    }

    private:
        node<T,K> *m_root;

    public:
        AvlTree();
        ~AvlTree();
        int height(node<T,K> *N);
        node<T,K> * minValueNode(node<T,K>* root);
        node<T,K> * removeHelper(node<T,K>* root, K key);
        std::shared_ptr<T>find_by_key_helper(node<T,K>* root,K key);
        std::shared_ptr<T>find_by_key(K key);
        node<T,K>* newNode(std::shared_ptr<T> data, K key);
        node<T,K> *rightRotate(node<T,K> *y);
        node<T,K> *leftRotate(node<T,K> *x);
        int getBalance(node<T,K> *N);
        void insert(std::shared_ptr<T> data, K key);
        node<T,K>* insertHelper(node<T,K>* root, std::shared_ptr<T> data, K key);
        void deleteTree(node<T,K> *r);
        void remove(K key);
        int max2(int a, int b)
        {
            return (a > b)? a : b;
        }
    };
/////////////////////////////////////////////////////implementation//////////////////////////////////////////////////

template<class T, class K>
AvlTree<T,K>::AvlTree() : m_root(nullptr) {}

template<class T, class K>
AvlTree<T,K>::~AvlTree() {
    deleteTree(this->m_root);
}


/* Given a non-empty binary search tree,
return the node with minimum key value
found in that tree. Note that the entire
tree does not need to be searched. */
template<class T, class K>
node<T, K> *AvlTree<T, K>::minValueNode(node<T,K> *root) {
    node<T,K>* current = root;

    /* loop down to find the leftmost leaf */
    while (current->left != nullptr)
        current = current->left;

    return current;
}

// Recursive function to delete a node
// with given key from subtree with
// given root. It returns root of the
// modified subtree.
template<class T, class K>
node<T,K> * AvlTree<T,K>::removeHelper(node<T,K>* root,K key) {

    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == nullptr)
        return root;

    // If the key to be deleted is smaller
    // than the root's key, then it lies
    // in left subtree
    if ( key < root->key )
        root->left = removeHelper(root->left, key);

        // If the key to be deleted is greater
        // than the root's key, then it lies
        // in right subtree
    else if( key > root->key )
        root->right = removeHelper(root->right, key);

        // if key is same as root's key, then
        // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == nullptr) ||
            (root->right == nullptr) )
        {
            node<T,K> *temp = root->left ?
                              root->left :
                              root->right;

            // No child case
            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else // One child case
                *root = *temp; // Copy the contents of
            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            node<T,K>* temp = minValueNode(root->right);

            // Copy the inorder successor's
            // data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = removeHelper(root->right,
                                       temp->key);
        }
    }

    // If the tree had only one node
    // then return
    if (root == nullptr)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max2(height(root->left),
                            height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced,
    // then there are 4 cases

    // Left Left Case
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

template<class T, class K>
std::shared_ptr<T> AvlTree<T,K>::find_by_key_helper(node *root, K key) {
    if(root== nullptr){
        return nullptr;
    }
    if (key == root->key){
        return root->data;
    }
    else{
        if (key < root->key){
            return find_by_key(root->left,key);
        }
        else if (key > root->key){
            return find_by_key(root->right,key);
        }
        else {
            return nullptr;
        }
    }
}

template<class T, class K>
std::shared_ptr<T> AvlTree<T,K>::find_by_key(K key) {
    return find_by_key_helper(m_root,key);
}

template<class T, class K>
void AvlTree<T, K>::deleteTree(node<T,K> *r) {

    if (r != nullptr) {
        deleteTree(r->left);
        deleteTree(r->right);
        remove(r->key);
    }
}

/* Helper function that allocates a
   new node with the given key and
   NULL left and right pointers. */
template<class T, class K>
node<T, K> *AvlTree<T, K>::newNode(std::shared_ptr<T> data, K key) {
    node<T,K>* newNode = new node<T, K>(data,key);
    newNode->key = key;
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1; // new node is initially added at leaf
    return(newNode);
}

// A utility function to right
// rotate subtree rooted with y
// See the diagram given above.
template<class T, class K>
node<T, K> *AvlTree<T, K>::rightRotate(node<T, K> *y) {
    node<T,K> *x = y->left;
    node<T,K> *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max2(height(y->left),
                     height(y->right)) + 1;
    x->height = max2(height(x->left),
                     height(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left
// rotate subtree rooted with x
// See the diagram given above.
template<class T, class K>
node<T, K> *AvlTree<T, K>::leftRotate(node<T, K> *x) {
    node<T,K> *y = x->right;
    node<T,K> *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max2(height(x->left),
                     height(x->right)) + 1;
    y->height = max2(height(y->left),
                     height(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
template<class T, class K>
int AvlTree<T, K>::getBalance(node<T, K> *N) {
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

template<class T, class K>
void AvlTree<T, K>::remove(K key) {
    m_root=removeHelper(m_root,key);
}

// Recursive function to insert a key
// in the subtree rooted with node and
// returns the new root of the subtree.
template<class T, class K>
node<T, K> *AvlTree<T, K>::insertHelper(node<T,K> *root, std::shared_ptr<T> data, K key) {
/* 1. Perform the normal BST insertion */
    if (root == nullptr){
        root=newNode(data,key); //maybe fix unchanging root?
        return root;
    }

    if (key < root->key)
        root->left = insertHelper(root->left, data,key);
    else if (key > root->key)
        root->right = insertHelper(root->right,data, key);
    else // Equal keys are not allowed in BST
        return root;

    /* 2. Update height of this ancestor node */
    root->height = 1 + max2(height(root->left),
                            height(root->right));

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(root);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key < root->left->key)
        return rightRotate(root);

    // Right Right Case
    if (balance < -1 && key > root->right->key)
        return leftRotate(root);

    // Left Right Case
    if (balance > 1 && key > root->left->key)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && key < root->right->key)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    /* return the (unchanged) node pointer */
    return root;
}

// A utility function to get height of the tree
template<class T, class K>
int AvlTree<T, K>::height(node<T, K> *N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

template<class T, class K>
void AvlTree<T, K>::insert(std::shared_ptr<T> data, K key) {
    m_root = insertHelper(m_root, data, key);
    return;
}


#endif //WET2_AVLTREE_H

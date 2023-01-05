//
// Created by user on 06/01/2023.
//

#ifndef HASHTABLE_H_AVL_H
#define HASHTABLE_H_AVL_H
#include<iostream>

using namespace std;

template <class T>
class BST
{
    struct node
    {
        int key;
        T* data;
        node* left;
        node* right;
        int height;
        T* getVal(){
            return data;
        }
    };

    node* root;

    void makeEmpty(node* t)
    {
        if(t == NULL)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }

    node* find_by_key_helper(node* t, int k){
        if(t == NULL){
            return NULL;
        }
        else if(t->key>k)
            return find_by_key_helper(t->left,k);
        else if(t->key<k)
            return find_by_key_helper(t->right,k);
        else
            return t;
    }

    node* insert(int x,T* val, node* t)
    {
        if(t == NULL)
        {
            t = new node;
            t->key = x;
            t->data = val;
            t->height = 0;
            t->left = t->right = NULL;
        }
        else if(x < t->key)
        {
            t->left = insert(x, val, t->left);
            if(height(t->left) - height(t->right) == 2)
            {
                if(x < t->left->key)
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        }
        else if(x > t->key)
        {
            t->right = insert(x, val, t->right);
            if(height(t->right) - height(t->left) == 2)
            {
                if(x > t->right->key)
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        t->height = max(height(t->left), height(t->right))+1;
        return t;
    }

    node* singleRightRotate(node* &t)
    {
        node* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(u->left), t->height)+1;
        return u;
    }

    node* singleLeftRotate(node* &t)
    {
        node* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(t->right), t->height)+1 ;
        return u;
    }

    node* doubleLeftRotate(node* &t)
    {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    node* doubleRightRotate(node* &t)
    {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    node* findMin(node* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->left == NULL)
            return t;
        else
            return findMin(t->left);
    }

    node* findMax(node* t)
    {
        if(t == NULL)
            return NULL;
        else if(t->right == NULL)
            return t;
        else
            return findMax(t->right);
    }

    node* remove(int x, node* t)
    {
        node* temp;

        // Element not found
        if(t == NULL)
            return NULL;

            // Searching for element
        else if(x < t->key)
            t->left = remove(x, t->left);
        else if(x > t->key)
            t->right = remove(x, t->right);

            // Element found
            // With 2 children
        else if(t->left && t->right)
        {
            temp = findMin(t->right);
            t->key = temp->key;
            t->right = remove(t->key, t->right);
        }
            // With one or zero child
        else
        {
            temp = t;
            if(t->left == NULL)
                t = t->right;
            else if(t->right == NULL)
                t = t->left;
            delete temp;
        }
        if(t == NULL)
            return t;

        t->height = max(height(t->left), height(t->right))+1;

        // If node is unbalanced
        // If left node is deleted, right case
        if(height(t->left) - height(t->right) == 2)
        {
            // right right case
            if(height(t->left->left) - height(t->left->right) == 1)
                return singleLeftRotate(t);
                // right left case
            else
                return doubleLeftRotate(t);
        }
            // If right node is deleted, left case
        else if(height(t->right) - height(t->left) == 2)
        {
            // left left case
            if(height(t->right->right) - height(t->right->left) == 1)
                return singleRightRotate(t);
                // left right case
            else
                return doubleRightRotate(t);
        }
        return t;
    }

    int height(node* t)
    {
        return (t == NULL ? -1 : t->height);
    }

    int getBalance(node* t)
    {
        if(t == NULL)
            return 0;
        else
            return height(t->left) - height(t->right);
    }

    void inorder(node* t)
    {
        if(t == NULL)
            return;
        inorder(t->left);
        cout << t->key << " ";
        inorder(t->right);
    }

public:
    BST()
    {
        root = NULL;
    }

    void insert(int x, T* val)
    {
        root = insert(x, val, root);
    }

    void remove(int x)
    {
        root = remove(x, root);
    }

    T* find_by_key(int k){
        node* tmp= find_by_key_helper(root,k);
        T* value = tmp->getVal();
        if(value==NULL){
            return NULL;
        }
        return value;
    }

    void display()
    {
        inorder(root);
        cout << endl;
    }
};
#endif //HASHTABLE_H_AVL_H

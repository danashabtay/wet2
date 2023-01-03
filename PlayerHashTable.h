//
// Created by user on 03/01/2023.
//

#ifndef WET2_PLAYERHASHTABLE_H
#define WET2_PLAYERHASHTABLE_H

#include "UnionFind.cpp"
class playerNode;

class PlayerHashNode {
private:
    std::shared_ptr<playerNode> m_value;
    int m_key;
    bool isDeleted;
public:
    PlayerHashNode(int key, std::shared_ptr<playerNode> value)
    {
        m_value = value;
        m_key = key;
        isDeleted= false;
    }
    bool isItDeleted() {
        return isDeleted;
    }
    void setAsDeleted() {
        isDeleted=true;
    }
};

class PlayerHashTable {
    private:
    PlayerHashNode **m_HashTable;
        int capacity; // current size
        int size;
    PlayerHashNode *dummy; // dummy node

    public:
        void insertNode(int key, std::shared_ptr<playerNode> value);
        PlayerHashTable();
        ~PlayerHashTable();
        int hashFunc(int key);
        void reHash();
        std::shared_ptr<playerNode> findNode(int key);
        void markDeleted(int key);
        void deleteTable();
        bool isItDeleted(int key);
    };

#endif //WET2_PLAYERHASHTABLE_H

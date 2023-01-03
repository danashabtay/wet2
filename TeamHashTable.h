//
// Created by user on 03/01/2023.
//

#ifndef WET2_TEAMHASHTABLE_H
#define WET2_TEAMHASHTABLE_H

#include "UnionFind.cpp"
class teamNode;

class TeamHashNode {
private:
    std::shared_ptr<teamNode> m_value;
    int m_key;
    bool isDeleted;
public:
    TeamHashNode(int key, std::shared_ptr<teamNode> value)
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

class TeamHashTable {
    private:
        TeamHashNode **m_HashTable;
        int capacity; // current size
        int size;
        TeamHashNode *dummy; // dummy node

    public:
        void insertNode(int key, std::shared_ptr<teamNode> value);
        TeamHashTable();
        ~TeamHashTable();
        int hashFunc(int key);
        void reHash();
        std::shared_ptr<teamNode> findNode(int key);
        void markDeleted(int key);
        void deleteTable();
        bool isItDeleted(int key);
        bool DoesExist(int key);
    };


#endif //WET2_TEAMHASHTABLE_H

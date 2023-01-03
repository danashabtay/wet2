//
// Created by user on 03/01/2023.
//

#include "TeamHashTable.h"
TeamHashTable::TeamHashTable()
{
    capacity = 16;
    size = 0;
    m_HashTable = new TeamHashNode*[capacity]; ///parameters for initializer?
    for (int i = 0; i < capacity; i++)
        m_HashTable[i] = nullptr;
    // dummy node with key -1
    dummy = new TeamHashNode(-1, nullptr); ///????????
}

int TeamHashTable::hashFunc(int key)
{
    return key % capacity;
}

TeamHashTable::~TeamHashTable(){
    deleteTable();
}

void TeamHashTable::reHash()
{
    int oldCapacity = capacity;
    capacity = oldCapacity * 2 + 1;

    TeamHashNode** newHashTable = new TeamHashNode*[capacity]; ///parameters for initializer?

    //fill in the new temp table with old info
    for (int i = 0; i < oldCapacity; ++i)
    {
        TeamHashNode *oldNode = m_HashTable[i];
        TeamHashNode *tmp = oldNode;
        TeamHashNode *bucket = newHashTable[HashFunc(tmp->key) % capacity];
        bucket = tmp;
    }
    delete [] m_HashTable;
    m_HashTable = newHashTable;
}


void TeamHashTable::insertNode(int key, std::shared_ptr<teamNode> value) {
    TeamHashNode* temp = new HashNode(key, value);

    // Apply hash function to find index for given key
    int hashIndex = hashFunc(key);

    // find next free space
    while (m_HashTable[hashIndex] != nullptr
           && m_HashTable[hashIndex]->key != key
           && m_HashTable[hashIndex]->key != -1) {
        hashIndex++;
        hashIndex %= capacity;
    }

    // if new node to be inserted
    // increase the current size
    if (m_HashTable[hashIndex] == nullptr || m_HashTable[hashIndex]->key == -1)
        size++;

    m_HashTable[hashIndex] = temp;

    if(size==0.5*capacity){
        reHash();
    }
}

std::shared_ptr<teamNode> TeamHashTable::findNode(int key)
{
    int hashIndex = hashFunc(key);
    int counter = 0;
    // finding the node with given key
    while (m_HashTable[hashIndex] != nullptr) {
        if (counter++ > capacity)
            return nullptr;
        // if node found return its value
        if (m_HashTable[hashIndex]->key == key)
            return m_HashTable[hashIndex]->value;
        hashIndex++;
        hashIndex %= capacity;
    }
    return nullptr;
}


void TeamHashTable::markDeleted(int key){
    int hashIndex = hashFunc(key);
    int counter = 0;
    // finding the node with given key
    while (m_HashTable[hashIndex] != nullptr) {
        if (counter++ > capacity)
            return;
        if (m_HashTable[hashIndex]->key == key){
            m_HashTable[hashIndex]->setAsDeleted();
            return;
        }
        hashIndex++;
        hashIndex %= capacity;
    }
}


bool TeamHashTable::isItDeleted(int key){
    int hashIndex = hashFunc(key);
    int counter = 0;
    // finding the node with given key
    while (m_HashTable[hashIndex] != nullptr) {
        if (counter++ > capacity)
            return true;
        if (m_HashTable[hashIndex]->m_key == key){
            return m_HashTable[hashIndex]->isItDeleted();

        }
        hashIndex++;
        hashIndex %= capacity;
    }
    return true;
}


void TeamHashTable::deleteTable() {
    for (int i = 0; i < capacity; ++i)
    {
        delete m_HashTable[i];
    }
    delete[] m_HashTable;
}


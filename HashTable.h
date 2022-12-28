//
// Created by user on 25/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

template <class V>
class HashNode {
public:
    V value;
    int key;
    bool isDeleted;

    // Constructor of hashnode
    HashNode(int key, V value)
    {
        this->value = value;
        this->key = key;
        this->isDeleted= false;
    }
    bool isDeleted() {
    return isDeleted;
    }
    void setAsdDeleted() {
    isDeleted=true;
    }
};

template <class V>
class HashTable {
private:
    HashNode<V> **HashTable;
    int capacity; // current size
    int size;
    HashNode<V> *dummy; // dummy node

public:
    HashTable();
    int hashFunc(int key);
    void reHash();
    void insertNode(int key, V value);
    V* findNode(int key);
    void markDeleted(int key);
};

/////////////////////////////////implementation///////////////////////////////////////////////////////////////////

HashTable()
    {
        HashTable = new HashNode<V>*[capacity]; ///parameters for initializer?
        capacity = 16;
        size = 0;

        for (int i = 0; i < capacity; i++)
            HashTable[i] = NULL;
        // dummy node with key -1
        dummy = new HashNode<V>(-1, nullptr); ///????????
    }

    int hashFunc(int key)
    {
        return key % capacity;
    }
}

void reHash()
{
    int oldCapacity = capacity;
    capacity = oldCapacity * 2 + 1;

    HashNode<V>** newHashTable = new HashNode<V>*[capacity]; ///parameters for initializer?

    //fill in the new temp table with old info
    for (int i = 0; i < oldCapacity; ++i)
    {
        HashNode<V> *oldNode = HashTable[i];
        HashNode<V> *tmp = oldNode;
        HashNode<V> *bucket = newHashTable[HashFunc(tmp->key) % cap];
        bucket = tmp;
    }
    delete [] HashTable;
    HashTable = newHashTable;
}

void insertNode(int key, V value) {
    HashNode<V>* temp = new HashNode<V>(key, value);

    // Apply hash function to find index for given key
    int hashIndex = hashFunc(key);

    // find next free space
    while (HashTable[hashIndex] != nullptr
           && HashTable[hashIndex]->key != key
           && HashTable[hashIndex]->key != -1) {
        hashIndex++;
        hashIndex %= capacity;
    }

    // if new node to be inserted
    // increase the current size
    if (HashTable[hashIndex] == nullptr || HashTable[hashIndex]->key == -1)
        size++;

    HashTable[hashIndex] = temp;

    if(size==0.5*capacity){
        reHash();
    }
}

V* findNode(int key)
{
    int hashIndex = hashFunc(key);
    int counter = 0;
    // finding the node with given key
    while (HashTable[hashIndex] != nullptr) {
        if (counter++ > capacity)
            return nullptr;
        // if node found return its value
        if (HashTable[hashIndex]->key == key)
            return &HashTable[hashIndex]->value;
        hashIndex++;
        hashIndex %= capacity;
    }
    return nullptr;
}

void markDeleted(int key){
    int hashIndex = hashFunc(key);
    int counter = 0;
    // finding the node with given key
    while (HashTable[hashIndex] != nullptr) {
        if (counter++ > capacity)
            return;
        if (HashTable[hashIndex]->key == key){
            HashTable[hashIndex]->setAsDeleted();
        return;
    }
        hashIndex++;
        hashIndex %= capacity;
    }
    return;
}

void isDeleted(int key){
    int hashIndex = hashFunc(key);
    int counter = 0;
    // finding the node with given key
    while (HashTable[hashIndex] != nullptr) {
        if (counter++ > capacity)
            return true;
        if (HashTable[hashIndex]->key == key){
            return HashTable[hashIndex]->isDeleted();

        }
        hashIndex++;
        hashIndex %= capacity;
    }
    return true;
}



#endif //WET2_HASHTABLE_H

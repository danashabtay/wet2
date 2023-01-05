//
// Created by user on 04/01/2023.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

template <class V>
class HashTable{
    V** m_hashtable;
    int m_max_size;
    int m_curr_size;
public:
    explicit HashTable(int size);
    ~HashTable();
    void rehash();
    void insert(V* data, int key);
    V* find(int Id);
};

template <class V>
HashTable<V>::HashTable(int size) : m_max_size(size), m_curr_size(0), m_hashtable(){
    m_hashtable=new V*[size*sizeof(V*)];
    for(int i=0;i<size;i=i+1){
        std::cout<<i;
        m_hashtable[i]=NULL;
    }
}

template <class V>
HashTable<V>::~HashTable() {
    for(int i = 0; i < m_max_size; i++){
        delete m_hashtable[i];
    }
    delete[] m_hashtable;
}

template <class V>
void HashTable<V>::rehash() {
        m_max_size *= 2;
        V **new_table = new V *[m_max_size * sizeof(V*)];
        V **tmp_table = new V *[m_curr_size * sizeof(V*)];
        ////////////////////////////////copy old teams_table to tmp table
        for(int i = 0; i < m_curr_size; i++) {
            tmp_table[i] = m_hashtable[i];
        }
        ///////////////////////////////delete old teams_table
        for(int i = 0; i < m_curr_size; i++) {
            delete m_hashtable[i];
        }
        delete[] m_hashtable;
        ////////////////////////////////assign new table as teams table
        m_hashtable=new_table;
        ///////////////////////////////copy tmp table to new teams_table
        for(int i = 0; i < m_max_size; i++) {
            m_hashtable[i]=NULL;
            if (tmp_table[i]!= NULL){
                insert(tmp_table[i],tmp_table[i]->getKey());
            }
        }
    }

template <class V>
void HashTable<V>::insert(V *data, int key) {
    if(m_max_size==m_curr_size){
        rehash();
    }
    int counter=0;
    int hashIndex = ((key%m_max_size)+(counter * (1 + (key%(m_max_size-1)))))%m_max_size;
    std::cout<<"hash index: " << hashIndex;
    // find next free space
    while (m_hashtable[hashIndex] != NULL) {
        std::cout<<"inside while";
        counter++;
        hashIndex =((key%m_max_size) + (counter * (1 + (key%(m_max_size-1)))))%m_max_size;
    }
    m_hashtable[hashIndex] = data;
    m_curr_size++;
}

template <class V>
V* HashTable<V>::find(int Id) {
    int key=Id;
    int counter = 0;
    int hashIndex = ((key%m_max_size) + (counter * (1 + (key%(m_max_size-1)))))%m_max_size;
    while (m_hashtable[hashIndex] != NULL) {
        if (counter++ > m_max_size)
            return NULL;
        // if node found return its value
        if (m_hashtable[hashIndex]->getKey() == key)
            return m_hashtable[hashIndex];
        counter++;
        hashIndex =((key%m_max_size) + (counter * (1+(key%(m_max_size-1)))))%m_max_size;
    }
    return NULL;
}

#endif //WET2_HASHTABLE_H

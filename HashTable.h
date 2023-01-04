//
// Created by user on 04/01/2023.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

template <class V>
class HashTable{
    V** m_array;
    int m_max_size;
    int m_curr_size;
public:
    HashTable();
    ~HashTable();
    void rehash();
    void insert(V* data, int key);
    V* find(int Id);
};

template <class V>
HashTable<V>::HashTable() : m_array(new V*[2]),m_max_size(2), m_curr_size(0){
    for(int i=0;i<2;i++){
        m_array[i]=NULL;
    }
}

template <class V>
HashTable<V>::~HashTable() {
    for(int i = 0; i < m_max_size; i++){
        delete m_array[i];
    }
    delete[] m_array;
}

template <class V>
void HashTable<V>::rehash() {
    std::cout<<"rehashing";
    int old_max=m_max_size;
        m_max_size *= 2;
        V **new_table = new V *[m_max_size * sizeof(V*)];
        V **tmp_table = new V *[m_curr_size * sizeof(V*)];
        ////////////////////////////////copy old teams_table to tmp table
        for(int i = 0; i < old_max; i++) {
            tmp_table[i] = m_array[i];
        }
        //////////////////////////////delete old teams_table
        /*for(int i = 0; i < m_curr_size; i++) {
            delete m_array[i];
        }
        delete[] m_array;*/
        ////////////////////////////////assign new table as teams table
        m_array=new_table;
        ///////////////////////////////copy tmp table to new teams_table
        m_curr_size=0;
        for(int i = 0; i < m_max_size; i++) {
            m_array[i]=NULL;
        }
        for(int i = 0; i < old_max; i++) {
            insert(tmp_table[i], tmp_table[i]->getKey());
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
    while (m_array[hashIndex] != NULL) {
        std::cout<<"inside while";
        counter++;
        hashIndex =((key%m_max_size) + (counter * (1 + (key%(m_max_size-1)))))%m_max_size;
    }
    std::cout<<"hash index after while: " << hashIndex;
    m_array[hashIndex] = data;
    m_curr_size++;
    std::cout<<"max: " << m_max_size;
    std::cout<<"curr: " << m_curr_size;

}

template <class V>
V* HashTable<V>::find(int Id) {
    int key=Id;
    int counter = 0;
    int hashIndex = ((key%m_max_size) + (counter * (1 + (key%(m_max_size-1)))))%m_max_size;
    while (m_array[hashIndex] != NULL) {
        if (counter++ > m_max_size)
            return NULL;
        // if node found return its value
        if (m_array[hashIndex]->getKey() == key)
            return m_array[hashIndex];
        counter++;
        hashIndex =((key%m_max_size) + (counter * (1+(key%(m_max_size-1)))))%m_max_size;
    }
    return NULL;
}

#endif //WET2_HASHTABLE_H

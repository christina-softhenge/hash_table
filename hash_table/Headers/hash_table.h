#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <cstddef>

template <typename Key, typename Value>
class HashTable {
public:
    HashTable() {}
    ~HashTable(){}
    std::size_t size() const;
    void insert(const Key& key, const Value& val);
    Value& operator[](const Key& key);
    int hashFunction(const Key& key);
private:
    struct Node {
        Node(Key key, Value val) : pair{key,val}, next(nullptr) {}
        std::pair<Key,Value> pair;
        Node* next;
    };
    std::vector<Node*> m_HashTable;
};


#endif
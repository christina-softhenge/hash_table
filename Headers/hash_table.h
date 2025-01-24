#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <cstddef>

template <typename Key, typename Value>
class HashTable {
public:
    HashTable() {}

    ~HashTable() {
    for (int i = 0; i < m_HashTable.size(); ++i) {
            delete m_HashTable[i];
        }
    }

    std::size_t size() {
        return m_HashTable.size();
    }

    void insert(const Key& key, const Value& val) {
        for (Node* node : m_HashTable) {
            if (node->key == key) {
                node->val = val;
                return;
            }
        }
        m_HashTable.push_back(new Node(key,val));
    }

    Value& operator[](const Key& key) {
        for (Node* node : m_HashTable) {
            if (node->key == key) {
                return node->val;
            }
        }
    }
    
    bool find(const Key& key) {
            for (Node* node : m_HashTable) {
                if (node->key == key) {
                    return true;
                }
            }
            return false;
    }

private:
    struct Node{
        Node(Key key, Value val) : 
            key{key}, 
            val{val}, 
            next{nullptr} { }
        Key key;
        Value val;
        Node* next;
    };
    std::vector<Node*> m_HashTable;
};

#endif
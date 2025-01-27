#ifndef HASH_TABLE_STRING
#define HASH_TABLE_STRING

#include "hash_table.h"
#include <iostream>

template <typename Value>
class HashTable<std::string, Value> {
public:
    HashTable() : m_tableSize{17} {
        m_HashTable.resize(m_tableSize,nullptr);
    }

    ~HashTable() {
        for (Node* node : m_HashTable) {
            while(node) {
                Node* tmp = node->next;
                delete node;
                node = tmp;
            }
        }
    }

    std::size_t size() const {
        return m_HashTable.size();
    }


    void insert(const std::string& key, const Value& val) {
        int hashValue = hashFunction(key);
        Node* newNode = new Node(key, val);
        newNode->next = m_HashTable[hashValue];
        m_HashTable[hashValue] = newNode;
    }

    void print() {
        for (int i = 0; i < m_tableSize; ++i) {
            Node* n = m_HashTable[i];
            std::cout << i << ": ";
            while (n) {
                std::cout  << n->pair.first << " " << n->pair.second << "  ";
                n = n->next;
            }
            std::cout << std::endl;
        }
    }

    Value& operator[](const std::string& key) {
        int hashValue = hashFunction(key);
        Node* node = m_HashTable[hashValue];
        while (node) {
            if (node->pair.first == key) {
                return node->pair.second;
            } 
            node = node->next;
        }
        node = new Node(key,0);
        m_HashTable[hashValue] = node;
        return node->pair.second;
    }


    int hashFunction(const std::string& key) {
        return key.size() % m_tableSize;
    }

    private:
    struct Node {
        Node(std::string key, Value val) : pair{key,val}, next(nullptr) {}
        std::pair<std::string,Value> pair;
        Node* next;
    };
    int m_tableSize;
    std::vector<Node*> m_HashTable;

    public:
        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Node; 
            using pointer = value_type*;
            using reference = value_type&;

            Iterator(pointer ptr, HashTable* table) : m_ptr{ptr}, m_table{table} {}
            reference operator*() { return &m_ptr; }
            pointer operator->() { return m_ptr; }

            operator bool() const{
                return m_ptr != nullptr;
            }

            Iterator& operator++() {
                if (!m_ptr) {
                    return *this;
                }
                if (m_ptr->next) {
                    m_ptr = m_ptr->next;
                    return *this;
                }
                for (int i = m_table->hashFunction(m_ptr->pair.first) + 1; i < m_table->m_tableSize; ++i) {
                    if (m_table->m_HashTable[i]) {
                        m_ptr = m_table->m_HashTable[i];
                        return *this;
                    }
                }
                m_ptr = nullptr;
                return *this;
            }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        private:
            pointer m_ptr;
            HashTable* m_table; 
    };

    Iterator begin() {
        for (Node* node : m_HashTable) {
            if (node) {
                return Iterator(node,this);
            }
        }
        return Iterator(nullptr, this);
    }

    Iterator end() {
        return Iterator(*m_HashTable.end(), this); 
    }

    Iterator find(int key) {
        for (Iterator it = begin(); it != end(); ++it) {
            if (it->pair.first == key) {
                return it;
            }
        }
        return end();
    }
};

#endif
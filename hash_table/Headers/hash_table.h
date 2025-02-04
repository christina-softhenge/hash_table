#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>

template <typename Key>
struct Hash {
    std::size_t operator()(const Key& key, int hashTableSize) const {
        std::cout << "hash main\n";
        return std::hash<Key>{}(key) % hashTableSize;
    }
};

template <>
struct Hash<std::string> {
    std::size_t operator()(const std::string& key, int hashTableSize) const {
        int sum{};
        for (char ch : key) {
            sum += static_cast<int>(ch);
        }
        return sum % hashTableSize;
    }
};

template<>
struct Hash<int> {
    std::size_t operator()(const int& key, int hashTableSize) const {
        std::cout << "hash int\n";
        return key % hashTableSize;
    }
};

template <typename Key, typename Value, typename HashFunction = Hash<Key>>
class HashTable {
public:
    HashTable() : 
    m_tableSize{17}, 
    m_size{} 
    {
        m_HashTable.resize(m_tableSize,nullptr);
    }

    ~HashTable() 
    {
        for (Node* node : m_HashTable) {
            while(node) {
                Node* tmp = node->next;
                delete node;
                node = tmp;
            }
        }
    }

    float load_factor() 
    {
        return float(m_size)/m_HashTable.size();
    }

    
    void resize() 
    {
        std::vector<Node*> newTable;
        newTable.resize(m_tableSize*2);
        int oldSize = m_tableSize;
        m_tableSize *= 2; 
        for (int i = 0; i < oldSize; ++i) {
            Node* node = m_HashTable[i];
            while (node) {
                int hashValue = hashFunction(node->pair.first, m_tableSize);
                Node* newNode = new Node(node->pair);
                newNode->next = newTable[hashValue];
                newTable[hashValue] = newNode;
                node = node->next;
            }
        }
        for (int i = 0; i < oldSize; ++i) {
            Node* node = m_HashTable[i];
            while (node) {
                Node* tmp = node->next;
                delete node;
                node = tmp;
            }
        }
        m_HashTable = newTable;
    }

    void insert(const Key& key, const Value& val) 
    {
        if (load_factor() > 0.7) {
            resize();
        }
        ++m_size;
        int hashValue = hashFunction(key, m_tableSize);
        Node* newNode = new Node(key, val);
        newNode->next = m_HashTable[hashValue];
        m_HashTable[hashValue] = newNode;
    }

    void print() 
    {
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

    Value& subscriptHelper(const Key& key, int hashValue) 
    {
        Node* node = m_HashTable[hashValue];
        while (node) {
            if (node->pair.first == key) {
                return node->pair.second;
            } 
            node = node->next;
        }
        Node* tablesNode = m_HashTable[hashValue];
        node = new Node(key,0);
        node->next = tablesNode;
        m_HashTable[hashValue] = node;
        ++m_size;
        return node->pair.second;
    }

    Value& operator[](const Key& key) 
    {
        if (load_factor() > 0.7) {
            resize();
        }
        int hashValue = hashFunction(key, m_tableSize);
        return subscriptHelper(key,hashValue);
    }

    int getTableSize() 
    {
        return m_tableSize;
    }
private:
    struct Node {
        Node(Key key, Value val) : pair{key,val}, next(nullptr) {}
        Node(std::pair<Key,Value> valPair) : pair{valPair}, next{nullptr} {}
        std::pair<Key,Value> pair;
        Node* next;
    };
    int m_size;
    int m_tableSize;
    std::vector<Node*> m_HashTable;
    HashFunction hashFunction;

public:
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Node;
        using pointer = value_type *;
        using reference = value_type &;

        Iterator(pointer ptr, HashTable *table) : 
        m_ptr{ptr}, 
        m_table{table} {}
        reference operator*() { return &m_ptr; }
        pointer operator->() { return m_ptr; }

        operator bool() const
        {
            return m_ptr != nullptr;
        }

        bool operator==(const Iterator &oth)
        {
            if (this->m_ptr == oth.m_ptr) {
                return true;
            }
            return false;
        }

        bool operator!=(const Iterator &oth)
        {
            if (this->m_ptr != oth.m_ptr) {
                return true;
            }
            return false;
        }

        Iterator &operator++()
        {
            if (!m_ptr) {
                return *this;
            }
            if (m_ptr->next) {
                m_ptr = m_ptr->next;
                return *this;
            }
            for (int i = m_table->hashFunction(m_ptr->pair.first, m_table->m_tableSize) + 1; i < m_table->m_tableSize; ++i) {
                if (m_table->m_HashTable[i]) {
                    m_ptr = m_table->m_HashTable[i];
                    return *this;
                }
            }
            m_ptr = nullptr;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

    public:
        pointer m_ptr;
        HashTable *m_table;
    };

    Iterator begin()
    {
        for (Node* node : m_HashTable) {
            if (node) {
                return Iterator(node,this);
            }
        }
        return Iterator(nullptr, this);
    }

    Iterator end()
    {
        return Iterator(*m_HashTable.end(), this); 
    }

    Iterator find(const Key& key) 
    {
        for (Iterator it = this->begin(); it != this->end(); ++it) {
            if (it->pair.first == key) {
                return it;
            }
        }
        return end();
    }
};


#endif
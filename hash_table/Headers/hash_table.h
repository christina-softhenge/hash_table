#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>

template <typename Key>
struct Hash {
    std::size_t operator()(const Key& key) const {
        return std::hash<Key>{}(key) ;
    }
};

template <>
struct Hash<std::string> {
    std::size_t operator()(const std::string& key) const {
        int sum{};
        for (char ch : key) {
            sum += static_cast<int>(ch);
        }
        return sum;
    }
};

template<>
struct Hash<int> {
    std::size_t operator()(const int& key) const {
        return key;
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

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            clear();
            for (auto node : other.m_HashTable) {
                while (node) {
                    this->insert(node->pair);
                    node = node->next;
                }
            }
        }
        return *this;
    }
    
    int size() {
        return m_size;
    }

    bool empty() {
        return !static_cast<bool>(m_size);
    }

    void insert(const Key& key, const Value& val) 
    {
        if (load_factor() > 0.7) {
            resize();
        }
        ++m_size;
        int hashValue = hashFunction(key);
        Node* newNode = new Node(key, val);
        newNode->next = m_HashTable[hashValue];
        m_HashTable[hashValue] = newNode;
    }

    void insert(std::pair<Key,Value> pair) {
        insert(pair.first,pair.second);
    }

    bool erase(const Key& key) {
        int hashVal = hashFunction(key);
        Node* node = m_HashTable[hashVal];
        if (node->pair.first == key) {
            m_HashTable[hashVal] = node->next;
            delete node;
            --m_size;
            return 1;
        }
        Node* prev = node;
        while (prev->next) {
            node = prev->next;
            if (node->pair.first == key) {
                prev->next = node->next;
                delete node;
                --m_size;
                return 1;
            }
            prev = node;
        }
        return 0;
    }

    
    void clear() {
        for (Node* node : m_HashTable) {
            while(node) {
                Node* tmp = node->next;
                delete node;
                node = tmp;
            }
        }
        m_tableSize = 17;
        m_HashTable.clear();
        m_HashTable.assign(m_tableSize, nullptr);
        m_size = 0;
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

    Value& operator[](const Key& key) 
    {
        if (load_factor() > 0.7) {
            resize();
        }
        int hashValue = hashFunction(key);
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

private:  //private methods
    int hashFunction(const Key& key) 
    {
        return m_hashFunctionHelper(key) % m_tableSize;
    }

    float load_factor() 
    {
        return float(m_size) / m_HashTable.size();
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
                int hashValue = hashFunction(node->pair.first);
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

private:    //member data
    struct Node {
        Node(Key key, Value val) : pair{key,val}, next(nullptr) {}
        Node(std::pair<Key,Value> valPair) : pair{valPair}, next{nullptr} {}
        std::pair<const Key,Value> pair;
        Node* next;
    };
    int m_size;
    int m_tableSize;
    std::vector<Node*> m_HashTable;
    HashFunction m_hashFunctionHelper;

public:
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<const Key, Value>;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(Node *ptr, HashTable *table) : 
        m_ptr{ptr}, 
        m_table{table} {}
        Iterator(const Iterator& oth) {
            this->m_ptr = oth.m_ptr;
            this->m_table = oth.m_table;
        }
        reference operator*() { return m_ptr->pair; }
        pointer operator->() { return &m_ptr->pair; }

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
            return ! (*this == oth);
        }

        Iterator& operator++()
        {
            if (!m_ptr) {
                *this = m_table->end();
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
            *this = m_table->end();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

    private:
        Node* m_ptr;
        HashTable *m_table;
    };

    Iterator begin()
    {
        for (Node* node : m_HashTable) {
            if (node) {
                return Iterator(node,this);
            }
        }
        return end();
    }

    Iterator end()
    {
        return Iterator(m_HashTable[m_HashTable.size()], this); 
    }

    Iterator find(const Key& key) 
    {
        for (Iterator it = this->begin(); it != this->end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return end();
    }
    
    Iterator erase (Iterator pos) {
        int key = pos->pair.first;
        int hashValue = hashFunction(key);
        Node* node = m_HashTable[hashValue];
        if (node->pair.first == key) {
            Iterator nextIt = ++Iterator(node,this); 
            m_HashTable[hashValue] = node->next;
            delete node;
            --m_size;
            return nextIt;
        }
        Node* prev = node;
        while (prev->next) {
            node = prev->next;
            if (node->pair.first == key) {
                Iterator nextIt = ++Iterator(node,this); 
                prev->next = node->next;
                delete node;
                --m_size;
                return nextIt;
            }
            prev = node;
        }
        return end();
    }

    Iterator erase(Iterator first, Iterator last) {
        Iterator it = first;
        while (it != last && it != end()) {
            it = erase(it);
        }
        return last;
    }
};


#endif
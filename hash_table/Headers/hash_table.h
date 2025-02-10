#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <functional>

//default hashing function struct
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
    HashTable();
    HashTable(const HashTable& oth);
    ~HashTable();

    HashTable& operator=(const HashTable& other);
    Value& operator[](const Key& key);
    
    int size() const;
    bool empty() const;
    void insert(const Key& key, const Value& val);
    void insert(std::pair<Key,Value> pair);
    bool erase(const Key& key);
    void clear();
    void print() const;

private: //private methods
    int hashFunction(const Key& key);
    float load_factor();
    void resize();

private: //member data
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

public: //Iterator's declaration
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<const Key, Value>;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(Node *ptr, HashTable *table);
        Iterator(const Iterator& oth);

        reference operator*();
        pointer operator->();
        operator bool() const;
        bool operator==(const Iterator &oth) const;
        bool operator!=(const Iterator &oth) const;
        Iterator& operator++();
        Iterator operator++(int);

    private:
        Node* m_ptr;
        HashTable *m_table;
    };
    
public: //Iterator returning functions
    Iterator begin();
    Iterator end();
    Iterator find(const Key& key);
    Iterator erase (Iterator pos);
    Iterator erase(Iterator first, Iterator last);
};

#include "hash_table.tpp"

#endif
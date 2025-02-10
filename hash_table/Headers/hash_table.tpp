
template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>::HashTable() : 
    m_tableSize{17}, 
    m_size{} 
{
    m_HashTable.resize(m_tableSize, nullptr);
}


template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>::HashTable(const HashTable& oth) : 
        m_tableSize{oth.m_tableSize},
        m_size{oth.m_size},
        m_hashFunctionHelper{oth.m_hashFunctionHelper}
{
    this->m_HashTable.resize(m_tableSize, nullptr);
    for (int i = 0; i < oth.m_tableSize; ++i) {
        Node* node = oth.m_HashTable[i];
        while (node) {
            Node* newNode = new Node(node->pair);
            newNode->next = this->m_HashTable[i];
            this->m_HashTable[i] = newNode;
            node = node->next;
        }
    }
}

template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>::~HashTable()
{
    for (auto node : m_HashTable) {
        while(node) {
            Node* tmp = node->next;
            delete node;
            node = tmp;
        }
    }
}    

template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>& HashTable<Key, Value, HashFunction>::operator=(const HashTable& other)
{
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

template <typename Key, typename Value, typename HashFunction>
Value& HashTable<Key, Value, HashFunction>::operator[](const Key& key) 
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

template <typename Key, typename Value, typename HashFunction>
int HashTable<Key, Value, HashFunction>::size() const
{
    return m_size;
}

template <typename Key, typename Value, typename HashFunction>
bool HashTable<Key, Value, HashFunction>::empty() const
{
    return !static_cast<bool>(m_size);
}

template <typename Key, typename Value, typename HashFunction>
void HashTable<Key, Value, HashFunction>::insert(const Key& key, const Value& val) 
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

template <typename Key, typename Value, typename HashFunction>
void HashTable<Key, Value, HashFunction>::insert(std::pair<Key,Value> pair) 
{
    insert(pair.first,pair.second);
}

template <typename Key, typename Value, typename HashFunction>
bool HashTable<Key, Value, HashFunction>::erase(const Key& key)
{
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

template <typename Key, typename Value, typename HashFunction>
void HashTable<Key, Value, HashFunction>::clear() 
{
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

template <typename Key, typename Value, typename HashFunction>
void HashTable<Key, Value, HashFunction>::print() const
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

//private methods
template <typename Key, typename Value, typename HashFunction>
int HashTable<Key, Value, HashFunction>::hashFunction(const Key& key)
{
    return m_hashFunctionHelper(key) % m_tableSize;
}

template <typename Key, typename Value, typename HashFunction>
float HashTable<Key, Value, HashFunction>::load_factor() 
{
    return float(m_size) / m_HashTable.size();
}

template <typename Key, typename Value, typename HashFunction>
void HashTable<Key, Value, HashFunction>::resize() 
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

//Itesrator's functions
template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>::Iterator::Iterator(Node *ptr, HashTable *table) 
    : m_ptr{ptr}, m_table{table} 
{}

template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>::Iterator::Iterator(const Iterator& oth)
{
    this->m_ptr = oth.m_ptr;
    this->m_table = oth.m_table;    
}

template <typename Key, typename Value, typename HashFunction>
std::pair<const Key, Value>& HashTable<Key, Value, HashFunction>::Iterator::operator*() 
{ 
    return m_ptr->pair; 
}

template <typename Key, typename Value, typename HashFunction>
std::pair<const Key, Value>* HashTable<Key, Value, HashFunction>::Iterator::operator->() 
{ 
    return &m_ptr->pair; 
}

template <typename Key, typename Value, typename HashFunction>
HashTable<Key, Value, HashFunction>::Iterator::operator bool() const         
{
    return m_ptr != nullptr;
}

template <typename Key, typename Value, typename HashFunction>
bool HashTable<Key, Value, HashFunction>::Iterator::operator==(const Iterator &oth) const
{
    return this->m_ptr == oth.m_ptr;
}

template <typename Key, typename Value, typename HashFunction>
bool HashTable<Key, Value, HashFunction>::Iterator::operator!=(const Iterator &oth) const
{
    return ! (*this == oth);
}

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key,Value,HashFunction>::Iterator& 
HashTable<Key, Value, HashFunction>::Iterator::operator++()
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

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key,Value,HashFunction>::Iterator
HashTable<Key, Value, HashFunction>::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

//Iterator related member functions

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key, Value, HashFunction>::Iterator
HashTable<Key, Value, HashFunction>::begin() 
{
    for (Node* node : m_HashTable) {
        if (node) {
            return Iterator(node,this);
        }
    }
    return end();
}

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key, Value, HashFunction>::Iterator
HashTable<Key, Value, HashFunction>::end()
{
    return Iterator(nullptr, this); 
}

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key, Value, HashFunction>::Iterator
HashTable<Key, Value, HashFunction>::find(const Key& key) 
{
    for (Iterator it = this->begin(); it != this->end(); ++it) {
        if (it->first == key) {
            return it;
        }
    }
    return end();
}

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key, Value, HashFunction>::Iterator
HashTable<Key, Value, HashFunction>::erase (Iterator pos) 
{
    int key = pos->first;
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

template <typename Key, typename Value, typename HashFunction>
typename HashTable<Key, Value, HashFunction>::Iterator
HashTable<Key, Value, HashFunction>::erase(Iterator first, Iterator last) 
{
    Iterator it = first;
    while (it != last && it != end()) {
        it = erase(it);
    }
    return last;
}
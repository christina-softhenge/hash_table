#include <iostream>
#include "hash_table.h"

int main() {
    HashTable<int,int> map;
    map.insert(4,3);
    std::cout << map[4] << '\n';
    std::cout << map.find(4) << " " << map.find(5) << " " << map.size() << '\n';
}
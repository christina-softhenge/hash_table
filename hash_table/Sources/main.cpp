#include <iostream>
#include "Headers/hash_table.h"
#include "unordered_map"

int main() {
    HashTable<int,int> map;
    map.insert(4,3);
    std::cout << map[4] << '\n';
    std::cout << map.find(4) << " " << map.find(5) << " " << map.size() << '\n';
}
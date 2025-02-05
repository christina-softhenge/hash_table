#include <iostream>
#include "Headers/hash_table.h"
#include "unordered_map"

int main() {
    HashTable<int,int> map;
    map.insert(4,3);
    map.insert(21,2);
    map[65] = 4;
    map[43] = 3;
    map[13] = 75;
    map[38] = 43;
    map.erase(4);
    map.print();
    std::cout << "number of elements: " << map.size() << std::endl; 
    map.clear();
    map.print();
    std::cout << "isEmpty: " << map.empty() << std::endl; 
}
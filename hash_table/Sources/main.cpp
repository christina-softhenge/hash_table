#include <iostream>
#include "Headers/hash_table.h"
#include "unordered_map"

int main() {
    HashTable<int,int> map;
    map.insert(4,3);
    map.insert(21,2);
    if (map.begin()) {
        std::cout << "non null\n";
    } else {
        std::cout << "null\n";
    }
    std::cout << map.begin()->pair.first << '\n';
    std::cout << map[4] << ' ' << map[21] << ' ' << map[7] << '\n';
    map.print();
    map.end();
    map.find(21);
    return 0;
}
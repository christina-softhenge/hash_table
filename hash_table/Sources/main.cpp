#include <iostream>
#include "Headers/hash_table_int.h"
#include "Headers/hash_table_string.h"
#include "unordered_map"

int main() {
    HashTable<int,int> map;
    map.insert(4,3);
    map.insert(21,2);
    std::cout << map.begin()->pair.first << '\n';
    // std::cout << map[4] << ' ' << map[21] << ' ' << map[7] << '\n';
    // map.print();
    map.end();
    map.find(21);

    HashTable<std::string,int> mapStr;
    mapStr["Krist"] = 4;
    mapStr["Nare"] = 2;
    mapStr.print();
    return 0;
}
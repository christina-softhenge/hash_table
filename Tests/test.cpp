#include <gtest/gtest.h>
#include "Headers/hash_table_int.h"
#include "Headers/hash_table_string.h"
#include <chrono>

int SubscriptTest(int a, int b) {
    HashTable<int,int> map;
    map[a] = b;
    return map[a];
}

HashTable<int,int> makeRandomHashMap(int num_elements, std::vector<int>& keys) {
    HashTable<int,int> hashMap;
    keys.reserve(num_elements);
    for (int i = 0; i < num_elements; ++i) {
        int key = rand()%100000;
        keys.push_back(key);
        hashMap[key] = i;
    }
    return hashMap;
}

void LookupTimeCheck(int num_elements) {
    std::vector<int> keys;
    HashTable<int,int> hashMap = makeRandomHashMap(num_elements, keys);
    int lookups = 10000;
    int value;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < lookups; ++i) {
        value = hashMap[keys[rand() % num_elements]];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::micro>(end - start).count() / lookups;
    std::cout << "Lookup duration: " << duration << std::endl; 
}

void InsetTimeCheck(int num_elements) {
    HashTable<int,int> hashMap;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_elements; ++i) {
        int key = rand()%100000;
        hashMap[key] = i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::micro>(end - start).count() / num_elements;
    std::cout << "Insert duration: " << duration << std::endl; 
}

TEST(TimeCheckingTest,HandlesTimeCheck) {
    LookupTimeCheck(1000000);
    InsetTimeCheck(1000000);
}

int SubscriptTestString(std::string a, int b) {
    HashTable<std::string,int> map;
    map[a] = b;
    return map[a];
}

int FindTest(HashTable<int,int>& map, int a) {
    auto iter = map.find(a);
    if (iter == map.end()) { 
        std::cout << "out of range\n";
        return 0;
    }
    return iter->pair.second;
}

int FindTestString(HashTable<std::string,int>& map, const std::string& key) {
    auto iter = map.find(key);
    if (iter == map.end()) { 
        std::cout << "out of range\n";
        return 0;
    }
    return iter->pair.second;
}

TEST(SubscriptionTest,HandlesSubscription) {
    EXPECT_EQ(SubscriptTest(2,3),3);
    EXPECT_EQ(SubscriptTest(-2,-3),-3);
}

TEST(SubscriptionTestString,HandlesStringSubscription) {
    EXPECT_EQ(SubscriptTestString("Ani",3),3);
    EXPECT_EQ(SubscriptTestString("Arman",-3),-3);
}

TEST(FindAndInsertTest,HandlesFindAndInsert) {
    HashTable<int,int> mapint;
    for (int i = 0; i < 100; ++i) {
        mapint.insert(i,i);
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(FindTest(mapint,i),i);
    }
}

TEST(StringFindAndInsertTest,HandlesStringFindAndInsert) {
    HashTable<std::string,int> mapString;
    for (int i = 0; i < 100; ++i) {
        mapString.insert(std::to_string(i),i);
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(FindTestString(mapString,std::to_string(i)),i);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>
#include "Headers/hash_table_int.h"
#include "Headers/hash_table_string.h"

int SubscriptTest(int a, int b) {
    HashTable<int,int> map;
    map[a] = b;
    return map[a];
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
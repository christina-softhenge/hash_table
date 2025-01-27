#include <gtest/gtest.h>
#include "Headers/hash_table_int.h"

int SubscriptTest(int a, int b) {
    HashTable<int,int> map;
    map[a] = b;
    return map[a];
}

bool FindTest(HashTable<int,int>& map, int a) {
    return map.find(a);
}

TEST(SubscriptionTest,HandlesSubscription) {
    EXPECT_EQ(SubscriptTest(2,3),3);
    EXPECT_EQ(SubscriptTest(-2,-3),-3);
}
TEST(SubscriptionTest,HandlesFindAndInsert) {
    HashTable<int,int> map;
    map.insert(4,5);
    map.insert(3,6);
    map.insert(9,10);
    EXPECT_EQ(FindTest(map, 4), true);
    EXPECT_EQ(FindTest(map, 75), false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
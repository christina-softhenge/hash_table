#include <gtest/gtest.h>
#include "Headers/hash_table.h"
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

double LookupTimeCheck(int num_elements) {
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
    std::cout << "Lookup duration in element number of: " << num_elements << " is: " << duration << std::endl; 
    return duration;
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
    std::cout << "Insert duration in element number of: " << num_elements << " is: " << duration << std::endl; 
}

TEST(TimeCheckingTest,HandlesTimeCheck) {
    for (int i = 10,j = 1000000; i < 10000, j >= 10000; i*=10,j/=10) {
        double res1 = LookupTimeCheck(i);
        double res2 = LookupTimeCheck(j);
        double diff = std::abs(res1-res2);
        EXPECT_NEAR(diff,0,0.2);
    }
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
    return iter->second;
}

int FindTestString(HashTable<std::string,int>& map, const std::string& key) {
    auto iter = map.find(key);
    if (iter == map.end()) { 
        std::cout << "out of range\n";
        return 0;
    }
    return iter->second;
}

void test_stl_algorithms() {
    HashTable<std::string, int> ht;
    
    std::vector<std::pair<std::string, int>> initial_data = {
        {"apple", 1}, {"banana", 2}, {"cherry", 3},
        {"date", 4}, {"elderberry", 5}, {"fig", 6}
    };
    
    for (const auto& pair : initial_data) {
        ht.insert(pair);
    }

    int i = 0;
    for (auto it = ht.begin(); it != ht.end(); ++it) {
        std::cout << "it" << it->first << std::endl;
        if (++i > 10) {
            break;
        }
    }
    
    {
        auto endIT = ht.end();
        bool all_positive = std::all_of(ht.begin(), endIT,
            [](const auto& pair) { return pair.second > 0; });
        assert(all_positive);
        
        bool has_even = std::any_of(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second % 2 == 0; });
        assert(has_even);
        
        bool none_negative = std::none_of(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second < 0; });
        assert(none_negative);
        
        auto count_even = std::count_if(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second % 2 == 0; });
        assert(count_even > 0);
        
        auto it = std::find_if(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second == 3; });
        assert(it != ht.end() && it->first == "cherry");
    }
    
    {
        std::vector<std::pair<std::string, int>> target;
        
        std::copy_if(ht.begin(), ht.end(), std::back_inserter(target),
            [](const auto& pair) { return pair.second % 2 == 0; });
        
        std::vector<int> values;
        std::transform(ht.begin(), ht.end(), std::back_inserter(values),
            [](const auto& pair) { return pair.second * 2; });
        
        std::vector<std::pair<std::string, int>> modified;
        std::replace_copy_if(ht.begin(), ht.end(), std::back_inserter(modified),
            [](const auto& pair) { return pair.second > 3; },
            std::make_pair(std::string("high"), 999));
    }
    
    {
        std::vector<std::pair<std::string, int>> sorted(ht.begin(), ht.end());
        
        std::sort(sorted.begin(), sorted.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
        
        bool is_sorted = std::is_sorted(sorted.begin(), sorted.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
        assert(is_sorted);
        
        std::vector<std::pair<std::string, int>> top3(3);

        std::partial_sort_copy(ht.begin(), ht.end(),
            top3.begin(), top3.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

    
    {
        int sum = std::accumulate(ht.begin(), ht.end(), 0,
            [](int total, const auto& pair) { return total + pair.second; });
        
        std::vector<int> weights(ht.size(), 2);
        int weighted_sum = std::inner_product(
            ht.begin(), ht.end(), weights.begin(), 0,
            std::plus<>(),
            [](const auto& pair, int weight) { return pair.second * weight; });
    }
    
    {
        std::vector<std::pair<std::string, int>> vec(ht.begin(), ht.end());
        std::sort(vec.begin(), vec.end());
        
        bool found = std::binary_search(vec.begin(), vec.end(),
            std::make_pair(std::string("cherry"), 3));
        
        auto range = std::equal_range(vec.begin(), vec.end(),
            std::make_pair(std::string("cherry"), 3));
    }
    
    std::cout << "All STL algorithm tests passed!\n";
    }
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

TEST(AlgorithmsTest, HandlesSTLAlgorithms) {
    test_s
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
#include <gtest/gtest.h>
#include "Headers/hash_table.h"
#include <chrono>
#include <iostream>
#include <numeric>

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

//conditional algorithms
bool test_all_of(HashTable<std::string,int> ht) {
    return std::all_of(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second > 0; });
}

bool test_any_of(HashTable<std::string,int> ht) {
    return std::any_of(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second % 2 == 0; });
}

bool test_none_of(HashTable<std::string,int> ht) {
    return std::none_of(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second < 0; });
}

auto test_count_if(HashTable<std::string,int> ht) {
    return std::count_if(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second % 2 == 0; });
}

auto test_find_if(HashTable<std::string,int> ht) {
    return std::find_if(ht.begin(), ht.end(),
            [](const auto& pair) { return pair.second == 3; });
}

//copying algorithms
auto test_copy_if(HashTable<std::string,int> ht) {
    std::vector<std::pair<std::string, int>> target;    
    std::copy_if(ht.begin(), ht.end(), std::back_inserter(target),
        [](const auto& pair) { return pair.second % 2 == 0; });
    return target; 
}

auto test_transform(HashTable<std::string,int> ht) {
    std::vector<int> values;
    std::transform(ht.begin(), ht.end(), std::back_inserter(values),
        [](const auto& pair) { return pair.second * 2; });
    return values;
}

auto test_replace_copy_if(HashTable<std::string,int> ht) {
    std::vector<std::pair<std::string, int>> modified;
    std::replace_copy_if(ht.begin(), ht.end(), std::back_inserter(modified),
        [](const auto& pair) { return pair.second > 3; },
        std::make_pair(std::string("high"), 999));
    return modified;
}

//sorting algorithms
auto test_sort(HashTable<std::string,int> ht) {
    std::vector<std::pair<std::string, int>> sorted(ht.begin(), ht.end());
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    return sorted;
}

auto test_partial_sort_copy(HashTable<std::string,int> ht) {
    std::vector<std::pair<std::string, int>> top3(3);
    std::partial_sort_copy(ht.begin(), ht.end(),
            top3.begin(), top3.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
    return top3;
}

//accumulation algorithms from <numeric>
int test_accumulate(HashTable<std::string,int> ht) {
    int sum = std::accumulate(ht.begin(), ht.end(), 0,
    [](int total, const auto& pair) { return total + pair.second; });
    return sum;
}

int test_weighted_sum(HashTable<std::string,int> ht) {
    std::vector<int> weights(ht.size(), 2);
    int weighted_sum = std::inner_product(
        ht.begin(), ht.end(), weights.begin(), 0,
        std::plus<>(),
        [](const auto& pair, int weight) { return pair.second * weight; });
    return weighted_sum;
}

//helper function
void fillHashTable(HashTable<std::string, int>& ht) {
    std::vector<std::pair<std::string, int>> initial_data = {
        {"apple", 1}, {"banana", 2}, {"cherry", 3},
        {"date", 4}, {"elderberry", 5}, {"fig", 6}
    };
    
    for (const auto& pair : initial_data) {
        ht.insert(pair);
    }
}

//passing hashfunction for the custom class
struct Point {  //the custom class
    Point(double dval, int ival) 
    : d{dval}
    , i{ival} { }
    friend std::ostream& operator<<(std::ostream& os, const Point& pt);
    bool operator==(const Point& oth) const {
        return (this->d == oth.d) && (this->i == oth.i);
    }
    bool operator!=(const Point& oth) {
        return ! (this == &oth);
    }
    int getI() const {
        return i;
    }
private:
    double d;
    int i;
};

std::ostream& operator<<(std::ostream& os, const Point& pt) {
    os << pt.d << " " << pt.i;
    return os;
}

//the custom hashing functor Point class 
template<typename Key>
struct PointHash {  
    std::size_t operator()(const Key& key) {
        return 0;
    }
};

template <>
struct PointHash<Point> {   
    std::size_t operator()(const Point& point) {
        return point.getI();
    }
};



//testing basic hashmap operations
TEST(SubscriptionTest, HandlesSubscription) {
    EXPECT_EQ(SubscriptTest(2,3),3);
    EXPECT_EQ(SubscriptTest(-2,-3),-3);
}

TEST(SubscriptionTestString, HandlesStringSubscription) {
    EXPECT_EQ(SubscriptTestString("Ani",3),3);
    EXPECT_EQ(SubscriptTestString("Arman",-3),-3);
}

TEST(FindAndInsertTest, HandlesFindAndInsert) {
    HashTable<int,int> mapint;
    for (int i = 0; i < 100; ++i) {
        mapint.insert(i,i);
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(FindTest(mapint,i),i);
    }
}

TEST(StringFindAndInsertTest, HandlesStringFindAndInsert) {
    HashTable<std::string,int> mapString;
    for (int i = 0; i < 100; ++i) {
        mapString.insert(std::to_string(i),i);
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(FindTestString(mapString,std::to_string(i)),i);
    }
}

//lookup time checking test
TEST(TimeCheckingTest, HandlesTimeCheck) {
    double timeFirst = LookupTimeCheck(100);
    double timeSecond = LookupTimeCheck(1000000);
    double diff = std::abs(timeFirst - timeSecond);
    EXPECT_NEAR(diff,0,0.2);

    double timeFirst2 = LookupTimeCheck(100);
    double timeSecond2 = LookupTimeCheck(1000000);
    double diff2 = std::abs(timeFirst2 - timeSecond2);
    EXPECT_NEAR(diff2,0,0.2);
}

//testing stl algorithms 
TEST(STLConditionals, HandlesConditionalAlgorithms) {
    HashTable<std::string, int> ht;
    fillHashTable(ht);

    EXPECT_EQ(test_all_of(ht),1);
    EXPECT_EQ(test_any_of(ht),1);
    EXPECT_EQ(test_none_of(ht),1);
    EXPECT_EQ(test_count_if(ht),3);
    auto it = test_find_if(ht);
    EXPECT_EQ(it->second, 3);
}

TEST(STLCopying, HandlesCopyingAlgorithms) {
    HashTable<std::string, int> ht;
    fillHashTable(ht);

    std::vector<std::pair<std::string, int>> target = test_copy_if(ht);
    EXPECT_EQ(target.size(),3);

    std::vector<int> values = test_transform(ht);
    std::sort(values.begin(), values.end());
    EXPECT_EQ(values[0],2);
    EXPECT_EQ(values[5],12);

    std::vector<std::pair<std::string, int>> modified = test_replace_copy_if(ht);
    int replaced_count{};
    for(const auto& pair : modified) {
        if(pair.first == "high" && pair.second == 999) {
            replaced_count++;
        }
    }
    EXPECT_EQ(replaced_count,3);
}

TEST(STLSorting, HandlesSortingAlgorithms) {
    HashTable<std::string, int> ht;
    fillHashTable(ht);

    auto sorted = test_sort(ht);
    EXPECT_EQ(sorted[0].second,1);
    EXPECT_EQ(sorted[5].second,6);

    auto top3 = test_partial_sort_copy(ht);
    EXPECT_EQ(top3[0].second,6);
    EXPECT_EQ(top3[2].second,4);
}

TEST(STLAccumulation, HandlesAccumulationAlgorithms) {
    HashTable<std::string, int> ht;
    fillHashTable(ht);

    int sum = test_accumulate(ht);
    EXPECT_EQ(sum, 21);

    int weighted_sum = test_weighted_sum(ht);
    EXPECT_EQ(weighted_sum,42);
}

//custom hash function test
TEST(PassingCustomHashFunctionTest, HandlesCustomHashFunction) {
    HashTable<Point,int, PointHash<Point>> hPoint;
    Point p1(4.34,5);
    hPoint[p1] = 4;

    Point p2(3.21,6);
    hPoint[p2] = 7;
    std::cout << "p2 second " << hPoint[p2];
    EXPECT_EQ(hPoint[p2],7);   
    EXPECT_EQ(hPoint[p1],4);   
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
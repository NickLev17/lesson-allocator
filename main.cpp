#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "MyAllocator.h"
#include "MyVector.h"
using namespace std;

int factorial(int value) {
    int result = 1;
    for (int i = 2; i <= value; i++) {
        result *= i;
    }
    return result;
}

int main() {
    // std MAP
    std::map<int, int> std_map;
    for (int i = 0; i < 10; i++) {
        std_map.insert({i, factorial(i)});
    }

    // for (auto &[k, v] : std_map) {
    //     cout << k << " " << v << "\n";
    // }

    // Custom ALLOCATOR
    using Key = int;
    using Value = int;
    using PairType = std::pair<const Key, Value>;

    MyAllocator<PairType> alloc(10);
    std::map<Key, Value, std::less<Key>, MyAllocator<PairType>> myMap(alloc);

    for (int i = 0; i < alloc.MAX_SIZE; i++) {
        myMap.insert({i, factorial(i)});
    }

    for (auto &[k, v] : myMap) {
        cout << k << " " << v << "\n";
    }

    // MyVector WITH INT
    MyVector<int> myVec;
    for (int i = 0; i < 10; i++) {
        myVec.push_back(i);
    }

    // for (const auto &v : myVec) {
    //   cout << v << "\n";
    // }

    // CUSTOM MyVector WITH CUSTOM ALLOCATOR
    MyAllocator<int> allocat(10);
    vector<int, MyAllocator<int>> myVecWithAlloc(allocat);
    //  vec.reserve(3);
    for (int i = 0; i < allocat.MAX_SIZE; i++) {
        myVecWithAlloc.push_back(i);
    }

    for (const auto &v : myVecWithAlloc) {
        cout << v << "\n";
    }

    return 0;
}

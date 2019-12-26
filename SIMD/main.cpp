#include <iostream>
#include "simd.h"

using namespace std;

int main() {
//    auto t = SIMD("/Users/ladisgin/empty.txt");
    vector<unsigned char> arr;
    for(unsigned char i = 0; i <= 0x3f; ++i){
        arr.push_back(i);
    }
    string s(arr.begin(), arr.end());
    auto t = SIMD(s);
    auto k = t.get_hash();

    for (auto i:k)
        printf("%02x", i);
    printf("\n");
    return 0;
}

#include <iostream>
#include <limits>
#include <vector>
#include <string>

using namespace std;

inline vector<uint8_t> gen_rc4(string key) {
    vector<uint8_t> s(256);
    for(size_t i = 0; i < s.size(); ++i){
        s[i] = static_cast<uint8_t>(i);
    }
    uint8_t j = 0;
    for(size_t i = 0; i < s.size(); ++i){
        j += s[i] + static_cast<uint8_t>(key[i % key.size()]);
        swap(s[i], s[j]);
    }
    return s;
}

inline uint8_t gen_next(uint8_t& i, uint8_t& j, vector<uint8_t>& s) {
    i += 1;
    j += s[i];
    swap(s[i], s[j]);
    return s[static_cast<uint8_t>(s[i] + s[j])];
}

string encode(string m, string& key) {
    auto s = gen_rc4(key);
    uint8_t i = 0, j = 0;
    for(auto& c : m) {
        c ^= gen_next(i, j, s);
    }
    return m;
}

string decode(string& m, string& key){
    return encode(m, key);
}

int main() {
    string m = "rc4 stream cipher rc4 stream cipher rc4 stream cipher rc4 stream cipher rc4 stream cipher";
    string key = "best key in the world best key in the world best key in the world best key in the world";
    auto c = encode(m, key);
    auto ans = decode(c, key);
    cout << ans << endl;
    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <x86intrin.h>

//rc6 32/20/128

typedef uint32_t word;

using namespace std;

const size_t w = std::numeric_limits<word>::digits; // word lenght
const size_t r = 20; // numbers of round
const word shift_n = static_cast<word>(log2(w));

//word odd() { // round to near odd

//}

word rol(word rot, size_t r) { //rotate left
    return ((rot << r) | (rot >> (w - r)));
}

word ror(word rot, size_t r) { //rotate right
    return ((rot << (w - r)) | (rot >> r));
}

vector<word> get_keys(vector<word> L){
//    word Pw_double, Qw_double;
//    Pw_double = odd((((sqrt(5) + 1)/2) - 1) * (static_cast<uint64_t>(1) << w)); //(Golden ratio - 1) * 2^w
//    Qw_double = odd((exp(1) - 2) * (static_cast<uint64_t>(1) << w)); //(exp - 2) * 2^w

    word Qw = 0x9E3779B9;
    word Pw = 0xB7E15163;

    vector<word> S(2*r + 4);
    S[0] = Pw;
    for (size_t i=1; i < 2*r + 4; ++i){
        S[i] = S[i-1] + Qw;
    }


    word A=0, B=0;
    size_t i=0, j=0, c = L.size(), v=3*max(c,2*r+4);
    for(size_t s=0; s < v; ++s) {
        A = S[i] = rol((S[i] + A + B), 3); //cycle shift 3
        B = L[j] = ror((L[j] + A + B), (A + B)); //cycle shift A+B
        i=(i+1) % (2*r + 4);
        j=(j+1) % c;
    }
    return S;
}

void encrypt_block(word& A, word& B, word& C, word& D, vector<word>& S) { // from 4 block of word and write the same memory 4 encrypted block
    B = B + S[0];
    D = D + S[1];
    for (size_t i = 1; i < r + 1; ++i) {
        word t = rol(B * (2 * B + 1), shift_n);
        word u = rol(D * (2 * D + 1), shift_n);
        A = rol((A ^ t), u) + S[2*i];
        C = rol((C ^ u), t) + S[2*i + 1];

        word A_t = A;
        A = B;
        B = C;
        C = D;
        D = A_t;
    }
    A = A + S[2*r + 2];
    C = C + S[2*r + 3];
}

vector<word> encrypt(vector<word> m, vector<word>& S){
    for(size_t i = 0; i < m.size()/4; ++i){
        encrypt_block(m[4*i], m[4*i + 1], m[4*i + 2], m[4*i + 3], S);
    }
    return m;
}

void decrypt_block(word& A, word& B, word& C, word& D, vector<word>& S) { // from 4 encrypted block of word and write the same memory 4 block
    C = C - S[2*r + 3];
    A = A - S[2*r + 2];

    for (size_t i = r; i > 0; --i) {
        word D_t = D;
        D = C;
        C = B;
        B = A;
        A = D_t;
        word u = rol(D * (2 * D + 1), shift_n);
        word t = rol(B * (2 * B + 1), shift_n);
        C = ror((C - S[2 * i + 1]), t) ^ u;
        A = ror((A - S[2 * i]), u) ^ t;
    }
    D = D - S[1];
    B = B - S[0];
}

vector<word> decrypt(vector<word> m, vector<word>& S){
    for(size_t i = 0; i < m.size()/4; ++i){
        decrypt_block(m[4*i], m[4*i + 1], m[4*i + 2], m[4*i + 3], S);
    }
    return m;
}


void get_words(const string& m, vector<word>& m_word) {
    m_word.resize((m.size() + sizeof (word) - 1) / sizeof (word), 0);
    for(size_t i = 0; i < m_word.size(); ++i) {
        for(size_t j = sizeof (word)*i; j < sizeof (word)*i + sizeof (word); ++j) {
            m_word[i] = (m_word[i] << 8);
            m_word[i] |= static_cast<word>(m[j]);
        }
    }
}

string get_string(vector<word>& m_word) {
    string m;
    for(auto i:m_word) {
        for(size_t j = sizeof (word); j > 0; --j){
            m += (static_cast<char>(0xff&(i >> (8 * (j - 1)))));
        }
    }
    return m;
}


int main() {
    string key = "aqfsa,a,njjnjjgg";
    vector<word> w_key;
    get_words(key, w_key);

    string m = "hello world";
    vector<word> m_word;
    get_words(m, m_word);
    auto keys = get_keys(w_key);
    auto c = encrypt(m_word, keys);
    auto m_de = decrypt(c, keys);
    cout << get_string(m_de) << endl;

//    word  a = 2906572611, b = 3499922478, c=2902049276, d=3634129468;
//    encrypt_block(a, b, c, d, w_key);
//    decrypt_block(a, b, c, d, w_key);
    return 0;
}

#ifndef SIMD_H
#define SIMD_H
#include <string>
#include <vector>

typedef unsigned int u32;
typedef unsigned char byte;


class SIMD {
private:
    static const size_t BS = 512;
    std::vector<byte> m;
    size_t left_bit;
    size_t it;

    std::vector<u32> abcd;
    u32 *A, *B, *C, *D;
    std::vector<byte> buffer;
    std::vector<byte> result;

private:
    void run();

    void init();
    void update();
    void final();

    void simd_Compress(const byte *M, int final);
    void message_expansion(u32 W[32][4], const byte *M, int final);
    void round(u32 w[32][4], size_t i, size_t p1, size_t p2, size_t p3, size_t p4);
    void step(const u32 w[4], size_t i, size_t r, size_t s, std::function<u32 (u32,u32,u32)> F);

public:
    SIMD(const std::string &M);
    std::vector<byte> get_hash();
};

#endif // SIMD_H

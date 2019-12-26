#include "simd.h"

#include <cstdio>


inline u32 rol(const u32 &x, const size_t &n) {
    return (x << n) | (x >> (32 - n));
}

u32 if_32(u32 A, u32 B, u32 C) {
    return ((A & B) | (~A & C));
}

u32 vote3_32(u32 A, u32 B, u32 C) {
    return ((A & B) | (A & C) | (B & C));
}

SIMD::SIMD(const std::string &M) {
    m = std::vector<byte>(M.begin(), M.end());
    left_bit = m.size() * 8;
    run();
}

std::vector <byte> SIMD::get_hash() {
    return result;
}

void SIMD::run() {
    init();
    update();
    final();
}

void SIMD::init() {
    it = 0;

    buffer = std::vector<byte>(64, 0);

    abcd = std::vector<u32>(16);
    A = abcd.data();
    B = A + 4;
    C = B + 4;
    D = C + 4;
    memset(A, 0, 64);

    std::string h = "SIMD-256 v1.1";
    std::vector <byte> IV(h.begin(), h.end());
    IV.resize(64);
    simd_Compress(IV.data(), 0);
}

void SIMD::update() {
    while (left_bit > 0) {
        if (left_bit >= BS) {
            simd_Compress(m.data() + (it / 8), 0);
            left_bit -= BS;
            it += BS;
        } else {
            buffer = std::vector<byte>(m.begin() + (it / 8), m.begin() + (it / 8 + left_bit + 7) / 8);
            it += left_bit;
            return;
        }
    }
}

void SIMD::final() {
    size_t current = it & (BS - 1);

    if (current) {
        if (current & 7) {
            auto mask = static_cast<byte>(0xff >> (current & 7));
            buffer[current / 8] &= ~mask;
        }
        current = (current + 7) / 8;
        memset(buffer.data() + current, 0, BS / 8 - current);
        simd_Compress(buffer.data(), 0);
    }

    buffer = std::vector<byte>(BS / 8, 0);

    size_t l = it;
    for (size_t i = 0; i < 8; i++) {
        buffer[i] = static_cast<byte>(l & 0xff);
        l >>= 8;
    }

    simd_Compress(buffer.data(), 1);

    std::vector <byte> bs(64, 0);
    for (size_t i = 0; i < 8; i++) {
        u32 x = A[i];
        bs[4 * i] = static_cast<byte>(x & 0xff);
        x >>= 8;
        bs[4 * i + 1] = static_cast<byte>(x & 0xff);
        x >>= 8;
        bs[4 * i + 2] = static_cast<byte>(x & 0xff);
        x >>= 8;
        bs[4 * i + 3] = static_cast<byte>(x & 0xff);
    }
    result = std::vector<byte>(bs.begin(), bs.begin() + 32);
}

void SIMD::simd_Compress(const byte *M, int final) {
    u32 W[32][4];
    u32 IV[4][4];
    const int n = 4;

    for (int i = 0; i < n; i++) {
        IV[0][i] = A[i];
        IV[1][i] = B[i];
        IV[2][i] = C[i];
        IV[3][i] = D[i];
    }

    message_expansion(W, M, final);

    for (int j = 0; j < n; j++) {
        A[j] ^= static_cast<u32>(M[4 * j] ^ ((M[4 * j + 1]) << 8) ^ ((M[4 * j + 2]) << 16) ^ ((M[4 * j + 3]) << 24));
        B[j] ^= static_cast<u32>(M[4 * j + 4 * n] ^ ((M[4 * j + 4 * n + 1]) << 8) ^ ((M[4 * j + 4 * n + 2]) << 16) ^
                                 ((M[4 * j + 4 * n + 3]) << 24));
        C[j] ^= static_cast<u32>(M[4 * j + 8 * n] ^ ((M[4 * j + 8 * n + 1]) << 8) ^ ((M[4 * j + 8 * n + 2]) << 16) ^
                                 ((M[4 * j + 8 * n + 3]) << 24));
        D[j] ^= static_cast<u32>(M[4 * j + 12 * n] ^ ((M[4 * j + 12 * n + 1]) << 8) ^ ((M[4 * j + 12 * n + 2]) << 16) ^
                                 ((M[4 * j + 12 * n + 3]) << 24));
    }

    round(W, 0, 3, 23, 17, 27);
    round(W, 1, 28, 19, 22, 7);
    round(W, 2, 29, 9, 15, 5);
    round(W, 3, 4, 13, 10, 25);

    step(IV[0], 32, 4, 13, if_32);
    step(IV[1], 33, 13, 10, if_32);
    step(IV[2], 34, 10, 25, if_32);
    step(IV[3], 35, 25, 4, if_32);
}

void SIMD::message_expansion(u32 W[32][4], const byte *const M, int final) {
    const int P[32] = {4, 6, 0, 2, 7, 5, 3, 1,
                       15, 11, 12, 8, 9, 13, 10, 14,
                       17, 18, 23, 20, 22, 21, 16, 19,
                       30, 24, 25, 31, 27, 29, 28, 26};

    short y[256];

    const int alpha = 139;
    int beta = 98;
    int beta_i = 1;
    int alpha_i = 1;
    u32 Z[32][4];

    const int y_size = 128;

    for (int i = 0; i < y_size; i++) {
        y[i] = static_cast<short>(beta_i);
        beta_i = (beta_i * beta) % 257;
    }


    if (final) {
        beta = 58;
        beta_i = 1;
        for (int i = 0; i < y_size; i++) {
            y[i] += beta_i;
            beta_i = (beta_i * beta) % 257;
        }
    }


    for (int i = 0; i < y_size; i++) {
        int last = 1;

        for (int j = 0; j < 64; j++) {
            y[i] = static_cast<short>((y[i] + last * M[j]) % 257);
            last = (last * alpha_i) % 257;
        }

        alpha_i = (alpha_i * alpha) % 257;
    }


    for (int i = 0; i < y_size; i++) {
        if (y[i] > 128) {
            y[i] -= 257;
        }
    }

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            Z[i][j] = ((static_cast<u32>(y[2 * i * 4 + 2 * j] * 185)) & 0xffff) |
                      (static_cast<u32>(y[2 * i * 4 + 2 * j + 1] * 185) << 16);
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            Z[i + 16][j] = ((static_cast<u32>(y[2 * i * 4 + 2 * j] * 233)) & 0xffff) |
                           (static_cast<u32>(y[2 * i * 4 + 2 * j + y_size / 2] * 233) << 16);
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            Z[i + 24][j] = ((static_cast<u32>(y[2 * i * 4 + 2 * j + 1] * 233)) & 0xffff) |
                           (static_cast<u32>(y[2 * i * 4 + 2 * j + y_size / 2 + 1] * 233) << 16);
        }
    }


    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 4; j++) {
            W[i][j] = Z[P[i]][j];
        }
    }
}

void SIMD::round(u32 w[32][4], const size_t i, const size_t p1, const size_t p2, const size_t p3, const size_t p4) {
    step(w[8 * i], 8 * i, p1, p2, if_32);
    step(w[8 * i + 1], 8 * i + 1, p2, p3, if_32);
    step(w[8 * i + 2], 8 * i + 2, p3, p4, if_32);
    step(w[8 * i + 3], 8 * i + 3, p4, p1, if_32);

    step(w[8 * i + 4], 8 * i + 4, p1, p2, vote3_32);
    step(w[8 * i + 5], 8 * i + 5, p2, p3, vote3_32);
    step(w[8 * i + 6], 8 * i + 6, p3, p4, vote3_32);
    step(w[8 * i + 7], 8 * i + 7, p4, p1, vote3_32);
}

void SIMD::step(const u32 *w, const size_t i, const size_t r, const size_t s, std::function<u32(u32, u32, u32)> F) {
    int p[][4] = {{1, 0, 3, 2},
                  {2, 3, 0, 1},
                  {3, 2, 1, 0}};

    u32 tmp[4];
    for (int j = 0; j < 4; j++)
        tmp[j] = rol(A[j], r);

    for (int j = 0; j < 4; j++) {
        A[j] = D[j] + w[j] + F(A[j], B[j], C[j]);
        A[j] = rol(A[j], s) + tmp[p[i % 3][j]];
        D[j] = C[j];
        C[j] = B[j];
        B[j] = tmp[j];
    }
}

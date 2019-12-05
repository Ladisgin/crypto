package com.company;

public class A5 {
    private int r1, r2, r3;
    private int iter;
    private int frame;
    byte[] in;

    A5(byte[] in) {
        this.in = in;
        frame = (in[8] << 14) | (in[9] << 6) | (in[10] >> 2);
        initialize();
    }

    private boolean shift1() {
        var a = ((r1 >> 18) & 1) ^ ((r1 >> 17) & 1) ^ ((r1 >> 16) & 1) ^ ((r1 >> 13) & 1);
        r1 = (r1 << 1) ^ a;
        r1 &= (1 << 19) - 1;
        return a == 1;
    }

    private boolean shift2() {
        var a = ((r2 >> 21) & 1) ^ ((r2 >> 20) & 1);
        r2 = (r2 << 1) ^ a;
        r2 &= (1 << 22) - 1;
        return a == 1;
    }

    private boolean shift3() {
        var a = ((r1 >> 22) & 1) ^ ((r1 >> 21) & 1) ^ ((r1 >> 20) & 1) ^ ((r1 >> 7) & 1);
        r1 = (r1 << 1) ^ a;
        r1 &= (1 << 23) - 1;
        return a == 1;
    }

    private void shiftAll() {
        shift1();
        shift2();
        shift3();
    }

    private void initialize() {
        iter = 0;
        r1 = 0;
        r2 = 0;
        r3 = 0;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                shiftAll();
                r1 ^= (in[i] >> j) & 1;
                r2 ^= (in[i] >> j) & 1;
                r3 ^= (in[i] >> j) & 1;
            }
        }

        for (int i = 0; i < 22; ++i) {
            shiftAll();
            r1 ^= (frame >> i);
            r2 ^= (frame >> i);
            r3 ^= (frame >> i);
        }
        for (int i = 0; i < 100; ++i) {
            get_next();
        }
    }

    boolean get_next() {
        boolean x1 = ((r1 >> 8) & 1) == 1;
        boolean x2 = ((r2 >> 10) & 1) == 1;
        boolean x3 = ((r3 >> 10) & 1) == 1;
        boolean f = x1 & x2 | x1 & x3 | x2 & x3;
        if (x1 == f) x1 = shift1();
        if (x2 == f) x2 = shift2();
        if (x3 == f) x3 = shift3();
        boolean ans = x1 ^ x2 ^ x3;
        ++iter;
        if (iter % 228 == 0) {
            frame = iter;
            initialize();
        }
        return ans;
    }
}

package com.company;

import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        byte[] rand = new String("HGFEDCBAHGFEDCBA").getBytes();
        byte[] key = new String("ABCDEFGHABCDEFGH").getBytes();
        byte[] out = A3A8.genKey(rand, key);
        A5 key_generator = new A5(Arrays.copyOfRange(out, 5, 16));
        for (int i = 0; i < 512; ++i) {
            System.out.print(key_generator.get_next() ? 1 : 0);
        }
        System.out.println();
    }
}

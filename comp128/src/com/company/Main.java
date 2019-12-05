package com.company;

public class Main {
    public static void main(String[] args) {
        byte[] rand = new String("HGFEDCBAHGFEDCBA").getBytes();
        byte[] key = new String("ABCDEFGHABCDEFGH").getBytes();
        byte[] out = A3A8.A3A8(rand, key);
        A5 key_generator = new A5(out);
        for (int i = 0; i < 512; ++i) {
            System.out.print(key_generator.get_next() ? 1 : 0);
        }
        System.out.println();
    }
}

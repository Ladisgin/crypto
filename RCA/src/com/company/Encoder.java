package com.company;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class Encoder {
    private BigInteger p, q, n, fi, e, d;
    byte[] encode(byte[] m) {
        byte[] dest = new byte[n.bitLength()*(m.length / (n.bitLength() - 1))];
        for(int i = 0; i < m.length; i += (n.bitLength() - 1)) {
            var t = Arrays.copyOfRange(m, i, Integer.max(i + n.bitLength() - 1, m.length));
        }
    }
}

package com.company;

import java.math.BigInteger;


public class EncrypterRSA {
    private BigInteger publicKey;
    private BigInteger modulus;

    EncrypterRSA(BigInteger publicKey, BigInteger modulus) {
        this.publicKey = publicKey;
        this.modulus = modulus;
    }

    byte[] encrypt(BigInteger message) {
        BigInteger encoded = BigInteger.ZERO;
        while(!message.equals(BigInteger.ZERO)) {
            var t = message.divideAndRemainder(modulus);
            encoded = encoded.multiply(modulus).add(t[1].modPow(publicKey, modulus));
            message = t[0];
        }
        return encoded.toByteArray();
    }

    byte[] encrypt(byte[] byte_message) {
        BigInteger message = new BigInteger(byte_message);
        return encrypt(message);
    }

    byte[] encrypt(String string_message) {
        return encrypt(string_message.getBytes());
    }
}

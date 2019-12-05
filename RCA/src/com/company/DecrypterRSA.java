package com.company;

import java.math.BigInteger;
import java.security.SecureRandom;

public class DecrypterRSA {

    private final static SecureRandom random = new SecureRandom();

    private BigInteger privateKey;
    private BigInteger publicKey;
    private BigInteger modulus;
    private int step;

    DecrypterRSA(int N) {
        BigInteger p = BigInteger.probablePrime(N/2, random);
        BigInteger q = BigInteger.probablePrime(N/2, random);
        BigInteger phi = (p.subtract(BigInteger.ONE)).multiply(q.subtract(BigInteger.ONE));

        this.modulus = p.multiply(q);
        this.publicKey = BigInteger.probablePrime(16, random);
        this.privateKey = publicKey.modInverse(phi);
        this.step = modulus.bitLength() - 1;
    }

    DecrypterRSA(BigInteger p, BigInteger q) {
        BigInteger phi = (p.subtract(BigInteger.ONE)).multiply(q.subtract(BigInteger.ONE));
        modulus = p.multiply(q);
        publicKey = BigInteger.probablePrime(16, random);
        privateKey = publicKey.modInverse(phi);
    }

    byte[] decrypt(BigInteger encrypted) {
        BigInteger decrypted = BigInteger.ZERO;
        while(!encrypted.equals(BigInteger.ONE) && !encrypted.equals(BigInteger.ZERO)) {
            var t = encrypted.and(BigInteger.ONE.shiftLeft(step + 1).subtract(BigInteger.ONE));
            decrypted = decrypted.shiftLeft(step).or(t.modPow(privateKey, modulus));
            encrypted = encrypted.shiftRight(step + 1);
        }
        return decrypted.toByteArray();

//        BigInteger decrypted = BigInteger.ZERO;
//        while(!encrypted.equals(BigInteger.ZERO)) {
//            var t = encrypted.divideAndRemainder(modulus);
//            decrypted = decrypted.multiply(modulus).add(t[1].modPow(privateKey, modulus));
//            encrypted = t[0];
//        }
//        return decrypted.toByteArray();
    }

    byte[] decrypt(byte[] byte_encrypted) {
        BigInteger encrypted = new BigInteger(byte_encrypted);
        return decrypt(encrypted);
    }

    BigInteger getPublicKey() {
        return publicKey;
    }

    BigInteger getModulus() {
        return modulus;
    }
}

package com.company;

import java.math.BigInteger;
import java.time.Duration;
import java.time.Instant;
import java.util.Random;

public class Main {

    public static void main(String[] args) {
        int N = 512;
        DecrypterRSA decrypterRSA = new DecrypterRSA(N);
        EncrypterRSA encrypterRSA = new EncrypterRSA(decrypterRSA.getPublicKey(), decrypterRSA.getModulus());

        BigInteger m = BigInteger.probablePrime(4, new Random());
        System.out.println("message = " + m);
        BigInteger e = new BigInteger(encrypterRSA.encrypt(m));
        System.out.println("encrypted = " + e);
        BigInteger d = new BigInteger(decrypterRSA.decrypt(e));
        System.out.println("decrypted = " + d);
        System.out.println("is equal = " + m.equals(d));
        System.out.println();


        String string_m = "Currants, Flame Seedless raisins, Golden Seedless raisins, Monukka raisins, Muscat raisins, Natural seedless raisins";
        System.out.println("lenght = " + string_m.getBytes().length * 8);
        System.out.println("message = " + string_m);
        e = new BigInteger(encrypterRSA.encrypt(string_m));
        System.out.println("encrypted = " + e);
        String string_d = new String(decrypterRSA.decrypt(e));
        System.out.println("decrypted = " + string_d);
        System.out.println("is equal = " + string_m.equals(string_d));
        System.out.println();


        m = new BigInteger(5273231, new Random());
        System.out.println("lenght = " + m.bitLength());
        Instant start = Instant.now();
        e = new BigInteger(encrypterRSA.encrypt(m));
        Instant end = Instant.now();
        Duration timeElapsed = Duration.between(start, end);
        System.out.println("encoding: " + m.bitLength()/timeElapsed.toMillis() + " bits per milliseconds");
        start = Instant.now();
        d = new BigInteger(decrypterRSA.decrypt(e));
        end = Instant.now();
        timeElapsed = Duration.between(start, end);
        System.out.println("decoding: " + m.bitLength()/timeElapsed.toMillis()+ " bits per milliseconds");
        System.out.println("is equal = " + m.equals(d));
        System.out.println();

    }
}

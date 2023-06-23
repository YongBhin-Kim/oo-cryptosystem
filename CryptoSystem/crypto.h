#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>
#include <stdint.h>
#include <string>
using namespace std;

class Crypto {
protected:
    uint8_t *key;
    uint8_t *pt;
    uint8_t *ct;
    uint8_t *recovered;
    int keyLen;
    int ptLen;
    int ctLen;
public:

    Crypto(uint8_t *key, uint8_t *pt, int keyLen, int ptLen);
    virtual void encrypt() = 0;
    virtual void decrypt() = 0;
    virtual void keySchedule() = 0;

    virtual void printKey() = 0;
    virtual void printCiphertext() = 0;
    virtual void printPlaintext() = 0;
    virtual void printDecryptedtext() = 0;
    void getRecovered(uint8_t *recovered);
};

// CryptographicService {
//     encrypt() {
//         Crypto crypto = getCryproModule();
//         crypto
//     }
//     virtual Crypto getCryptoModule() = 0;
// }
// Aes {
//     Crypto getCryptoModule() {
//         return new AES();
//     }
// }

#endif
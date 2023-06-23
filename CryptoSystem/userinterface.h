#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <string>
#include "cryptographicmanager.h"

using namespace std;
class UserInterface {
    string query;
    string plaintextString;
    string cryptoModuleString;
    const int keyLen = 16;
    const int ptLen = 16;
    char recoveredString[17];
    uint8_t key[16]; // KeyManager으로부터 키 발급
    uint8_t recovered[16];
    uint8_t pt[16];
    int keycnt;

    int validate();
    void genKey();
    void printPlaintext(CryptographicManager *CryptographicManager);
    void printCiphertext(CryptographicManager *CryptographicManager);
    void keySchedule(CryptographicManager *CryptographicManager);
    void encrypt(CryptographicManager *CryptographicManager);
    void decrypt(CryptographicManager *CryptographicManager);
    void printDecryptedtext(CryptographicManager *CryptographicManager);
    void getRecovered(CryptographicManager *CryptographicManager);
public:
    const static int endCryptosystem = -1;
    int startCryptoSystem();
};

#endif

// User -> UaserInterface 
// [UserInterface]
// -key: uint8_t*
// -pt: uint8_t*
// -recovered: uint8_t*
// -keyLen: int
// -ptLen: int
// -keycnt: int

// -query: string
// -plaintextString: string
// -recoveredString: string
// -cryptoModuleString: string

// [UserInterface] --manageQuary()--> [UserInterface]
// [UserInterface] --validate()--> [UserInterface]
// [UserInterface] ---genKey()--> [CryptographicManager]  --genKey()--> [KeyManager]
// [UserInterface] -----> [CryptographicManager]

// [CryptographicManager]
// -moduleName: string
// -launched: bool
// -crypto: Crypto*
// -keyManager: KeyManager*

// [Crypto]


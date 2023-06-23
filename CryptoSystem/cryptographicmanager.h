#ifndef CRYPTOGRAPHICMANAGER_H
#define CRYPTOGRAPHICMANAGER_H

#include <iostream>
#include <stdint.h>
#include <string>
#include "crypto.h"
#include "aes.h"
#include "aria.h"
#include "keymanager.h"

using namespace std;

class CryptographicManager {
    string moduleName;
    bool launched;
public:
    Crypto *crypto; // Factory pattern에 의해 어떤 crypto인지 결정됨
    // KeyManager *keyManager
    bool getLaunched() {return launched;}
    CryptographicManager(uint8_t *key=nullptr, uint8_t *pt=nullptr, int keyLen=0, int ptLen=0, string moduleName="GENKEY");
};

#endif
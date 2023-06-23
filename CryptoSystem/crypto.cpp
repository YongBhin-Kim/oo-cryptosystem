#include "crypto.h"
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <cstdlib>

Crypto::Crypto(uint8_t *key, uint8_t *pt, int keyLen, int ptLen): keyLen(keyLen), ptLen(ptLen), ctLen(ptLen) {

    this->key = new uint8_t[keyLen];
    this->pt  = new uint8_t[ptLen];
    this->ct = new uint8_t[ctLen];
    this->recovered = new uint8_t[ptLen];

    for (int i=0; i<keyLen; i++) this->key[i] = key[i];
    for (int i=0; i<ptLen; i++)  this->pt[i] = pt[i];
};
void Crypto::getRecovered(uint8_t *recovered) {
    for (int i=0; i<16; i++)
        recovered[i] = this->recovered[i];
}

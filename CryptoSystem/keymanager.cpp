#include "keymanager.h"
using namespace std;

SingletonKeyManager &SingletonKeyManager::getInstance() {
    static SingletonKeyManager s;
    return s;
}
void SingletonKeyManager::genKey(uint8_t *key) {
    srand(time(NULL));
    for (int i=0; i<16; i++) {
        key[i] = (uint8_t) (rand() % 256);
    }
    printKey(key, 16);
}
void SingletonKeyManager::printKey(uint8_t *key, int keyLen) {
    cout << " -> Generated 16-byte random key is: " << endl;
    for (int i=0; i<keyLen; i++) {
        printf("%02x ", key[i]);
        if ((i%16)==15) cout << endl;
    }
    cout << endl;
}
#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

// Singleton class - Key manager
class SingletonKeyManager {
private:
    // No allowed to inheritance
    SingletonKeyManager() { } // constructor - private
    SingletonKeyManager(const SingletonKeyManager &ref) { } // copy consturctor - private
    SingletonKeyManager &operator=(const SingletonKeyManager &ref) { } // allocation operator - private
    ~SingletonKeyManager() { } // destructor - private
public:
    static SingletonKeyManager &getInstance(); // get singleton instance
    
    void genKey(uint8_t *key); 
    void printKey(uint8_t *key, int keyLen);
};

#endif
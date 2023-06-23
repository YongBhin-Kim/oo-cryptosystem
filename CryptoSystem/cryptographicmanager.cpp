#include "cryptographicmanager.h"
using namespace std;

// Design pattern - Factory 
CryptographicManager::CryptographicManager(uint8_t *key, uint8_t *pt, int keyLen, int ptLen, string moduleName) {
    this->moduleName = moduleName;
    // crypto 속성이 사용자의 입력에 따라 결정됨
    if (this->moduleName == "AES") {
        cout << endl << " [ CryptographicManager - crypto Launched ]\n" << endl;
        cout << " -> AES Encrypt module launched. " << endl;
        crypto = new Aes(key, pt, keyLen, ptLen);
        launched = true;
    }
    else if (this->moduleName == "ARIA") {
        cout << " [ CryptographicManager - crypto Launched ]\n" << endl;
        cout << " -> ARIA Encrypt module launched. " << endl;
        crypto = new Aria(key, pt, keyLen, ptLen);
        launched = true;
    }
    else if (this->moduleName == "GENKEY") {
        cout << " [ CryptographicManager - keyManager Launched ]\n" << endl;
        cout << " -> Get random 16-byte key.";
    }
    else {
        cout << " Fatal error : " << moduleName << " is not exists module name. please select correct module!" << endl;
        launched=false;
    }
}
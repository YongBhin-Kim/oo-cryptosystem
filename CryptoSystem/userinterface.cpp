#include "userinterface.h"

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <stdint.h>
#include <string>

using namespace std;
int UserInterface::validate() {
    int fail = -1; 
    if (this->keycnt>=3)
        return -1;
    return (-1)*fail;
}
int UserInterface::startCryptoSystem() {
    cout << " ** ****************************************************************************** ** " << endl;
    cout << " **              CryptoSystem에 오신 것을 환영합니다.           " << endl;
    cout << " ** ****************************************************************************** ** " << endl;
    while(true) {
        cout << " - 선택 메뉴 : 키 발급(1), 종료(q) > ";
        cin >> query;
        cout << "    ------------------------------------------------------------------------------    " << endl;
        if (query=="1") break;
        else if (query=="q") return endCryptosystem;
        else {
            cout << " -> 알 수 없는 명령어입니다. 다시 입력해주세요. > " << endl;
            goto getQuery;
        }
    }
    while(true) { 
        if (this->query=="1") { 
            genKey();

            // HW4의 implementation
            // CryptographicManager *keyManager = new CryptographicManager();
            // keyManager->keyManager->genKey(key);

        }
        else if (this->query=="2") { 
            cout << " -> 발급받은 키가 " << keycnt << "번 사용되었습니다. 3번 이상 사용할 경우 키를 재발급 받으시기 바랍니다." << endl;
            if (validate()==-1) {
                goto getQuery;
            }
        }
        else if (this->query=="3") { 
            if (keycnt>=3) {
                cout << " -> 발급받은 키가 3번 이상 사용되었습니다. 키를 재발급 받으시기 바랍니다." << endl;
                goto getQuery;
            }
            keycnt++;
            cout << " - 평문을 입력하시오 > ";
            cin.ignore();
            getline(std::cin, plaintextString);
            cout << endl;
            memset(&pt, 0, sizeof(pt));
            memcpy(pt, plaintextString.data(), plaintextString.length());
            cout << " - 암호 모듈을 선택하시오 : ARIA, AES > ";
            getline(std::cin, cryptoModuleString);
            cout << endl;
            
            // 평문과 발급받은 key를 이용하여 CryptoGraphicManager class의 object 생성
            CryptographicManager *cryptographicManager = new CryptographicManager(key, pt, keyLen, ptLen, cryptoModuleString);

            if ( cryptographicManager->getLaunched()==false ) {
                goto getQuery;
            }

            printPlaintext(cryptographicManager); // cryptoGraphicManager->crypto->printPlaintext();

            keySchedule(cryptographicManager); // cryptoGraphicManager->crypto->keySchedule();
            // cryptoGraphicManager->crypto->printKey();
            encrypt(cryptographicManager); // cryptoGraphicManager->crypto->encrypt();
            printCiphertext(cryptographicManager); // cryptoGraphicManager->crypto->printCiphertext();

            decrypt(cryptographicManager); // cryptoGraphicManager->crypto->decrypt();
            printDecryptedtext(cryptographicManager); // cryptoGraphicManager->crypto->printDecryptedtext();

            getRecovered(cryptographicManager); // cryptoGraphicManager->crypto->getRecovered(recovered);
            int i=0;
            while (recovered[i]!=0) {
                recoveredString[i]=recovered[i];
                i++;
            }
            recoveredString[i]='\0';
            cout << " --------------------------------------------- " << endl;
            cout << " * 평문 : " << plaintextString << endl;
            cout << " * 복호문 : ";
            cout << recoveredString << endl;
            cout << " --------------------------------------------- " << endl;
            memset(&recovered, 0, sizeof(recovered));
        }
        else if (this->query=="q") break;
        else cout << " -> 알 수 없는 명령어입니다. 다시 입력해주세요." << endl;

getQuery:
        cout << " ** ============================================================================== ** " << endl;
        cout << " ** ============================================================================== ** " << endl;
        cout << endl << " - 선택 메뉴 : 키 재발급(1), 키 검증(2), 암호화&복호화(3), 종료(q) > ";
        cin >> query;
        cout << "    ------------------------------------------------------------------------------    " << endl;
    }
}
// HW5 추가
void UserInterface::genKey() {
    memset(&key, 0, sizeof(key));
    keycnt = 0; // initialize a count of key

    // Desing patterns - Singleton 
    SingletonKeyManager &keyManager = SingletonKeyManager::getInstance();
    keyManager.genKey(key);
}
void UserInterface::printPlaintext(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->printPlaintext();
}
void UserInterface::printCiphertext(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->printCiphertext();
}
void UserInterface::keySchedule(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->keySchedule();
}
void UserInterface::encrypt(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->encrypt();
}
void UserInterface::decrypt(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->decrypt();
}
void UserInterface::printDecryptedtext(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->printDecryptedtext();
}
void UserInterface::getRecovered(CryptographicManager *cryptographicManager) {
    cryptographicManager->crypto->getRecovered(recovered);
}
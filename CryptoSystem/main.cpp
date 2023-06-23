
// OOAD HW4 20182211 Yongbhin Kim
// - CryptoSystem

#include <iostream>
#include "crypto.h"
#include "userinterface.h"
#include <cstddef>
#include <cstring>
#include <stdint.h>
#include <string>

using namespace std;

int main() { 
    

    UserInterface userInterface;
    if (userInterface.startCryptoSystem()==UserInterface::endCryptosystem) {
        cout << " 프로그램을 종료합니다. " << endl;
    }


    return 0;
}

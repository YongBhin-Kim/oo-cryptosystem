#include "aes.h"
using namespace std;

void Aes::encrypt() {
    uint32_t state[4], temp[4];
    // u8[16] pt -> u8[4] state
    for (int i=0; i<4; i++)
        state[i] = getu(Crypto::pt + 4*i);
    
    // AddRoundKey
    for (int i=0; i<4; i++) 
        state[i] ^= roundKey[0][i];

    // 1~9 round
    for (int r=1; r<=9; r++) {
        encryptRound(state, r);
    }

    // 10 round
    temp[0] = ( te4[(uint8_t)(state[0]>>24)] & 0xff000000) ^ ( te4[(uint8_t)(state[1]>>16)] & 0x00ff0000) ^ ( te4[(uint8_t)(state[2]>>8)] & 0x0000ff00) ^ ( te4[(uint8_t)(state[3])] & 0x000000ff) ^ roundKey[10][0];
    temp[1] = ( te4[(uint8_t)(state[1]>>24)] & 0xff000000) ^ ( te4[(uint8_t)(state[2]>>16)] & 0x00ff0000) ^ ( te4[(uint8_t)(state[3]>>8)] & 0x0000ff00) ^ ( te4[(uint8_t)(state[0])] & 0x000000ff) ^ roundKey[10][1];
    temp[2] = ( te4[(uint8_t)(state[2]>>24)] & 0xff000000) ^ ( te4[(uint8_t)(state[3]>>16)] & 0x00ff0000) ^ ( te4[(uint8_t)(state[0]>>8)] & 0x0000ff00) ^ ( te4[(uint8_t)(state[1])] & 0x000000ff) ^ roundKey[10][2];
    temp[3] = ( te4[(uint8_t)(state[3]>>24)] & 0xff000000) ^ ( te4[(uint8_t)(state[0]>>16)] & 0x00ff0000) ^ ( te4[(uint8_t)(state[1]>>8)] & 0x0000ff00) ^ ( te4[(uint8_t)(state[2])] & 0x000000ff) ^ roundKey[10][3];

    for (int i=0; i<4; i++) 
        state[i] = temp[i];


    for (int i=0; i<4; i++) 
        putu(ct + 4*i, state[i]);

}
void Aes::encryptRound(uint32_t state[4], int round) {
    uint32_t temp[4];
    temp[0] = te0[ (uint8_t)(state[0]>>24)] ^ te1[ (uint8_t)(state[1]>>16)] ^ te2[ (uint8_t)(state[2]>>8)] ^ te3[ (uint8_t)(state[3]) ] ^ roundKey[round][0];
    temp[1] = te0[ (uint8_t)(state[1]>>24)] ^ te1[ (uint8_t)(state[2]>>16)] ^ te2[ (uint8_t)(state[3]>>8)] ^ te3[ (uint8_t)(state[0]) ] ^ roundKey[round][1];
    temp[2] = te0[ (uint8_t)(state[2]>>24)] ^ te1[ (uint8_t)(state[3]>>16)] ^ te2[ (uint8_t)(state[0]>>8)] ^ te3[ (uint8_t)(state[1]) ] ^ roundKey[round][2];
    temp[3] = te0[ (uint8_t)(state[3]>>24)] ^ te1[ (uint8_t)(state[0]>>16)] ^ te2[ (uint8_t)(state[1]>>8)] ^ te3[ (uint8_t)(state[2]) ] ^ roundKey[round][3];

    for (int i=0; i<4; i++) state[i] = temp[i];

}
void Aes::addRoundKey8(uint8_t state[16], uint8_t rRoundKeyPrime[16]) {
    for (int i=0; i<16; i++)
        state[i] ^= rRoundKeyPrime[i];
}
void Aes::invSubBytes(uint8_t state[16]) {
    for (int i=0; i<16; i++)
        state[i] = isBox[state[i]];
}
void Aes::invShiftRows(uint8_t state[16]) {
    uint8_t temp;
    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;

    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    temp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = temp;
}
void Aes::invMixColumns(uint8_t state[16]) {
    uint8_t invMixColMatrix[4][4] = { {0x0e, 0x0b, 0x0d, 0x09}, {0x09, 0x0e, 0x0b, 0x0d}, {0x0d, 0x09, 0x0e, 0x0b} , {0x0b, 0x0d, 0x09, 0x0e} };
    GaloisField mc(4, 4);
    for (int i=0; i<mc.row; i++)
        for (int j=0; j<mc.col; j++)
            mc.setValue(invMixColMatrix[i][j], i, j); //mc.mat[i][j] = invMixColMatrix[i][j];

    GaloisField x(4, 1);
    GaloisField y(4, 1);

    // y = mc * x
    for (int col=0; col<4; col++) {
        for (int i=0; i<4; i++)
            x.setValue(state[col*4 + i], i, 0);//x.mat[i][0] = state[col*4 + i]; // 
        y = mc * x; // GaloisField 위에서의 연산자를 *, =를 정의

        for (int i=0; i<4; i++) 
            state[col*4 + i] = y.getValue(i, 0); //state[col*4 + i] = y.mat[i][0]; 
    }
}

void Aes::decrypt() {
    uint8_t state[16];
    for (int i=0; i<16; i++)
        state[i] = ct[i];

    // Round 10
    for (int i=0; i<16; i++)
        state[i] ^= roundKey8[10][i];
    
    // Round 9 to 1
    uint8_t rRoundKeyPrime[16];
    for (int r=9; r>=1; r--) {
        invSubBytes(state);
        invShiftRows(state);
        invMixColumns(state);
        for (int i=0; i<16; i++) 
            rRoundKeyPrime[i] = roundKey8[r][i];
        invMixColumns(rRoundKeyPrime);
        addRoundKey8(state, rRoundKeyPrime);
    }

    invSubBytes(state);
    invShiftRows(state);
    addRoundKey8(state, roundKey8[0]);

    for (int i=0; i<16; i++)
        recovered[i] = state[i];
}
void Aes::keySchedule() {
    uint32_t temp;
    int rc_index;

    for (int i=0; i<4; i++)
        roundKey[0][i] = getu(Crypto::key + 4*i);

    rc_index = 0;
    for (int i=1; i<11; i++) {
        temp = roundKey[i-1][3];
        for (int k=0; k<4; k++) {
            if ( k==0 ) {
                temp = Aes::subWord(Aes::rotWord(temp)) ^ Aes::rc[rc_index++];
            }
            roundKey[i][k] = roundKey[i-1][k] ^ temp;
            temp = roundKey[i][k];
        }
    }
    for (int i=0; i<11; i++)
        for (int j=0; j<4; j++)
            putu(roundKey8[i]+ 4*j, roundKey[i][j]);
}
uint32_t Aes::rotWord(uint32_t w) {
    return (w<<8) ^ (w>>24);
}
uint32_t Aes::subWord(uint32_t w) {
    return (sBox[w >> 24] << 24) ^ (sBox[(w >> 16) & 0xff] << 16) ^ (sBox[(w >> 8) & 0xff] << 8) ^ sBox[w & 0xff];
}
uint32_t Aes::getu(uint8_t *state) {
    return ( ((uint32_t)(state)[0] << 24) ^ ((uint32_t)(state)[1] << 16) ^ ((uint32_t)(state)[2] << 8) ^ ((uint32_t)(state)[3]) );
}
void Aes::putu(uint8_t *ct, uint32_t state) {
    (ct)[0] = (uint8_t)(state>>24); 
    (ct)[1] = (uint8_t)(state>>16); 
    (ct)[2] = (uint8_t)(state>>8); 
    (ct)[3] = (uint8_t)(state);
}
void Aes::printKey() {
    cout << " Master key = " << endl;
    for (int i=0; i<keyLen; i++) {
        printf("%02x ", key[i]);
        if ((i%16)==15) cout << endl;
    }

    cout << endl << " Round key = " << endl;
    for (int i=0; i<11; i++) {
        for (int j=0; j<4; j++)
            printf("0x%08x ", roundKey[i][j]);
        printf("\n");
    }
    for (int i=0; i<11; i++) {
        for (int j=0; j<16; j++)
            printf("0x%02x ", roundKey8[i][j]);
        printf("\n");
    }
}
void Aes::printCiphertext() {
    cout << " Ciphertext = " << endl;
    for (int i=0; i<Crypto::ctLen; i++) {
        printf("%02x ", Crypto::ct[i]);
        if ((i%16)==15) cout << endl;
    }
}
void Aes::printPlaintext() {
    cout << " Plaintext = " << endl;
    for (int i=0; i<Crypto::ptLen; i++) {
        printf("%02x ", Crypto::pt[i]);
        if ((i%16)==15) cout << endl;
    }
}
void Aes::printDecryptedtext() {
    cout << " Recovered = " << endl;
    for (int i=0; i<Crypto::ptLen; i++) {
        printf("%02x ", Crypto::recovered[i]);
        if ((i%16)==15) cout << endl;
    }

}

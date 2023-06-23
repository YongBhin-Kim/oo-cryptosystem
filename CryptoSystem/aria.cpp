#include "aria.h"
using namespace std;

void Aria::keySchedule() {
    // Crypto::key
    // Feistal 암호의 입력인 KL, KR은 mk||0..0 으로 적절히 만들것
    uint8_t W0[16], W1[16], W2[16], W3[16];             // output of feistal crypto 
    uint8_t KL[16], KR[16], CK1[16], CK2[16], CK3[16];  // input of feistal crypto 
    memset(&W0, 0, sizeof(W0)); memset(&W1, 0, sizeof(W1)); memset(&W2, 0, sizeof(W2)); memset(&W3, 0, sizeof(W3));
    memset(&KL, 0, sizeof(KL)); memset(&KR, 0, sizeof(KR)); memset(&CK1, 0, sizeof(CK1)); memset(&CK2, 0, sizeof(CK2)); memset(&CK3, 0, sizeof(CK3));

    // KL <- mk[0~15] for all case of ARIA-128/192/256
    for (int i=0; i<16; i++) 
        KL[i] = key[i]; // Allocations KL which is msb-16-bytes of master key
    
    // KR <- mk||0..0 case ARIA-128/192/256
    for (int i=0; i<16; i++) { // select CK1, CK2, CK3
        CK1[i] = C1[i];
        CK2[i] = C2[i];
        CK3[i] = C3[i];
    }

    // W0 <- KL
    for (int i=0; i<16; i++)
        W0[i] = KL[i];
    
    // W1 <- fo(W0, CK1) ^ KR
    uint8_t temp0[16];
    memset(&temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W0[i];
    fo(temp0, CK1);
    for (int i=0; i<16; i++)
        W1[i] = temp0[i] ^ KR[i];
    // W2 <- fe(W1, CK2) ^ W0
    memset(&temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W1[i];
    fe(temp0, CK2);
    for (int i=0; i<16; i++)
        W2[i] = temp0[i] ^ W0[i];
    // W3 <- fo(W2, CK3) ^ W1
    memset(temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W2[i];
    fo(temp0, CK3);
    for (int i=0; i<16; i++)
        W3[i] = temp0[i] ^ W1[i];

    /* Round key generation part (ARIA-128/192/256 : 12/14/16 round, 16 bytes) */
    uint8_t rotW0[16], rotW1[16], rotW2[16], rotW3[16];
    memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));

    // Generate encrypt round key
    rot(W0, 19, rotW0); rot(W1, 19, rotW1); rot(W2, 19, rotW2); rot(W3, 19, rotW3); // rotation >>>19 W0,W1,W2,W3
    for (int i=0; i<16; i++) {
        enc_w[0][i] = W0[i] ^ rotW1[i];      // ek1 <- W0 ^ (W1 >>> 19)
        enc_w[1][i] = W1[i] ^ rotW2[i];      // ek2 <- W1 ^ (W2 >>> 19)
        enc_w[2][i] = W2[i] ^ rotW3[i];      // ek3 <- W2 ^ (W3 >>> 19)
        enc_w[3][i] = W3[i] ^ rotW0[i];      // ek4 <- (W0 >>> 19) ^ W3
    }

    memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
    rot(W0, 31, rotW0); rot(W1, 31, rotW1); rot(W2, 31, rotW2); rot(W3, 31, rotW3); // rotation >>>31 W0,W1,W2,W3
    for (int i=0; i<16; i++) {
        enc_w[4][i] = W0[i] ^ rotW1[i];      // ek5 <- W0 ^ (W1>>>31) 
        enc_w[5][i] = W1[i] ^ rotW2[i];      // ek6 <- W1 ^ (W2>>>31)
        enc_w[6][i] = W2[i] ^ rotW3[i];      // ek7 <- W2 ^ (W3>>>31)
        enc_w[7][i] = W3[i] ^ rotW0[i];      // ek8 <- (W0>>>31) ^ W3
    }

    memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
    rot(W0, 67, rotW0); rot(W1, 67, rotW1); rot(W2, 67, rotW2); rot(W3, 67, rotW3); // rotation <<<61 W0,W1,W2,W3
    for (int i=0; i<16; i++) {
        enc_w[8][i] = W0[i] ^ rotW1[i];      // ek9  <- W0 ^ (W1<<<61) 
        enc_w[9][i] = W1[i] ^ rotW2[i];      // ek10 <- W1 ^ (W2<<<61)
        enc_w[10][i] = W2[i] ^ rotW3[i];     // ek11 <- W2 ^ (W3<<<61)
        enc_w[11][i] = W3[i] ^ rotW0[i];     // ek12 <- (W0<<<61) ^ W3
    }

    // Generate encrypt round key or whitening key for ARIA128/192/256
    memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
    rot(W1, 97, rotW1);
    for (int i=0; i<16; i++)
        enc_w[12][i] = W0[i] ^ rotW1[i];     // ek13 <- W0 ^ (W1<<<31) (ARIA-128 whiteningkey)

    //  Generate decrypt round key (Parallel for dec_w[0, 1, 2, ...])
    for (int i=0; i<16; i++) {
        dec_w[0][i] = enc_w[12][i];       // dec_w[0] <- enc_w[round]
        dec_w[12][i] = enc_w[0][i];       // dec_w[round] <- enc_w[0];
    }
    for (int r=1; r<=12-1; r++) {
        diffLayer(enc_w[12-r], dec_w[r]); // dec_w[r] <- A(enc_w[round-r]) for r 1 to round-1
    }
}
void Aria::fo(uint8_t state[16], uint8_t key[16]) {

    // AddRoundKey
    for (int i=0; i<16; i++)
        state[i] ^= key[i];

    // SubstLayer type1 (LT||LT||LT||LT)
    for (int i=0; i<4; i++) {
        state[4*i] = sBox[0][state[4*i]];
        state[4*i+1] = sBox[1][state[4*i+1]];
        state[4*i+2] = isBox[0][state[4*i+2]];
        state[4*i+3] = isBox[1][state[4*i+3]];
    }

    // Difflayer
    diffLayer(state, state);
}
void Aria::fe(uint8_t state[16], uint8_t key[16]) {

    // AddRoundKey
    for (int i=0; i<16; i++)
        state[i] ^= key[i];
    
    // SubstLayer type2 (invLT||invLT||invLT||invLT)
    for (int i=0; i<4; i++) {
        state[4*i] = isBox[0][state[4*i]];
        state[4*i+1] = isBox[1][state[4*i+1]];
        state[4*i+2] = sBox[0][state[4*i+2]];
        state[4*i+3] = sBox[1][state[4*i+3]];
    }

    // Difflayer
    diffLayer(state, state);
}
void Aria::ff(uint8_t state[16], string process) {

    // AddRoundKey
    for (int i=0; i<16; i++) {
        if (process=="encrypt")
            state[i] ^= enc_w[11][i];
        else if (process=="decrypt")
            state[i] ^= dec_w[11][i];
    }

    // SubstLayer type2 (invLT||invLT||invLT||invLT)
    for (int i=0; i<4; i++) {
        state[4*i] = isBox[0][state[4*i]];
        state[4*i+1] = isBox[1][state[4*i+1]];
        state[4*i+2] = sBox[0][state[4*i+2]];
        state[4*i+3] = sBox[1][state[4*i+3]];
    }

    // AddRoundKey - Whitening key
    for (int i=0; i<16; i++) {
        if (process=="encrypt")
            state[i] ^= enc_w[12][i];
        else if (process=="decrypt")
            state[i] ^= dec_w[12][i];
    }
}
void Aria::diffLayer(uint8_t src[16], uint8_t dst[16]) {

    uint8_t temp[16];

    for (int i=0; i<16; i++)
        temp[i] = src[i];

    dst[0]=temp[3]^temp[4]^temp[6]^temp[8]^temp[9]^temp[13]^temp[14];
    dst[1]=temp[2]^temp[5]^temp[7]^temp[8]^temp[9]^temp[12]^temp[15];
    dst[2]=temp[1]^temp[4]^temp[6]^temp[10]^temp[11]^temp[12]^temp[15];
    dst[3]=temp[0]^temp[5]^temp[7]^temp[10]^temp[11]^temp[13]^temp[14];

    dst[4]=temp[0]^temp[2]^temp[5]^temp[8]^temp[11]^temp[14]^temp[15];
    dst[5]=temp[1]^temp[3]^temp[4]^temp[9]^temp[10]^temp[14]^temp[15];
    dst[6]=temp[0]^temp[2]^temp[7]^temp[9]^temp[10]^temp[12]^temp[13];
    dst[7]=temp[1]^temp[3]^temp[6]^temp[8]^temp[11]^temp[12]^temp[13];

    dst[8]=temp[0]^temp[1]^temp[4]^temp[7]^temp[10]^temp[13]^temp[15];
    dst[9]=temp[0]^temp[1]^temp[5]^temp[6]^temp[11]^temp[12]^temp[14];
    dst[10]=temp[2]^temp[3]^temp[5]^temp[6]^temp[8]^temp[13]^temp[15];
    dst[11]=temp[2]^temp[3]^temp[4]^temp[7]^temp[9]^temp[12]^temp[14];
	
    dst[12]=temp[1]^temp[2]^temp[6]^temp[7]^temp[9]^temp[11]^temp[12];
    dst[13]=temp[0]^temp[3]^temp[6]^temp[7]^temp[8]^temp[10]^temp[13];
    dst[14]=temp[0]^temp[3]^temp[4]^temp[5]^temp[9]^temp[11]^temp[14];
    dst[15]=temp[1]^temp[2]^temp[4]^temp[5]^temp[8]^temp[10]^temp[15];
        
}
void Aria::rot(uint8_t input[16], int bits, uint8_t output[16]) {
    int q = bits / 8;
    int r = bits % 8;
    for (int i=0; i<16; i++) {
        output[(q+i) % 16] ^= input[i] >> r;
        if ( r != 0 )
            output[(q+i+1) % 16] ^= (input[i] << (8-r) & 0xfe);
    }
}
void Aria::encrypt() {

    uint8_t state[16] = { 0x00, };
    for (int i=0; i<16; i++)
        state[i] = pt[i];
    
    // 1 ~ 11/13/15 round (ARIA-128/192/256)
    for (int r=1; r<12; r++) {
        if ( (r%2) == 1 )
            fo(state, enc_w[r-1]);
        else
            fe(state, enc_w[r-1]);
    }
    // Final round : 12/14/16 (ARIA-128/192/256)
    ff(state, "encrypt");

    // deep copy
    for (int i=0; i<16; i++)
        ct[i] = state[i];
}
void Aria::decrypt() {

    uint8_t state[16] = { 0x00, };
    for (int i=0; i<16; i++)
        state[i] = ct[i];
    
    // 1 ~ 11/13/15 round (ARIA-128/192/256)
    for (int r=1; r<12; r++) {
        if ( (r%2) == 1 )
            fo(state, dec_w[r-1]);
        else
            fe(state, dec_w[r-1]);
    }
    // Final round : 12/14/16 (ARIA-128/192/256)
    ff(state, "decrypt");

    // deep copy
    for (int i=0; i<16; i++)
        recovered[i] = state[i];
}
void Aria::printKey() {
    cout << " Master key = " << endl;
    for (int i=0; i<keyLen; i++) {
        printf("%02x ", key[i]);
        if ((i%16)==15) cout << endl;
    }

    cout << endl << " Encrypt Round key = " << endl;
    for (int i=0; i<13; i++) {
        for (int j=0; j<16; j++)
            printf("0x%02x ", enc_w[i][j]);
        printf("\n");
    }
    cout << endl << " Decrypt Round key = " << endl;
    for (int i=0; i<13; i++) {
        for (int j=0; j<16; j++)
            printf("0x%02x ", dec_w[i][j]);
        printf("\n");
    }
}
void Aria::printCiphertext() {
    cout << " Ciphertext = " << endl;
    for (int i=0; i<Crypto::ctLen; i++) {
        printf("%02x ", Crypto::ct[i]);
        if ((i%16)==15) cout << endl;
    }
}
void Aria::printPlaintext() {
    cout << " Plaintext = " << endl;
    for (int i=0; i<Crypto::ptLen; i++) {
        printf("%02x ", Crypto::pt[i]);
        if ((i%16)==15) cout << endl;
    }
}
void Aria::printDecryptedtext() {
    cout << " Recovered = " << endl;
    for (int i=0; i<Crypto::ptLen; i++) {
        printf("%02x ", Crypto::recovered[i]);
        if ((i%16)==15) cout << endl;
    }

}

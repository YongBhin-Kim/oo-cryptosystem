#ifndef GALOISFIELD_H
#define GALOISFIELD_H

#include <stdint.h>
#include "crypto.h"

class GaloisField {
    static uint8_t gfAdd(uint8_t f, uint8_t g);
    static uint8_t gfXtime(uint8_t f);
    static uint8_t gfMul(uint8_t f, uint8_t g);
    uint8_t mat[8][8];
public:
    int row;
    int col;
    
    GaloisField(int row=0, int col=0); // parameterized(default) constructor
    GaloisField(const GaloisField &gf); // copy constructor
    void setValue(uint8_t value, int r, int c);
    uint8_t getValue(int r, int c);

    friend GaloisField operator*(const GaloisField &mc, const GaloisField &x);
    GaloisField &operator=(const GaloisField &gf);
};

#endif
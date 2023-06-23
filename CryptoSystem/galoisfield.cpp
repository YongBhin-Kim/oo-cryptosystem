#include "galoisfield.h"
using namespace std;

GaloisField::GaloisField(int row, int col) {
    this->row = row;
    this->col = col;
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
            mat[i][j] = 0;
}
GaloisField::GaloisField(const GaloisField &gf) {
    this->row = gf.row;
    this->col = gf.col;
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
            mat[i][j] = gf.mat[i][j];
}
GaloisField operator*(const GaloisField &m, const GaloisField &x) {
    // Init ab
    GaloisField garbage(1,1);
    garbage.mat[0][0]=1;

    if (m.col != x.row) {
        cout << "Matrix size error! \n";
        return garbage;
    }

    uint8_t temp;
    GaloisField mx(m.row, x.col); // 4 x 1 vector

    for (int i=0; i<mx.row; i++)
        for (int j=0; j<mx.col; j++) {        
            mx.mat[i][j] = 0; // initialize

            for (int k=0; k<m.col; k++) { // m.col == x.row
                temp = GaloisField::gfMul(m.mat[i][k], x.mat[k][j]); // 각 원소의 곱은 GF(2^8)에서의 곱셈
                mx.mat[i][j] = GaloisField::gfAdd(mx.mat[i][j], temp);
            }
        }
    return mx;
}
GaloisField &GaloisField::operator=(const GaloisField &gf) {
    if ( (this!=&gf) ) {
        this->row = gf.row;
        this->col = gf.col;
        for (int i=0; i<row; i++)
            for (int j=0; j<col; j++)
                this->mat[i][j] = gf.mat[i][j];
    }
    return *this;
}
uint8_t GaloisField::gfAdd(uint8_t f, uint8_t g) {
    return f^g;
}
uint8_t GaloisField::gfXtime(uint8_t f) {
    return ( ((f >> 7) & 0x01) == 1 ? (f << 1) ^ 0x1b : f << 1 );
}
uint8_t GaloisField::gfMul(uint8_t f, uint8_t g) {
    uint8_t h;
    int coef;
    h = 0x00;
    for(int i=7; i>=0; i--) {
        coef = (f>>i) & 0x01;
        if (h != 0)
            h = gfXtime(h);
        if (coef == 1)
            h = gfAdd(h, g);
    }
    return h;
}
void GaloisField::setValue(uint8_t value, int r, int c) {
    mat[r][c] = value;
}
uint8_t GaloisField::getValue(int r, int c) {
    return mat[r][c];
}
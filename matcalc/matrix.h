#ifndef PIXEL_MATCALC_MATRIX_H
#define PIXEL_MATCALC_MATRIX_H

#include <stddef.h>

typedef struct Matrix {
    size_t rows;
    size_t cols;
    float* data;
} Matrix;

#ifdef __cplusplus
extern "C" {
#endif

// A(mxk), B(kxn), C（mxn）
// C = A B
void matrix_multiply(float* mA, float* mB, float* mC, size_t M, size_t K, size_t N);

// A(mxn), B(mxn), C(mxn)
// C = A + B
void matrix_add(float* mA, float* mB, float* mC, size_t M, size_t N);

// A(mxn), B(mxn), C(mxn)
// C = A - B
void matrix_sub(float* mA, float* mB, float* mC, size_t M, size_t N);

// TODO: matrix inverse

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_H
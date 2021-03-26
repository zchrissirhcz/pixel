#ifndef PIXEL_MATCALC_MATRIX_ADD_SUB_H
#define PIXEL_MATCALC_MATRIX_ADD_SUB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// A(mxn), B(mxn), C(mxn)
// C = A + B
void matrix_add_f32_naive(float* mA, float* mB, float* mC, size_t M, size_t N);

void matrix_add_f32_asimd(float* mA, float* mB, float* mC, size_t M, size_t N);


//----------------------------------------------------------------------

// A(mxn), B(mxn), C(mxn)
// C = A - B
void matrix_sub(float* mA, float* mB, float* mC, size_t M, size_t N);


#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_ADD_SUB_H
#ifndef PIXEL_MATCALC_MATRIX_ADD_SUBTRACT_H
#define PIXEL_MATCALC_MATRIX_ADD_SUBTRACT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// A(mxn), B(mxn), C(mxn)
// C = A + B
void matrix_add_f32_naive(float* mA, float* mB, float* mC, const size_t M, const size_t N);

void matrix_add_f32_asimd(float* mA, float* mB, float* mC, const size_t M, const size_t N);


//----------------------------------------------------------------------

// A(mxn), B(mxn), C(mxn)
// C = A - B
void matrix_subtract_f32_naive(float* mA, float* mB, float* mC, const size_t M, const size_t N);

void matrix_subtract_f32_asimd(float* mA, float* mB, float* mC, const size_t M, const size_t N);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_ADD_SUBTRACT_H
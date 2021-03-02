#include "matrix.h"

// A(mxk), B(kxn), C（mxn）
void matrix_multiply_matrix(float* mA, float* mB, float* mC, size_t M, size_t K, size_t N)
{
    for (size_t i=0; i<M; i++) {
        for (size_t j=0; j<N; j++) {
            float sum = 0;
            for (size_t k=0; k<K; k++) {
                size_t a_idx = i*K + k;
                size_t b_idx = k*N + j;
                sum += mA[a_idx] * mB[b_idx];
            }
            size_t c_idx = i*N + j;
            mC[c_idx] = sum;
        }
    }
}
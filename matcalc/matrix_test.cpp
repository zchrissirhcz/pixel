#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

static void print_matrix(float* data, size_t rows, size_t cols) {
    printf("------------------\n");
    size_t idx=0;
    for (size_t i=0; i<rows; i++){
        for (size_t j=0; j<cols; j++) {
            printf("%.2f, ", data[idx]);
            idx++;
        }
        printf("\n");
    }
}

int main() {

    size_t m = 3;
    size_t k = 2;
    size_t n = 4;

    Matrix mA;
    mA.rows = m;
    mA.cols = k;
    
    Matrix mB;
    mB.rows = k;
    mB.cols = n;

    mA.data = (float*)malloc(mA.rows*mA.cols * sizeof(float));
    mB.data = (float*)malloc(mB.rows*mB.cols * sizeof(float));

    mA.data[0] = 2;
    mA.data[1] = -6;
    mA.data[2] = 3;
    mA.data[3] = 5;
    mA.data[4] = 1;
    mA.data[5] = -1;

    mB.data[0] = 4;
    mB.data[1] = -2;
    mB.data[2] = -4;
    mB.data[3] = -5;
    mB.data[4] = -7;
    mB.data[5] = -3;
    mB.data[6] = 6;
    mB.data[7] = 7;

    Matrix mC;
    mC.rows = m;
    mC.cols = n;
    mC.data = (float*)malloc(mC.rows*mC.cols * sizeof(float));

    matrix_multiply_matrix(mA.data, mB.data, mC.data, m, k, n);

    print_matrix(mA.data, mA.rows, mA.cols);
    print_matrix(mB.data, mB.rows, mB.cols);
    print_matrix(mC.data, mC.rows, mC.cols);

    return 0;
}
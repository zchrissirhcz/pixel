#ifndef PIXEL_CHECK_H
#define PIXEL_CHECK_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

//----------------------------------------------------------------------
// function declarations
//----------------------------------------------------------------------
static int check_array_equal(size_t len, float* data, float* gt);
static int check_array_equal_u8(size_t len, uint8_t* data, uint8_t* gt);
static bool nearly_equal_absolutely(float a, float b, float epsilon);
static bool nearly_equal_normally(float a, float b, float epsilon);

//----------------------------------------------------------------------
// function implementations
//----------------------------------------------------------------------
#include <math.h>

bool nearly_equal_absolutely(float a, float b, float epsilon) {
    return fabs(a - b) <= epsilon;
}

bool nearly_equal_normally(float a, float b, float epsilon) {
    if (a == b)
        return true;

    float diff = (float)fabs(a - b);
    if (diff <= epsilon)
        return true;
    float max_val = (fabs(a)>fabs(b)) ? fabs(a) : fabs(b);
    return diff < epsilon * max_val;
}

int check_array_equal(size_t len, float* data, float* gt)
{
    int cnt = 0;
    for (size_t i=0; i<len; i++) {
        //if (data[i]!=gt[i]) {
        if (!nearly_equal_absolutely(data[i], gt[i], 0.001)) {
            cnt++;
            printf("expect %.4lf, got %.4lf\n", gt[i], data[i]);
        }
    }
    return cnt;
}


int check_array_equal_u8(size_t len, uint8_t* data, uint8_t* gt)
{
    int cnt = 0;
    for (size_t i=0; i<len; i++) {
        if (data[i]!=gt[i]) {
            cnt++;
            printf("expect %d, got %d\n", gt[i], data[i]);
        }
    }
    return cnt;
}

#endif // PIXEL_CHECK_H
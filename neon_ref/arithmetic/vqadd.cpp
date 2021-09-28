#include "vqadd.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqadd_type
//----------------------------------------------------------------------
int8x8_t vqadd_s8(int8x8_t N, int8x8_t M)
{
    int8x8_t D;
    for (size_t i=0; i<8; i++)
    {
        int16_t temp = (int16_t)N[i] + (int16_t)M[i];
        if (temp > INT8_MAX) {
            D[i] = INT8_MAX;
        }
        else if(temp < INT8_MIN) {
            D[i] = INT8_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

int16x4_t vqadd_s16(int16x4_t N, int16x4_t M)
{
    int16x4_t D;
    for (size_t i=0; i<4; i++)
    {
        int32_t temp = (int32_t)N[i] + (int32_t)M[i];
        if (temp > INT16_MAX) {
            D[i] = INT16_MAX;
        }
        else if (temp < INT16_MIN){
            D[i] = INT16_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

int32x2_t vqadd_s32(int32x2_t N, int32x2_t M)
{
    int32x2_t D;
    for (size_t i=0; i<2; i++)
    {
        int64_t temp = (int64_t)N[i] + (int64_t)M[i];
        if (temp > INT32_MAX) {
            D[i] = INT32_MAX;
        }
        else if (temp < INT32_MIN) {
            D[i] = INT32_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

int64x1_t vqadd_s64(int64x1_t N, int64x1_t M)
{
    int64x1_t D;
    double temp = (double)N[0] + (double)M[0];
    if (temp > INT64_MAX) {
        D[0] = INT64_MAX;
    }
    else if (temp < INT64_MIN) {
        D[0] = INT64_MIN;
    }
    else {
        D[0] = temp;
    }
    return D;
}

uint8x8_t vqadd_u8(uint8x8_t N, uint8x8_t M)
{
    uint8x8_t D;
    for (size_t i=0; i<8; i++)
    {
        uint16_t temp = (uint16_t)N[i] + (uint16_t)M[i];
        if (temp > UINT8_MAX) {
            D[i] = UINT8_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint16x4_t vqadd_u16(uint16x4_t N, uint16x4_t M)
{
    uint16x4_t D;
    for (size_t i=0; i<4; i++)
    {
        uint32_t temp = (uint32_t)N[i] + (uint32_t)M[i];
        if (temp > UINT16_MAX) {
            D[i] = UINT16_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint32x2_t vqadd_u32(uint32x2_t N, uint32x2_t M)
{
    uint32x2_t D;
    for (size_t i=0; i<2; i++)
    {
        uint64_t temp = (uint64_t)N[i] + (uint64_t)M[i];
        if (temp > UINT32_MAX) {
            D[i] = UINT32_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint64x1_t vqadd_s64(uint64x1_t N, uint64x1_t M)
{
    uint64x1_t D;
    double temp = (double)N[0] + (double)M[0];
    if (temp > UINT64_MAX) {
        D[0] = UINT64_MAX;
    }
    else {
        D[0] = temp;
    }
    return D;
}

//----------------------------------------------------------------------
// vqaddq_type
//----------------------------------------------------------------------
int8x16_t vqaddq_s8(int8x16_t N, int8x16_t M)
{
    int8x16_t D;
    for (size_t i=0; i<16; i++)
    {
        int16_t temp = (int16_t)N[i] + (int16_t)M[i];
        if (temp > INT8_MAX) {
            D[i] = INT8_MAX;
        }
        else if(temp < INT8_MIN) {
            D[i] = INT8_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

int16x8_t vqaddq_s16(int16x8_t N, int16x8_t M)
{
    int16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        int32_t temp = (int32_t)N[i] + (int32_t)M[i];
        if (temp > INT16_MAX) {
            D[i] = INT16_MAX;
        }
        else if(temp < INT16_MIN) {
            D[i] = INT16_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

int32x4_t vqaddq_s32(int32x4_t N, int32x4_t M)
{
    int32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        int64_t temp = (int64_t)N[i] + (int64_t)M[i];
        if (temp > INT32_MAX) {
            D[i] = INT32_MAX;
        }
        else if(temp < INT32_MIN) {
            D[i] = INT32_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

int64x2_t vqaddq_s64(int64x2_t N, int64x2_t M)
{
    int64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        double temp = (double)N[i] + (double)M[i];
        if (temp > INT64_MAX) {
            D[i] = INT64_MAX;
        }
        else if(temp < INT64_MIN) {
            D[i] = INT64_MIN;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint8x16_t vqaddq_u8(uint8x16_t N, uint8x16_t M)
{
    uint8x16_t D;
    for (size_t i=0; i<16; i++)
    {
        uint16_t temp = (uint16_t)N[i] + (uint16_t)M[i];
        if (temp > UINT8_MAX) {
            D[i] = UINT8_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint16x8_t vqaddq_u16(uint16x8_t N, uint16x8_t M)
{
    uint16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        uint32_t temp = (uint32_t)N[i] + (uint32_t)M[i];
        if (temp > UINT16_MAX) {
            D[i] = UINT16_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint32x4_t vqaddq_u32(uint32x4_t N, uint32x4_t M)
{
    uint32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        uint64_t temp = (uint64_t)N[i] + (uint64_t)M[i];
        if (temp > UINT32_MAX) {
            D[i] = UINT32_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

uint64x2_t vqaddq_u64(uint64x2_t N, uint64x2_t M)
{
    uint64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        double temp = (double)N[i] + (double)M[i];
        if (temp > UINT64_MAX) {
            D[i] = UINT64_MAX;
        }
        else {
            D[i] = temp;
        }
    }
    return D;
}

} // namespace pxl
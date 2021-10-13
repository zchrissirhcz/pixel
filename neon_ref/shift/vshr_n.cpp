#include "vshr_n.hpp"

namespace pxl {

// VSHR_N right shifts each element in a vector by an immediate value, and places the results in the destination vector.

//----------------------------------------------------------------------
// vshr_n_type
//----------------------------------------------------------------------

/// @param n 1-8
int8x8_t vshr_n_s8(int8x8_t v, const int n)
{
    int8x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = v[i] >> n; // 注意是 >> n 而不是 / (1<<n)， 因为v可能是负数，负数时>>和/结果不同
    }
    return D;
}

/// @param n 1-16
int16x4_t vshr_n_s16(int16x4_t v, const int n)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}

/// @param n 1-32
int32x2_t vshr_n_s32(int32x2_t v, const int n)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-64
int64x1_t vshr_n_s64(int64x1_t v, const int n)
{
    int64x1_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-8
uint8x8_t vshr_n_u8(uint8x8_t v, const int n)
{
    uint8x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-16
uint16x4_t vshr_n_u16(uint16x4_t v, const int n)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-32
uint32x2_t vshr_n_u32(uint32x2_t v, const int n)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-64
uint64x1_t vshr_n_u64(uint64x1_t v, const int n)
{
    uint64x1_t D;
    for (int i=0; i<1; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


//----------------------------------------------------------------------
// vshrq_n_type
//----------------------------------------------------------------------

/// @param n 1-8
int8x16_t vshrq_n_s8(int8x16_t v, const int n)
{
    int8x16_t D;
    for (int i=0; i<16; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-16
int16x8_t vshrq_n_s16(int16x8_t v, const int n)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-32
int32x4_t vshrq_n_s32(int32x4_t v, const int n)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-64
int64x2_t vshrq_n_s64(int64x2_t v, const int n)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-8
uint8x16_t vshrq_n_u8(uint8x16_t v, const int n)
{
    uint8x16_t D;
    for (int i=0; i<16; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-16
uint16x8_t vshrq_n_u16(uint16x8_t v, const int n)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-32
uint32x4_t vshrq_n_u32(uint32x4_t v, const int n)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}


/// @param n 1-64
uint64x2_t vshrq_n_u64(uint64x2_t v, const int n)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = v[i] >> n;
    }
    return D;
}



} // namespace pxl

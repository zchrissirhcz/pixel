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
        D[i] = v[i] >> n;
    }
    return D;
}

/// @param n 1-16
int16x4_t vshr_n_s16(int16x4_t v, const int n);

/// @param n 1-32
int32x2_t vshr_n_s32(int32x2_t v, const int n);

/// @param n 1-64
int64x1_t vshr_n_s64(int64x1_t v, const int n);

/// @param n 1-8
uint8x8_t vshr_n_u8(uint8x8_t v, const int n);

/// @param n 1-16
uint16x4_t vshr_n_u16(uint16x4_t v, const int n);

/// @param n 1-32
uint32x2_t vshr_n_u32(uint32x2_t v, const int n);

/// @param n 1-64
uint64x1_t vshr_n_u64(uint64x1_t v, const int n);

//----------------------------------------------------------------------
// vshrq_n_type
//----------------------------------------------------------------------

/// @param n 1-8
int8x16_t vshrq_n_s8(int8x16_t v, const int n);

/// @param n 1-16
int16x8_t vshrq_n_s16(int16x8_t v, const int n);

/// @param n 1-32
int32x4_t vshrq_n_s32(int32x4_t v, const int n);

/// @param n 1-64
int64x2_t vshrq_n_s64(int64x2_t v, const int n);

/// @param n 1-8
uint8x16_t vshrq_n_u8(uint8x16_t v, const int n);

/// @param n 1-16
uint16x8_t vshrq_n_u16(uint16x8_t v, const int n);

/// @param n 1-32
uint32x4_t vshrq_n_u32(uint32x4_t v, const int n);

/// @param n 1-64
uint64x2_t vshrq_n_u64(uint64x2_t v, const int n);

} // namespace pxl

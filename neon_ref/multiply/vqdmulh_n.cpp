#include "vqdmulh_n.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmulh_n_type
//----------------------------------------------------------------------
int16x4_t vqdmulh_n_s16(int16x4_t M, int16_t N)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        int32_t temp = M[i] * N * 2;
        D[i] = temp >> 16;
    }
    return D;
}

int32x2_t vqdmulh_n_s32(int32x2_t M, int32_t N)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        int64_t temp = M[i] * N * 2;
        D[i] = temp >> 32;
    }
    return D;
}

//----------------------------------------------------------------------
// vqdmulhq_n_type
//----------------------------------------------------------------------
int16x8_t vqdmulhq_n_s16(int16x8_t M, int16_t N)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        int32_t temp = M[i] * N * 2;
        D[i] = temp >> 16;
    }
    return D;
}

int32x4_t vqdmulhq_n_s32(int32x4_t M, int32_t N)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        int64_t temp = M[i] * N * 2;
        D[i] = temp >> 32;
    }
    return D;
}

} // namespace pxl
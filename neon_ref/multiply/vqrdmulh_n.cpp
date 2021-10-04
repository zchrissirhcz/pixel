#include "vqrdmulh_n.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqrdmulh_n_type
//----------------------------------------------------------------------
int16x4_t vqrdmulh_n_s16(int16x4_t N, int16_t M)
{
    int16x4_t D;
    const int32_t delta = 1<<15;
    for (int i=0; i<4; i++)
    {
        int32_t temp = N[i] * M * 2;
        D[i] = (temp + delta) >> 16;
    }
    return D;
}

int32x2_t vqrdmulh_n_s32(int32x2_t N, int32_t M)
{
    int32x2_t D;
    const int64_t delta = 1<<31;
    for (int i=0; i<2; i++)
    {
        int32_t temp = N[i] * M * 2;
        D[i] = (temp + delta) >> 32;
    }
    return D;
}

//----------------------------------------------------------------------
// vqrdmulhq_n_type
//----------------------------------------------------------------------
int16x8_t vqrdmulhq_n_s16(int16x8_t N, int16_t M)
{
    int16x8_t D;
    const int32_t delta = 1<<15;
    for (int i=0; i<8; i++)
    {
        int32_t temp = N[i] * M * 2;
        D[i] = (temp + delta) >> 16;
    }
    return D;
}

int32x4_t vqrdmulhq_n_s32(int32x4_t N, int32_t M)
{
    int32x4_t D;
    const int64_t delta = 1<<31;
    for (int i=0; i<4; i++)
    {
        int32_t temp = N[i] * M * 2;
        D[i] = (temp + delta) >> 32;
    }
    return D;
}

} // namespace pxl
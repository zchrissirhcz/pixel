#include "vabdl.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vabdl_type
//----------------------------------------------------------------------
int16x8_t vabdl_s8(int8x8_t N, int8x8_t M)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = abs(N[i]-M[i]);
    }
    return D;
}

int32x4_t vabdl_s16(int16x4_t N, int16x4_t M)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = abs(N[i]-M[i]);
    }
    return D;
}

int64x2_t vabdl_s32(int32x2_t N, int32x2_t M)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = abs(N[i]-M[i]);
    }
    return D;
}

uint16x8_t vabdl_u8(uint8x8_t N, uint8x8_t M)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] > M[i] ? N[i] - M[i] : M[i] - N[i];
    }
    return D;
}

uint32x4_t vabdl_u16(uint16x4_t N, uint16x4_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] > M[i] ? N[i] - M[i] : M[i] - N[i];
    }
    return D;
}

uint64x2_t vabdl_u32(uint32x2_t N, uint32x2_t M)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] > M[i] ? N[i] - M[i] : M[i] - N[i];
    }
    return D;
}


} // namespace pxl
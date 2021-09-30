#include "vmull_n.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmull_n_type
//----------------------------------------------------------------------

int32x4_t vmull_n_s16(int16x4_t N, int16_t M)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] * M;
    }
    return D;
}

int64x2_t vmull_n_s32(int32x2_t N, int32_t M)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] * M;
    }
    return D;
}

uint32x4_t vmull_n_u16(uint16x4_t N, uint16_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] * M;
    }
    return D;
}

uint64x2_t vmull_n_u32(uint32x2_t N, uint32_t M)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] * M;
    }
    return D;
}

} // namespace pxl
// template<typename Result_t, typename Vector1_t, typename Vector2_t=Vector1_t>
// Result_t vaddl_type(Vector1_t N, Vector2_t M)
// {
//     Result_t D;
//     for (i..len)
//     {
//         D[i] = wide(N[i] + M[i])
//     }
// }

#include "vaddl.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vaddl_type
//----------------------------------------------------------------------
int16x8_t vaddl_s8(int8x8_t N, int8x8_t M)
{
    int16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = (int16_t)(N[i]) + (int16_t)(M[i]);
    }
    return D;
}

int32x4_t vaddl_s16(int16x4_t N, int16x4_t M)
{
    int32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = (int32_t)(N[i]) + (int32_t)(M[i]);
    }
    return D;
}

int64x2_t vaddl_s32(int32x2_t N, int32x2_t M)
{
    int64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = (int64_t)(N[i]) + (int64_t)(M[i]);
    }
    return D;
}

uint16x8_t vaddl_u8(uint8x8_t N, uint8x8_t M)
{
    uint16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = (uint16_t)(N[i]) + (uint16_t)(M[i]);
    }
    return D;
}

uint32x4_t vaddl_u16(uint16x4_t N, uint16x4_t M)
{
    uint32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = (uint32_t)(N[i]) + (uint32_t)(M[i]);
    }
    return D;
}

uint64x2_t vaddl_u32(uint32x2_t N, uint32x2_t M)
{
    uint64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = (uint64_t)(N[i]) + (uint64_t)(M[i]);
    }
    return D;
}

} // namespace pxl
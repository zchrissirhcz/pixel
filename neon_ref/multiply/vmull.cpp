#include "vmull.hpp"

namespace pxl {

int16x8_t vmull_s8(int8x8_t N, int8x8_t M)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] * M[i];
    }
    return D;
}

int32x4_t vmull_s16(int16x4_t N, int16x4_t M);
int64x2_t vmull_s32(int32x2_t N, int32x2_t M);

uint16x8_t vmull_u8(uint8x8_t N, uint8x8_t M);
uint32x4_t vmull_u16(uint16x4_t N, uint16x4_t M);
uint64x2_t vmull_u32(uint32x2_t N, uint32x2_t M);

} // namespace pxl
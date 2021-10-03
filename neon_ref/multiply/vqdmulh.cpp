#include "vqdmulh.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmulh_type
//----------------------------------------------------------------------

// 将d0和d1相乘，结果再乘2，然后返回高一半位，或相当于结果>>16。因为此处两个16位向量相乘，结果应该为32位向量，所以高一半应该是16位。
// 官网还提到 rounding， 然而我觉得那写法肯定是针对多个类似指令写的，当前指令压根没处理rounding
// 中间结果用一个变量来存储是必要的，否则结果不正确。
int16x4_t vqdmulh_s16(int16x4_t M, int16x4_t N)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        int32_t temp = M[i] * N[i] * 2;
        D[i] = temp >> 16;
    }
    return D;
}

int32x2_t vqdmulh_s32(int32x2_t M, int32x2_t N)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        int64_t temp = M[i] * N[i] * 2;
        D[i] = temp >> 32;
    }
    return D;
}

//----------------------------------------------------------------------
// vqdmulhq_type
//----------------------------------------------------------------------
int16x8_t vqdmulhq_s16(int16x8_t M, int16x8_t N)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        int32_t temp = M[i] * N[i] * 2;
        D[i] = temp >> 16;
    }
    return D;
}

int32x4_t vqdmulhq_s32(int32x4_t M, int32x4_t N)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        int64_t temp = M[i] * N[i] * 2;
        D[i] = temp >> 32;
    }
    return D;
}

} // namespace pxl
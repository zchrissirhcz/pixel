#include "vrecpe.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vrecpe_type
//----------------------------------------------------------------------
uint32x2_t vrecpe_u32(uint32x2_t N)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = 1.0 / N[i];
    }
    return D;
}

float32x2_t vrecpe_f32(float32x2_t N)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = 1.0 / N[i];
    }
    return D;
}

//----------------------------------------------------------------------
// vrecpeq_type
//----------------------------------------------------------------------
uint32x4_t vrecpeq_u32(uint32x4_t N)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = 1.0 / N[i];
    }
    return D;
}

float FISqrt(float const number)
{
    int i;
    float x2;
    float y;
    const float threehalfs = 1.5f;
    x2 = number * 0.5f;
    y = number;
    i = *(int*)&y;
    i = 0x5f3759df - (i>>1);
    //i = 0x5f375a86 - (i>>1);
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));
    return y;
}

float fast_inv_sqrt(float x)
{
    float xhalf = 0.5f * x;
    int j = *(int*)&x;
    //j = 0x5f375a86 - (j>>1);
    //j = 0x5f3759df - (j>>1);
    j = 0x5f34ff59 - (j>>1);
    x = *(float*)&j;
    x = x*(1.5f - xhalf*x*x);
    return x;
}

float slow_reciprocal(float x)
{
    if (x<1) {
        for (float i=1; ; i+=1)
        {
            float diff = abs(i*i - x);
            if (diff < 1e-3) {
                return i;
            }
        }
    } else if (x>1) {
        return 
    }
    return 1;
}

/**
 * 备注： neon intrinsic 的 vrecpeq_f32 结果， 和实际的倒数相比， 精度不足
 * 但大概看一下是精确到小数点后3位的。
 * 
 * 直接上， 考虑用近似解法算出倒数， 然后和 intrinsics 的结果比较。
 * 做过一些尝试， 但失败了， 具体的遇到的困难是： 
 * - 直接用 Carmak 的 FastInverseSquareRoot 然后平方， 结果仍然有差异
 * - 找了一些博客， 讲怎么用 牛顿迭代法（newton-raphson）求解的，但是
 *      - 一方面，估计结果还是会对不上
 *      - 另一方面， 还没有具体实践， 没法直接拷贝用
 *      - https://blog.staynoob.cn/post/2016/09/the-famous-magic-number-fast-inverse-square-root-algorithm/
 *      - https://www.packetmania.net/2021/07/23/PGITVW-2-sqrt/
 * - 尝试在 StackOverFlow 发帖提问
 *      - https://stackoverflow.com/questions/69556488/reference-implementation-of-vrecpeq-f32-intrinsic
 *      - 网友直接指出说你贴出的文档里有进一步链接， 给了伪代码
 *      - 但是这个伪代码也太长了， 看到了就不想动了。
 */
float32x4_t vrecpeq_f32(float32x4_t N)
{
    float32x4_t D;
    for (int i=0; i<4; i++)
    {
        //D[i] = (1.0 / N[i] );
        // float temp = FISqrt(N[i]);

        // float temp = fast_inv_sqrt(N[i]);
        // D[i] = temp * temp;
        D[i] = slow_reciprocal(N[i]);
    }
    return D;
}

} // namespace pxl
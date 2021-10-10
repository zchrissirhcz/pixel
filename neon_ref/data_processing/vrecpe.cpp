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

float32x4_t vrecpeq_f32(float32x4_t N)
{
    float32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = 1.0 / N[i];
    }
    return D;
}

} // namespace pxl
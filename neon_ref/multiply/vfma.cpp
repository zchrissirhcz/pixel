#include "vfma.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vfma_type
//----------------------------------------------------------------------
float32x2_t vfma_f32(float32x2_t N, float32x2_t M, float32x2_t P)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] + M[i] * P[i];
    }
    return D;
}

//----------------------------------------------------------------------
// vfmaq_type
//----------------------------------------------------------------------
float32x4_t vfmaq_f32(float32x4_t N, float32x4_t M, float32x4_t P)
{
    float32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] + M[i] * P[i];
    }
    return D;
}

} // namespace pxl
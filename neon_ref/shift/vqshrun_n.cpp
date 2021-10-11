#include "vqshrun_n.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqshrun_n_type
//----------------------------------------------------------------------

/*
VQSHRUN_N right shifts each element in a quadword vector of integers by an immediate value, and
places the results in a doubleword vector. The results are unsigned, although the operands are
signed. The sticky QC flag (FPSCR bit[27]) is set if saturation occurs.
*/

/// @param n 1-8
uint8x8_t vqshrun_n_s16(int16x8_t v, const int n)
{
    uint8x8_t D;
    for (int i=0; i<8; i++) {
        int16_t temp = ( v[i] + (1<<(n-1) ) ) >> n;
		if (temp > UINT8_MAX) {
            temp = UINT8_MAX;
        } else if (temp < 0) {
            temp = 0;
        }
    }
	return D;
}

/// @param n 1-16
uint16x4_t vqshrun_n_s32(int32x4_t v, const int n);

/// @param n 1-32
uint32x2_t vqshrun_n_s64(int64x2_t v, const int n);


} // namespace pxl

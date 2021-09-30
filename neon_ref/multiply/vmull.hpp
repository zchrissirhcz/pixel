#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

// 注意： vmul_lane_type 这些intrinsic在arm_neon.h里是宏定义实现的，会和此处的函数实现冲突；
// 暂时的解决办法是， 在开启 __ARM_NEON 的情况下， 不用此处的函数实现
// 换言之， 对于用宏定义实现的 intrinsic， 无法在 ARM 平台上和 arm_neon.h 里的宏定义共存。

int16x8_t vmull_s8(int8x8_t N, int8x8_t M);
int32x4_t vmull_s16(int16x4_t N, int16x4_t M);
int64x2_t vmull_s32(int32x2_t N, int32x2_t M);

uint16x8_t vmull_u8(uint8x8_t N, uint8x8_t M);
uint32x4_t vmull_u16(uint16x4_t N, uint16x4_t M);
uint64x2_t vmull_u32(uint32x2_t N, uint32x2_t M);

} // namespace pxl
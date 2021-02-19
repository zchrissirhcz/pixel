#ifndef PIXEL_SIMD_NEON_H
#define PIXEL_SIMD_NEON_H

#include "pixel_simd_base.h"

//----------------------------------------------------------------------
// assign all element to zero
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_setzero_s8() { return vdup_n_s8(0); }
inline v_int16x4 vd_setzero_s16() { return vdup_n_s16(0); }
inline v_int32x2 vd_setzero_s32() { return vdup_n_s32(0); }
inline v_int64x1 vd_setzero_s64() { return vdup_n_s64(0); }
inline v_float32x2 vd_setzero_f32() { return vdup_n_f32(0); }
inline v_uint8x8 vd_setzero_u8() { return vdup_n_u8(0); }
inline v_uint16x4 vd_setzero_u16() { return vdup_n_u16(0); }
inline v_uint32x2 vd_setzero_u32() { return vdup_n_u32(0); }
inline v_uint64x1 vd_setzero_u64() { return vdup_n_u64(0); }

// 16bytes(128bits) part
inline v_int8x16 vq_setzero_s8() { return vdupq_n_s8(0); }
inline v_int16x8 vq_setzero_s16() { return vdupq_n_s16(0); }
inline v_int32x4 vq_setzero_s32() { return vdupq_n_s32(0); }
inline v_int64x2 vq_setzero_s64() { return vdupq_n_s64(0); }
inline v_float32x4 vq_setzero_f32() { return vdupq_n_f32(0); }
inline v_uint8x16 vq_setzero_u8() { return vdupq_n_u8(0); }
inline v_uint16x8 vq_setzero_u16() { return vdupq_n_u16(0); }
inline v_uint32x4 vq_setzero_u32() { return vdupq_n_u32(0); }
inline v_uint64x2 vq_setzero_u64() { return vdupq_n_u64(0); }

//----------------------------------------------------------------------
// assign all element to value
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8      vd_setvalue_s8(int8_t value) { return vdup_n_s8(value); }
inline v_int16x4     vd_setvalue_s16(int16_t value) { return vdup_n_s16(value); }
inline v_int32x2     vd_setvalue_s32(int32_t value) { return vdup_n_s32(value); }
inline v_int64x1     vd_setvalue_s64(int64_t value) { return vdup_n_s64(value); }
inline v_float32x2   vd_setvalue_f32(float value) { return vdup_n_f32(value); }
inline v_uint8x8     vd_setvalue_u8(uint8_t value) { return vdup_n_u8(value); }
inline v_uint16x4    vd_setvalue_u16(uint16_t value) { return vdup_n_u16(value); }
inline v_uint32x2    vd_setvalue_u32(uint32_t value) { return vdup_n_u32(value); }
inline v_uint64x1    vd_setvalue_u64(uint64_t value) { return vdup_n_s64(value); }

// 16bytes(128bits) part
inline v_int8x16     vq_setvalue_s8(int8_t value) { return vdupq_n_s8(value); }
inline v_int16x8     vq_setvalue_s16(int16_t value) { return vdupq_n_s16(value); }
inline v_int32x4     vq_setvalue_s32(int32_t value) { return vdupq_n_s32(value); }
inline v_int64x2     vq_setvalue_s64(int64_t value) { return vdupq_n_s64(value); }
inline v_float32x4   vq_setvalue_f32(float value) { return vdupq_n_f32(value); }
inline v_uint8x16    vq_setvalue_u8(uint8_t value) { return vdupq_n_u8(value); }
inline v_uint16x8    vq_setvalue_u16(uint16_t value) { return vdupq_n_u16(value); }
inline v_uint32x4    vq_setvalue_u32(uint32_t value) { return vdupq_n_u32(value); }
inline v_uint64x2    vq_setvalue_u64(uint64_t value) { return vdupq_n_u64(value); }

//----------------------------------------------------------------------
// copy data, memory => register
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_load_s8 (const int8_t* data) { return vld1_s8(data); }
inline v_int16x4 vd_load_s16(const int16_t* data) { return vld1_s16(data); }
inline v_int32x2 vd_load_s32(const int32_t* data) { return vld1_s32(data); }
inline v_int64x1 vd_load_s64(const int64_t* data) { return vld1_s64(data); }
inline v_float32x2 vd_load_f32(const float* data) { return vld1_f32(data); }
inline v_uint8x8 vd_load_u8 (const uint8_t* data) { return vld1_u8(data); }
inline v_uint16x4 vd_load_u16(const uint16_t* data) { return vld1_u16(data); }
inline v_uint32x2 vd_load_u32(const uint32_t* data) { return vld1_u32(data); }
inline v_uint64x1 vd_load_u64(const uint64_t* data) { return vld1_u64(data); }

// 16bytes(128bits) part
inline v_int8x16 vq_load_s8 (const int8_t* data) { return vld1q_s8(data); }
inline v_int16x8 vq_load_s16(const int16_t* data) { return vld1q_s16(data); }
inline v_int32x4 vq_load_s32(const int32_t* data) { return vld1q_s32(data); }
inline v_int64x2 vq_load_s64(const int64_t* data) { return vld1q_s64(data); }
inline v_float32x4 vq_load_f32(const float* data) { return vld1q_f32(data); }
inline v_uint8x16 vq_load_u8 (const uint8_t* data) { return vld1q_u8(data); }
inline v_uint16x8 vq_load_u16(const uint16_t* data) { return vld1q_u16(data); }
inline v_uint32x4 vq_load_u32(const uint32_t* data) { return vld1q_u32(data); }
inline v_uint64x2 vq_load_u64(const uint64_t* data) { return vld1q_u64(data); }

//----------------------------------------------------------------------
// copy-data, register => memory
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline void vd_store_s8(int8_t* data, v_int8x8 v) { vst1_s8(data, v); }
inline void vd_store_s16(int16_t* data, v_int16x4 v) { vst1_s16(data, v); }
inline void vd_store_s32(int32_t* data, v_int32x2 v) { vst1_s32(data, v); }
inline void vd_store_s64(int64_t* data, v_int64x1 v) { vst1_s64(data, v); }
inline void vd_store_f32(float* data, v_float32x2 v) { vst1_f32(data, v); }
inline void vd_store_u8 (uint8_t* data, v_uint8x8 v) { vst1_u8(data, v); }
inline void vd_store_u16(uint16_t* data, v_uint16x4 v) { vst1_u16(data, v); }
inline void vd_store_u32(uint32_t* data, v_uint32x2 v) { vst1_u32(data, v); }
inline void vd_store_u64(uint64_t* data, v_uint64x1 v) { vst1_u64(data, v); }

// 16bytes(128bits) part
inline void vq_store_s8(int8_t* data, v_int8x16 v) { vst1q_s8(data, v); }
inline void vq_store_s16(int16_t* data, v_int16x8 v) { vst1q_s16(data, v); }
inline void vq_store_s32(int32_t* data, v_int32x4 v) { vst1q_s32(data, v); }
inline void vq_store_s64(int64_t* data, v_int64x2 v) { vst1q_s64(data, v); }
inline void vq_store_f32(float* data, v_float32x4 v) { vst1q_f32(data, v); }
inline void vq_store_u8(uint8_t* data, v_uint8x16 v) { vst1q_u8(data, v); }
inline void vq_store_u16(uint16_t* data, v_uint16x8 v) { vst1q_u16(data, v); }
inline void vq_store_u32(uint32_t* data, v_uint32x4 v) { vst1q_u32(data, v); }
inline void vq_store_u64(uint64_t* data, v_uint64x2 v) { vst1q_u64(data, v); }

//----------------------------------------------------------------------
// add(v1, v2), return v1[i] + v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_add_s8(v_int8x8 v1, v_int8x8 v2) { return vadd_s8(v1, v2); }
inline v_int16x4 vd_add_s16(v_int16x4 v1, v_int16x4 v2) { return vadd_s16(v1, v2); }
inline v_int32x2 vd_add_s32(v_int32x2 v1, v_int32x2 v2) { return vadd_s32(v1, v2); }
inline v_int64x1 vd_add_s64(v_int64x1 v1, v_int64x1 v2) { return vadd_s64(v1, v2); }
inline v_float32x2 vd_add_f32(v_float32x2 v1, v_float32x2 v2) { return vadd_f32(v1, v2); }
inline v_uint8x8 vd_add_u8(v_uint8x8 v1, v_uint8x8 v2) { return vadd_u8(v1, v2); }
inline v_uint16x4 vd_add_u16(v_uint16x4 v1, v_uint16x4 v2) { return vadd_u16(v1, v2); }
inline v_uint32x2 vd_add_u32(v_uint32x2 v1, v_uint32x2 v2) { return vadd_u32(v1, v2); }
inline v_uint64x1 vd_add_u64(v_uint64x1 v1, v_uint64x1 v2) { return vadd_u64(v1, v2); }

// 16bytes(128bits) part
inline v_int8x16 vq_add_s8(v_int8x16 v1, v_int8x16 v2) { return vaddq_s8(v1, v2); }
inline v_int16x8 vq_add_s16(v_int16x8 v1, v_int16x8 v2) { return vaddq_s16(v1, v2); }
inline v_int32x4 vq_add_s32(v_int32x4 v1, v_int32x4 v2) { return vaddq_s32(v1, v2); }
inline v_int64x2 vq_add_s64(v_int64x2 v1, v_int64x2 v2) { return vaddq_s64(v1, v2); }
inline v_float32x4 vq_add_f32(v_float32x4 v1, v_float32x4 v2) { return vaddq_f32(v1, v2); }
inline v_uint8x16 vq_add_u8(v_uint8x16 v1, v_uint8x16 v2) { return vaddq_u8(v1, v2); }
inline v_uint16x8 vq_add_u16(v_uint16x8 v1, v_uint16x8 v2) { return vaddq_u16(v1, v2); }
inline v_uint32x4 vq_add_u32(v_uint32x4 v1, v_uint32x4 v2) { return vaddq_u32(v1, v2); }
inline v_uint64x2 vq_add_u64(v_uint64x2 v1, v_uint64x2 v2) { return vaddq_u64(v1, v2); }

//----------------------------------------------------------------------
// mul(v1, v2), return v1[i] * v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_mul_s8(v_int8x8 v1, v_int8x8 v2) { return vmul_s8(v1, v2); }
inline v_int16x4 vd_mul_s16(v_int16x4 v1, v_int16x4 v2) { return vmul_s16(v1, v2); }
inline v_int32x2 vd_mul_s32(v_int32x2 v1, v_int32x2 v2) { return vmul_s32(v1, v2); }
inline v_float32x2 vd_mul_f32(v_float32x2 v1, v_float32x2 v2) { return vmul_f32(v1, v2); }
inline v_uint8x8 vd_mul_u8(v_uint8x8 v1, v_uint8x8 v2) { return vmul_u8(v1, v2); }
inline v_uint16x4 vd_mul_u16(v_uint16x4 v1, v_uint16x4 v2) { return vmul_u16(v1, v2); }
inline v_uint32x2 vd_mul_u32(v_uint32x2 v1, v_uint32x2 v2) { return vmul_u32(v1, v2); }

// 16bytes(128bits) part
inline v_int8x16 vq_mul_s8(v_int8x16 v1, v_int8x16 v2) { return vmulq_s8(v1, v2); }
inline v_int16x8 vq_mul_s16(v_int16x8 v1, v_int16x8 v2) { return vmulq_s16(v1, v2); }
inline v_int32x4 vq_mul_s32(v_int32x4 v1, v_int32x4 v2) { return vmulq_s32(v1, v2); }
inline v_float32x4 vq_mul_f32(v_float32x4 v1, v_float32x4 v2) { return vmulq_f32(v1, v2); }
inline v_uint8x16 vq_mul_u8 (v_uint8x16 v1, v_uint8x16 v2) { return vmulq_u8(v1, v2); }
inline v_uint16x8 vq_mul_u16(v_uint16x8 v1, v_uint16x8 v2) { return vmulq_u16(v1, v2); }
inline v_uint32x4 vq_mul_u32(v_uint32x4 v1, v_uint32x4 v2) { return vmulq_u32(v1, v2); }

//----------------------------------------------------------------------
// sub(v1, v2), return v1[i] - v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_sub_s8(v_int8x8 v1, v_int8x8 v2) { return vsub_s8(v1, v2); }
inline v_int16x4 vd_sub_s16(v_int16x4 v1, v_int16x4 v2) { return vsub_s16(v1, v2); }
inline v_int32x2 vd_sub_s32(v_int32x2 v1, v_int32x2 v2) { return vsub_s32(v1, v2); }
inline v_int64x1 vd_sub_s64(v_int64x1 v1, v_int64x1 v2) { return vsub_s64(v1, v2); }
inline v_float32x2 vd_sub_f32(v_float32x2 v1, v_float32x2 v2) { return vsub_f32(v1, v2); }
inline v_uint8x8 vd_sub_u8(v_uint8x8 v1, v_uint8x8 v2) { return vsub_u8(v1, v2); }
inline v_uint16x4 vd_sub_u16(v_uint16x4 v1, v_uint16x4 v2) { return vsub_u16(v1, v2); }
inline v_uint32x2 vd_sub_u32(v_uint32x2 v1, v_uint32x2 v2) { return vsub_u32(v1, v2); }
inline v_uint64x1 vd_sub_u64(v_uint64x1 v1, v_uint64x1 v2) { return vsub_u64(v1, v2); }

// 16bytes(128bits) part
inline v_int8x16 vq_sub_s8(v_int8x16 v1, v_int8x16 v2) { return vsubq_s8(v1, v2); }
inline v_int16x8 vq_sub_s16(v_int16x8 v1, v_int16x8 v2) { return vsubq_s16(v1, v2); }
inline v_int32x4 vq_sub_s32(v_int32x4 v1, v_int32x4 v2) { return vsubq_s32(v1, v2); }
inline v_int64x2 vq_sub_s64(v_int64x2 v1, v_int64x2 v2) { return vsubq_s64(v1, v2); }
inline v_float32x4 vq_sub_f32(v_float32x4 v1, v_float32x4 v2) { return vsubq_f32(v1, v2); }
inline v_uint8x16 vq_sub_u8 (v_uint8x16 v1, v_uint8x16 v2) { return vsubq_u8(v1, v2); }
inline v_uint16x8 vq_sub_u16(v_uint16x8 v1, v_uint16x8 v2) { return vsubq_u16(v1, v2); }
inline v_uint32x4 vq_sub_u32(v_uint32x4 v1, v_uint32x4 v2) { return vsubq_u32(v1, v2); }
inline v_uint64x2 vq_sub_u64(v_uint64x2 v1, v_uint64x2 v2) { return vsubq_u64(v1, v2); }

//----------------------------------------------------------------------
// fmadd(v1, v2, v3), return (v1[i]*v2[i])+v3[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_fmadd_s8(v_int8x8 v1, v_int8x8 v2, v_int8x8 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_s8(v3, v1, v2);
#else
    return vadd_s8(vmul_s8(v1, v2), v3);
#endif
}

inline v_int16x4 vd_fmadd_s16(v_int16x4 v1, v_int16x4 v2, v_int16x4 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_s16(v3, v1, v2);
#else
    return vadd_s16(vmul_s16(v1, v2), v3);
#endif
}

inline v_int32x2 vd_fmadd_s32(v_int32x2 v1, v_int32x2 v2, v_int32x2 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_s32(v3, v1, v2);
#else
    return vadd_s32(vmul_s32(v1, v2), v3);
#endif
}

inline v_float32x2 vd_fmadd_f32(v_float32x2 v1, v_float32x2 v2, v_float32x2 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_f32(v3, v1, v2);
#else
    return vadd_f32(vmul_f32(v1, v2), v3);
#endif
}

inline v_uint8x8 vd_fmadd_u8(v_uint8x8 v1, v_uint8x8 v2, v_uint8x8 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_u8(v3, v1, v2);
#else
    return vadd_u8(vmul_u8(v1, v2), v3);
#endif
}

inline v_uint16x4 vd_fmadd_u16(v_uint16x4 v1, v_uint16x4 v2, v_uint16x4 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_u16(v3, v1, v2);
#else
    return vadd_u16(vmul_u16(v1, v2), v3);
#endif
}

inline v_uint32x2 vd_fmadd_u32(v_uint32x2 v1, v_uint32x2 v2, v_uint32x2 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmla_u32(v3, v1, v2);
#else
    return vadd_u32(vmul_u32(v1, v2), v3);
#endif
}

inline v_int16x8 vq_fmadd_s16(v_int16x8 v1, v_int16x8 v2, v_int16x8 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_s16(v3, v1, v2);
#else
    return vaddq_s16(vmulq_s16(v1, v2), v3);
#endif
}

inline v_int32x4 vq_fmadd_s32(v_int32x4 v1, v_int32x4 v2, v_int32x4 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_s32(v3, v1, v2);
#else
    return vaddq_s32(vmulq_s32(v1, v2), v3);
#endif
}

inline v_float32x4 vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_f32(v3, v1, v2);
#else
    return vaddq_f32(vmulq_f32(v1, v2), v3);
#endif
}

inline v_uint8x16 vq_fmadd_u8(v_uint8x16 v1, v_uint8x16 v2, v_uint8x16 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_u8(v3, v1, v2);
#else
    return vaddq_u8(vmulq_u8(v1, v2), v3);
#endif
}

inline v_uint16x8 vq_fmadd_u16(v_uint16x8 v1, v_uint16x8 v2, v_uint16x8 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_u16(v3, v1, v2);
#else
    return vaddq_u16(vmulq_u16(v1, v2), v3);
#endif
}

inline v_uint32x4 vq_fmadd_u32(v_uint32x4 v1, v_uint32x4 v2, v_uint32x4 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_u32(v3, v1, v2);
#else
    return vaddq_u32(vmulq_u32(v1, v2), v3);
#endif
}

//----------------------------------------------------------------------
// cmplt(v1, v2), return (v1[i] < v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmplt_s8(v_int8x8 v1, v_int8x8 v2) { return vclt_s8(v1, v2); }
inline v_uint16x4 vd_cmplt_s16(v_int16x4 v1, v_int16x4 v2) { return vclt_s16(v1, v2); }
inline v_uint32x2 vd_cmplt_s32(v_int32x2 v1, v_int32x2 v2) { return vclt_s32(v1, v2); }
inline v_uint64x1 vd_cmplt_s64(v_int64x1 v1, v_int64x1 v2) { return vclt_s64(v1, v2); }
inline v_uint32x2 vd_cmplt_f32(v_float32x2 v1, v_float32x2 v2) { return vclt_f32(v1, v2); }
inline v_uint8x8 vd_cmplt_u8(v_uint8x8 v1, v_uint8x8 v2) { return vclt_u8(v1, v2); }
inline v_uint16x4 vd_cmplt_u16(v_uint16x4 v1, v_uint16x4 v2) { return vclt_u16(v1, v2); }
inline v_uint32x2 vd_cmplt_u32(v_uint32x2 v1, v_uint32x2 v2) { return vclt_u32(v1, v2); }
inline v_uint64x1 vd_cmplt_u64(v_uint64x1 v1, v_uint64x1 v2) { return vclt_u64(v1, v2); }

// 16bytes(128bits) part
inline v_uint8x16 vq_cmplt_s8(v_int8x16 v1, v_int8x16 v2) { return vcltq_s8(v1, v2); }
inline v_uint16x8 vq_cmplt_s16(v_int16x8 v1, v_int16x8 v2) { return vcltq_s16(v1, v2); }
inline v_uint32x4 vq_cmplt_s32(v_int32x4 v1, v_int32x4 v2) { return vcltq_s32(v1, v2); }
inline v_uint64x2 vq_cmplt_s64(v_int64x2 v1, v_int64x2 v2) { return vcltq_s64(v1, v2); }
inline v_uint32x4 vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2) { return vcltq_f32(v1, v2); }
inline v_uint8x16 vq_cmplt_u8(v_uint8x16 v1, v_uint8x16 v2) { return vcltq_u8(v1, v2); }
inline v_uint16x8 vq_cmplt_u16(v_uint16x8 v1, v_uint16x8 v2) { return vcltq_u16(v1, v2); }
inline v_uint32x4 vq_cmplt_u32(v_uint32x4 v1, v_uint32x4 v2) { return vcltq_u32(v1, v2); }
inline v_uint64x2 vq_cmplt_u64(v_uint64x2 v1, v_uint64x2 v2) { return vcltq_u64(v1, v2); }

//----------------------------------------------------------------------
// cmple(v1, v2), return (v1[i] <= v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmple_s8(v_int8x8 v1, v_int8x8 v2) { return vcle_s8(v1, v2); }
inline v_uint16x4 vd_cmple_s16(v_int16x4 v1, v_int16x4 v2) { return vcle_s16(v1, v2); }
inline v_uint32x2 vd_cmple_s32(v_int32x2 v1, v_int32x2 v2) { return vcle_s32(v1, v2); }
inline v_uint64x1 vd_cmple_s64(v_int64x1 v1, v_int64x1 v2) { return vcle_s64(v1, v2); }
inline v_uint32x2 vd_cmple_f32(v_float32x2 v1, v_float32x2 v2) { return vcle_f32(v1, v2); }
inline v_uint8x8 vd_cmple_u8(v_uint8x8 v1, v_uint8x8 v2) { return vcle_u8(v1, v2); }
inline v_uint16x4 vd_cmple_u16(v_uint16x4 v1, v_uint16x4 v2) { return vcle_u16(v1, v2); }
inline v_uint32x2 vd_cmple_u32(v_uint32x2 v1, v_uint32x2 v2) { return vcle_u32(v1, v2); }
inline v_uint64x1 vd_cmple_u64(v_uint64x1 v1, v_uint64x1 v2) { return vcle_u64(v1, v2); }

// 16bytes(128bits) part
inline v_uint8x16 vq_cmple_s8(v_int8x16 v1, v_int8x16 v2) { return vcleq_s8(v1, v2); }
inline v_uint16x8 vq_cmple_s16(v_int16x8 v1, v_int16x8 v2) { return vcleq_s16(v1, v2); }
inline v_uint32x4 vq_cmple_s32(v_int32x4 v1, v_int32x4 v2) { return vcleq_s32(v1, v2); }
inline v_uint64x2 vq_cmple_s64(v_int64x2 v1, v_int64x2 v2) { return vcleq_s64(v1, v2); }
inline v_uint32x4 vq_cmple_f32(v_float32x4 v1, v_float32x4 v2) { return vcleq_s32(v1, v2); }
inline v_uint8x16 vq_cmple_u8(v_uint8x16 v1, v_uint8x16 v2) { return vcleq_u8(v1, v2); }
inline v_uint16x8 vq_cmple_u16(v_uint16x8 v1, v_uint16x8 v2) { return vcleq_u16(v1, v2); }
inline v_uint32x4 vq_cmple_u32(v_uint32x4 v1, v_uint32x4 v2) { return vcleq_u32(v1, v2); }
inline v_uint64x2 vq_cmple_u64(v_uint64x2 v1, v_uint64x2 v2) { return vcleq_u64(v1, v2); }

//----------------------------------------------------------------------
// cmpgt(v1, v2), return (v1[i] > v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmpgt_s8(v_int8x8 v1, v_int8x8 v2) { return vcgt_s8(v1, v2);}
inline v_uint16x4 vd_cmpgt_s16(v_int16x4 v1, v_int16x4 v2) { return vcgt_s16(v1, v2);}
inline v_uint32x2 vd_cmpgt_s32(v_int32x2 v1, v_int32x2 v2) { return vcgt_s32(v1, v2);}
inline v_uint64x1 vd_cmpgt_s64(v_int64x1 v1, v_int64x1 v2) { return vcgt_s64(v1, v2);}
inline v_uint32x2 vd_cmpgt_f32(v_float32x2 v1, v_float32x2 v2) { return vcgt_f32(v1, v2);}
inline v_uint8x8 vd_cmpgt_u8(v_uint8x8 v1, v_uint8x8 v2) { return vcgt_u8(v1, v2);}
inline v_uint16x4 vd_cmpgt_u16(v_uint16x4 v1, v_uint16x4 v2) { return vcgt_u16(v1, v2);}
inline v_uint32x2 vd_cmpgt_u32(v_uint32x2 v1, v_uint32x2 v2) { return vcgt_u32(v1, v2);}
inline v_uint64x1 vd_cmpgt_u64(v_uint64x1 v1, v_uint64x1 v2) { return vcgt_u64(v1, v2);}

// 16bytes(128bits) part
inline v_uint8x16 vq_cmpgt_s8(v_int8x16 v1, v_int8x16 v2) { return vcgtq_s8(v1, v2);}
inline v_uint16x8 vq_cmpgt_s16(v_int16x8 v1, v_int16x8 v2) { return vcgtq_s16(v1, v2);}
inline v_uint32x4 vq_cmpgt_s32(v_int32x4 v1, v_int32x4 v2) { return vcgtq_s32(v1, v2);}
inline v_uint64x2 vq_cmpgt_s64(v_int64x2 v1, v_int64x2 v2) { return vcgtq_s64(v1, v2);}
inline v_uint32x4 vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2) { return vcgtq_f32(v1, v2);}
inline v_uint8x16 vq_cmpgt_u8(v_uint8x16 v1, v_uint8x16 v2) { return vcgtq_u8(v1, v2);}
inline v_uint16x8 vq_cmpgt_u16(v_uint16x8 v1, v_uint16x8 v2) { return vcgtq_u16(v1, v2);}
inline v_uint32x4 vq_cmpgt_u32(v_uint32x4 v1, v_uint32x4 v2) { return vcgtq_u32(v1, v2);}
inline v_uint64x2 vq_cmpgt_u64(v_uint64x2 v1, v_uint64x2 v2) { return vcgtq_u64(v1, v2);}

//----------------------------------------------------------------------
// cmpge(v1, v2), return (v1[i] >= v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmpge_s8(v_int8x8 v1, v_int8x8 v2) { return vcge_s8(v1, v2); }
inline v_uint16x4 vd_cmpge_s16(v_int16x4 v1, v_int16x4 v2) { return vcge_s16(v1, v2); }
inline v_uint32x2 vd_cmpge_s32(v_int32x2 v1, v_int32x2 v2) { return vcge_s32(v1, v2); }
inline v_uint64x1 vd_cmpge_s64(v_int64x1 v1, v_int64x1 v2) { return vcge_s64(v1, v2); }
inline v_uint32x2 vd_cmpge_f32(v_float32x2 v1, v_float32x2 v2) { return vcge_f32(v1, v2); }
inline v_uint8x8 vd_cmpge_u8(v_uint8x8 v1, v_uint8x8 v2) { return vcge_u8(v1, v2); }
inline v_uint16x4 vd_cmpge_u16(v_uint16x4 v1, v_uint16x4 v2) { return vcge_u16(v1, v2); }
inline v_uint32x2 vd_cmpge_u32(v_uint32x2 v1, v_uint32x2 v2) { return vcge_u32(v1, v2); }
inline v_uint64x1 vd_cmpge_u64(v_uint64x1 v1, v_uint64x1 v2) { return vcge_u64(v1, v2); }

// 16bytes(128bits) part
inline v_uint8x16 vq_cmpge_s8(v_int8x16 v1, v_int8x16 v2) { return vcgeq_s8(v1, v2); }
inline v_uint16x8 vq_cmpge_s16(v_int16x8 v1, v_int16x8 v2) { return vcgeq_s16(v1, v2); }
inline v_uint32x4 vq_cmpge_s32(v_int32x4 v1, v_int32x4 v2) { return vcgeq_s32(v1, v2); }
inline v_uint64x2 vq_cmpge_s64(v_int64x2 v1, v_int64x2 v2) { return vcgeq_s64(v1, v2); }
inline v_uint32x4 vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2) { return vcgeq_f32(v1, v2); }
inline v_uint8x16 vq_cmpge_u8(v_uint8x16 v1, v_uint8x16 v2) { return vcgeq_u8(v1, v2); }
inline v_uint16x8 vq_cmpge_u16(v_uint16x8 v1, v_uint16x8 v2) { return vcgeq_u16(v1, v2); }
inline v_uint32x4 vq_cmpge_u32(v_uint32x4 v1, v_uint32x4 v2) { return vcgeq_u32(v1, v2); }
inline v_uint64x2 vq_cmpge_u64(v_uint64x2 v1, v_uint64x2 v2) { return vcgeq_u64(v1, v2); }

//----------------------------------------------------------------------
// bitselect(mask, v1, v2)
// selects each bit for the destination from the first operand
// if the corresponding bit of the destination is 1,
// or from the second operand if the corresponding bit of the destination is 0.
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_bitselect_s8(v_uint8x8 mask, v_int8x8 v1, v_int8x8 v2) { return vbsl_s8(mask, v1, v2); }
inline v_int16x4 vd_bitselect_s16(v_uint16x4 mask, v_int16x4 v1, v_int16x4 v2) { return vbsl_s16(mask, v1, v2); }
inline v_int32x2 vd_bitselect_s32(v_uint32x2 mask, v_int32x2 v1, v_int32x2 v2) { return vbsl_s32(mask, v1, v2); }
inline v_int64x1 vd_bitselect_s64(v_uint64x1 mask, v_int64x1 v1, v_int64x1 v2) { return vbsl_s64(mask, v1, v2); }
inline v_float32x2 vd_bitselect_f32(v_uint32x2 mask, v_float32x2 v1, v_float32x2 v2) { return vbsl_f32(mask, v1, v2); }
inline v_uint8x8 vd_bitselect_u8(v_uint8x8 mask, v_uint8x8 v1, v_uint8x8 v2) { return vbsl_u8(mask, v1, v2); }
inline v_uint16x4 vd_bitselect_u16(v_uint16x4 mask, v_uint16x4 v1, v_uint16x4 v2) { return vbsl_u16(mask, v1, v2); }
inline v_uint32x2 vd_bitselect_u32(v_uint32x2 mask, v_uint32x2 v1, v_uint32x2 v2) { return vbsl_u32(mask, v1, v2); }
inline v_uint64x1 vd_bitselect_u64(v_uint64x1 mask, v_uint64x1 v1, v_uint64x1 v2) { return vbsl_u64(mask, v1, v2); }

// 16bytes(128bits) part
inline v_int8x16 vq_bitselect_s8(v_uint8x16 mask, v_int8x16 v1, v_int8x16 v2) { return vbslq_s8(mask, v1, v2); }
inline v_int16x8 vq_bitselect_s16(v_uint16x8 mask, v_int16x8 v1, v_int16x8 v2) { return vbslq_s16(mask, v1, v2); }
inline v_int32x4 vq_bitselect_s32(v_uint32x4 mask, v_int32x4 v1, v_int32x4 v2) { return vbslq_s32(mask, v1, v2); }
inline v_int64x2 vq_bitselect_s64(v_uint64x2 mask, v_int64x2 v1, v_int64x2 v2) { return vbslq_s64(mask, v1, v2); }
inline v_float32x4 vq_bitselect_f32(v_uint32x4 mask, v_float32x4 v1, v_float32x4 v2) { return vbslq_f32(mask, v1, v2); }
inline v_uint8x16 vq_bitselect_u8(v_uint8x16 mask, v_uint8x16 v1, v_uint8x16 v2) { return vbslq_u8(mask, v1, v2); }
inline v_uint16x8 vq_bitselect_u16(v_uint16x8 mask, v_uint16x8 v1, v_uint16x8 v2) { return vbslq_u16(mask, v1, v2); }
inline v_uint32x4 vq_bitselect_u32(v_uint32x4 mask, v_uint32x4 v1, v_uint32x4 v2) { return vbslq_u32(mask, v1, v2); }
inline v_uint64x2 vq_bitselect_u64(v_uint64x2 mask, v_uint64x2 v1, v_uint64x2 v2) { return vbslq_u64(mask, v1, v2); }


#endif // PIXEL_SIMD_NEON_H
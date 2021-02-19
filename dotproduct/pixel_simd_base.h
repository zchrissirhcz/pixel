#ifndef PIXEL_SIMD_BASE_H
#define PIXEL_SIMD_BASE_H

//----------------------------------------------------------------------
// => including header file
//----------------------------------------------------------------------
// len   abbv
// 64bit  d  double     (2x)
// 128bit q  quadruple  (4x)
// 256bit o  octuple    (8x)
// 512bit s  sexdecuple (16x)
// auto   x  eXtensible (2x/4x/8x/16x)

#if (defined(_MSC_VER) && 0)
#define PIXEL_SIMD_SSE
#endif

#if ((defined(__ARM_NEON__) || defined(__ARM_NEON)))
#define PIXEL_SIMD_NEON
#endif

#if !defined(PIXEL_SIMD_SSE) && !(defined(PIXEL_SIMD_NEON))
#define PIXEL_SIMD_SIMU
#endif

// SSE determination
#ifdef PIXEL_SIMD_SSE
#include <stdint.h>
#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif
#elif defined(PIXEL_SIMD_NEON)
    #include <arm_neon.h>
#else
    #include <stdint.h>
#endif

#include <stdlib.h>
#include <string.h>

//#define PIXEL_VD_BYTES 8
#define PIXEL_VQ_BYTES 16
//#define PIXEL_VO_BYTES 32
//#define PIXEL_VS_BYTES 64

//----------------------------------------------------------------------
// => type definitions
//----------------------------------------------------------------------
#if defined(PIXEL_SIMD_NEON)

// 8 byte align required. D reqisters (64bit)
typedef int8x8_t    v_int8x8;
typedef int16x4_t   v_int16x4;
typedef int32x2_t   v_int32x2;
typedef int64x1_t   v_int64x1;
typedef float32x2_t v_float32x2;
typedef uint8x8_t   v_uint8x8;
typedef uint16x4_t  v_uint16x4;
typedef uint32x2_t  v_uint32x2;
typedef uint64x1_t  v_uint64x1;
// typedef poly8x8_t   v_poly8x8;
// typedef poly16x4_t  v_poly16x4;

// 16 byte align required
typedef int8x16_t   v_int8x16;
typedef int16x8_t   v_int16x8;
typedef int32x4_t   v_int32x4;
typedef int64x2_t   v_int64x2;
typedef float32x4_t v_float32x4;
typedef uint8x16_t  v_uint8x16;
typedef uint16x8_t  v_uint16x8;
typedef uint32x4_t  v_uint32x4;
typedef uint64x2_t  v_uint64x2;
// typedef poly8x16_t  v_poly8x16;
// typedef poly16x8_t  v_poly16x8;

#else

// (mock of) 8 byte align required. 64bit registers
typedef struct v_int8x8     { int8_t data_[8];  } v_int8x8;
typedef struct v_int16x4    { int16_t data_[4]; } v_int16x4;
typedef struct v_int32x2    { int32_t data_[2]; } v_int32x2;
typedef struct v_int64x1    { int64_t data_[1]; } v_int64x1;
typedef struct v_float32x2  { float data_[2];   } v_float32x2;
typedef struct v_uint8x8    { uint8_t data_[8]; } v_uint8x8;
typedef struct v_uint16x4   { uint16_t data_[4];} v_uint16x4;
typedef struct v_uint32x2   { uint32_t data_[2];} v_uint32x2;
typedef struct v_uint64x1   { uint64_t data_[1];} v_uint64x1;

// (mock of) 16 byte align required. 128bit registers
typedef struct v_int8x16    { int8_t data_[16]; } v_int8x16;
typedef struct v_int16x8    { int16_t data_[8]; } v_int16x8;
typedef struct v_int32x4    { int32_t data_[4]; } v_int32x4;
typedef struct v_int64x2    { int64_t data_[2]; } v_int64x2;
typedef struct v_float32x4  { float data_[4];   } v_float32x4;
typedef struct v_uint8x16   { uint8_t data_[16];} v_uint8x16;
typedef struct v_uint16x8   { uint16_t data_[8];} v_uint16x8;
typedef struct v_uint32x4   { uint32_t data_[4];} v_uint32x4;
typedef struct v_uint64x2   { uint64_t data_[2];} v_uint64x2;

#endif // PIXEL_SIMD_NEON

//----------------------------------------------------------------------
// => function declarations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// assign all element to zero
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_setzero_s8();
static inline v_int16x4     vd_setzero_s16();
static inline v_int32x2     vd_setzero_s32();
static inline v_int64x1     vd_setzero_s64();
static inline v_float32x2   vd_setzero_f32();
static inline v_uint8x8     vd_setzero_u8();
static inline v_uint16x4    vd_setzero_u16();
static inline v_uint32x2    vd_setzero_u32();
static inline v_uint64x1    vd_setzero_u64();

// 16bytes(128bits) part
static inline v_int8x16     vq_setzero_s8();
static inline v_int16x8     vq_setzero_s16();
static inline v_int32x4     vq_setzero_s32();
static inline v_int64x2     vq_setzero_s64();
static inline v_float32x4   vq_setzero_f32();
static inline v_uint8x16    vq_setzero_u8();
static inline v_uint16x8    vq_setzero_u16();
static inline v_uint32x4    vq_setzero_u32();
static inline v_uint64x2    vq_setzero_u64();

//----------------------------------------------------------------------
// copy data, memory => register
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_load_s8 (const int8_t*   data);
static inline v_int16x4     vd_load_s16(const int16_t*  data);
static inline v_int32x2     vd_load_s32(const int32_t*  data);
static inline v_int64x1     vd_load_s64(const int64_t*  data);
static inline v_float32x2   vd_load_f32(const float*    data);
static inline v_uint8x8     vd_load_u8 (const uint8_t*  data);
static inline v_uint16x4    vd_load_u16(const uint16_t* data);
static inline v_uint32x2    vd_load_u32(const uint32_t* data);
static inline v_uint64x1    vd_load_u64(const uint64_t* data);

// 16bytes(128bits) part
static inline v_int8x16     vq_load_s8 (const int8_t*   data);
static inline v_int16x8     vq_load_s16(const int16_t*  data);
static inline v_int32x4     vq_load_s32(const int32_t*  data);
static inline v_int64x2     vq_load_s64(const int64_t*  data);
static inline v_float32x4   vq_load_f32(const float*    data);
static inline v_uint8x16    vq_load_u8 (const uint8_t*  data);
static inline v_uint16x8    vq_load_u16(const uint16_t* data);
static inline v_uint32x4    vq_load_u32(const uint32_t* data);
static inline v_uint64x2    vq_load_u64(const uint64_t* data);

//----------------------------------------------------------------------
// copy-data, register => memory
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline void  vd_store_s8 (int8_t*   data,  v_int8x8    v);
static inline void  vd_store_s16(int16_t*  data,  v_int16x4   v);
static inline void  vd_store_s32(int32_t*  data,  v_int32x2   v);
static inline void  vd_store_s64(int64_t*  data,  v_int64x1   v);
static inline void  vd_store_f32(float*    data,  v_float32x2 v);
static inline void  vd_store_u8 (uint8_t*  data,  v_uint8x8   v);
static inline void  vd_store_u16(uint16_t* data,  v_uint16x4  v);
static inline void  vd_store_u32(uint32_t* data,  v_uint32x2  v);
static inline void  vd_store_u64(uint64_t* data,  v_uint64x1  v);

// 16bytes(128bits) part
static inline void  vq_store_s8 (int8_t*    data, v_int8x16   v);
static inline void  vq_store_s16(int16_t*    data, v_int16x8   v);
static inline void  vq_store_s32(int32_t*    data, v_int32x4   v);
static inline void  vq_store_s64(int64_t*    data, v_int64x2   v);
static inline void  vq_store_f32(float*     data, v_float32x4 v);
static inline void  vq_store_u8 (uint8_t*   data, v_uint8x16  v);
static inline void  vq_store_u16(uint16_t*   data, v_uint16x8  v);
static inline void  vq_store_u32(uint32_t*   data, v_uint32x4  v);
static inline void  vq_store_u64(uint64_t*   data, v_uint64x2  v);

//----------------------------------------------------------------------
// add(v1, v2), return v1[i] + v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_add_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_int16x4     vd_add_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_int32x2     vd_add_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_int64x1     vd_add_s64(v_int64x1 v1, v_int64x1 v2);
static inline v_float32x2   vd_add_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_add_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_add_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_add_u32(v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_add_u64(v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_int8x16     vq_add_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_int16x8     vq_add_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_int32x4     vq_add_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_int64x2     vq_add_s64(v_int64x2 v1, v_int64x2 v2);
static inline v_float32x4   vq_add_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_add_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_add_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_add_u32(v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_add_u64(v_uint64x2 v1, v_uint64x2 v2);

//----------------------------------------------------------------------
// mul(v1, v2), return v1[i] * v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_mul_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_int16x4     vd_mul_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_int32x2     vd_mul_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_float32x2   vd_mul_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_mul_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_mul_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_mul_u32(v_uint32x2 v1, v_uint32x2 v2);

// 16bytes(128bits) part
static inline v_int8x16     vq_mul_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_int16x8     vq_mul_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_int32x4     vq_mul_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_float32x4   vq_mul_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_mul_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_mul_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_mul_u32(v_uint32x4 v1, v_uint32x4 v2);

//----------------------------------------------------------------------
// sub(v1, v2), return v1[i] - v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_sub_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_int16x4     vd_sub_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_int32x2     vd_sub_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_int64x1     vd_sub_s64(v_int64x1 v1, v_int64x1 v2);
static inline v_float32x2   vd_sub_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_sub_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_sub_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_sub_u32(v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_sub_u64(v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_int8x16     vq_sub_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_int16x8     vq_sub_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_int32x4     vq_sub_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_int64x2     vq_sub_s64(v_int64x2 v1, v_int64x2 v2);
static inline v_float32x4   vq_sub_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_sub_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_sub_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_sub_u32(v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_sub_u64(v_uint64x2 v1, v_uint64x2 v2);

//----------------------------------------------------------------------
// fmadd(v1, v2, v3), return (v1[i]*v2[i])+v3[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_fmadd_s8 (v_int8x8 v1, v_int8x8 v2, v_int8x8 v3);
static inline v_int16x4     vd_fmadd_s16(v_int16x4 v1, v_int16x4 v2, v_int16x4 v3);
static inline v_int32x2     vd_fmadd_s32(v_int32x2 v1, v_int32x2 v2, v_int32x2 v3);
static inline v_int64x1     vd_fmadd_s64(v_int64x1 v1, v_int64x1 v2, v_int64x1 v3);
static inline v_float32x2   vd_fmadd_f32(v_float32x2 v1, v_float32x2 v2, v_float32x2 v3);
static inline v_uint8x8     vd_fmadd_u8 (v_uint8x8 v1, v_uint8x8 v2, v_uint8x8 v3);
static inline v_uint16x4    vd_fmadd_u16(v_uint16x4 v1, v_uint16x4 v2, v_uint16x4 v3);
static inline v_uint32x2    vd_fmadd_u32(v_uint32x2 v1, v_uint32x2 v2, v_uint32x2 v3);
static inline v_uint64x1    vd_fmadd_u64(v_uint64x1 v1, v_uint64x1 v2, v_uint64x1 v3);

// 16bytes(128bits) part
static inline v_int8x16     vq_fmadd_s8 (v_int8x16 v1, v_int8x16 v2, v_int8x16 v3);
static inline v_int16x8     vq_fmadd_s16(v_int16x8 v1, v_int16x8 v2, v_int16x8 v3);
static inline v_int32x4     vq_fmadd_s32(v_int32x4 v1, v_int32x4 v2, v_int32x4 v3);
static inline v_float32x4   vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3);
static inline v_uint8x16    vq_fmadd_u8 (v_uint8x16 v1, v_uint8x16 v2, v_uint8x16 v3);
static inline v_uint16x8    vq_fmadd_u16(v_uint16x8 v1, v_uint16x8 v2, v_uint16x8 v3);
static inline v_uint32x4    vq_fmadd_u32(v_uint32x4 v1, v_uint32x4 v2, v_uint32x4 v3);

//----------------------------------------------------------------------
// cmplt(v1, v2), return (v1[i] < v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_uint8x8     vd_cmplt_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_uint16x4    vd_cmplt_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_uint32x2    vd_cmplt_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_uint64x1    vd_cmplt_s64(v_int64x1 v1, v_int64x1 v2);
static inline v_uint32x2    vd_cmplt_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_cmplt_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_cmplt_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_cmplt_u32(v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_cmplt_u64(v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_uint8x16    vq_cmplt_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_uint16x8    vq_cmplt_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_uint32x4    vq_cmplt_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_uint64x2    vq_cmplt_s64(v_int64x2 v1, v_int64x2 v2);
static inline v_uint32x4    vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_cmplt_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_cmplt_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_cmplt_u32(v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_cmplt_u64(v_uint64x2 v1, v_uint64x2 v2);

//----------------------------------------------------------------------
// cmple(v1, v2), return (v1[i] <= v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_uint8x8     vd_cmple_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_uint16x4    vd_cmple_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_uint32x2    vd_cmple_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_uint64x1    vd_cmple_s64(v_int64x1 v1, v_int64x1 v2);
static inline v_uint32x2    vd_cmple_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_cmple_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_cmple_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_cmple_u32(v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_cmple_u64(v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_uint8x16    vq_cmple_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_uint16x8    vq_cmple_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_uint32x4    vq_cmple_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_uint64x2    vq_cmple_s64(v_int64x2 v1, v_int64x2 v2);
static inline v_uint32x4    vq_cmple_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_cmple_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_cmple_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_cmple_u32(v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_cmple_u64(v_uint64x2 v1, v_uint64x2 v2);

//----------------------------------------------------------------------
// cmpgt(v1, v2), return (v1[i] > v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_uint8x8     vd_cmpgt_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_uint16x4    vd_cmpgt_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_uint32x2    vd_cmpgt_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_uint64x1    vd_cmpgt_s64(v_int64x1 v1, v_int64x1 v2);
static inline v_uint32x2    vd_cmpgt_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_cmpgt_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_cmpgt_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_cmpgt_u32(v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_cmpgt_u64(v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_uint8x16    vq_cmpgt_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_uint16x8    vq_cmpgt_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_uint32x4    vq_cmpgt_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_uint64x2    vq_cmpgt_s64(v_int64x2 v1, v_int64x2 v2);
static inline v_uint32x4    vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_cmpgt_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_cmpgt_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_cmpgt_u32(v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_cmpgt_u64(v_uint64x2 v1, v_uint64x2 v2);

//----------------------------------------------------------------------
// cmpge(v1, v2), return (v1[i] >= v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_uint8x8     vd_cmpge_s8 (v_int8x8 v1, v_int8x8 v2);
static inline v_uint16x4    vd_cmpge_s16(v_int16x4 v1, v_int16x4 v2);
static inline v_uint32x2    vd_cmpge_s32(v_int32x2 v1, v_int32x2 v2);
static inline v_uint64x1    vd_cmpge_s64(v_int64x1 v1, v_int64x1 v2);
static inline v_uint32x2    vd_cmpge_f32(v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_cmpge_u8 (v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_cmpge_u16(v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_cmpge_u32(v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_cmpge_u64(v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_uint8x16    vq_cmpge_s8 (v_int8x16 v1, v_int8x16 v2);
static inline v_uint16x8    vq_cmpge_s16(v_int16x8 v1, v_int16x8 v2);
static inline v_uint32x4    vq_cmpge_s32(v_int32x4 v1, v_int32x4 v2);
static inline v_uint64x2    vq_cmpge_s64(v_int64x2 v1, v_int64x2 v2);
static inline v_uint32x4    vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16    vq_cmpge_u8 (v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_cmpge_u16(v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_cmpge_u32(v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_cmpge_u64(v_uint64x2 v1, v_uint64x2 v2);

//----------------------------------------------------------------------
// bitselect(mask, v1, v2)
// selects each bit for the destination from the first operand
// if the corresponding bit of the destination is 1,
// or from the second operand if the corresponding bit of the destination is 0.
//----------------------------------------------------------------------
// 8bytes(64bits) part
static inline v_int8x8      vd_bitselect_s8(v_uint8x8 mask, v_int8x8 v1, v_int8x8 v2);
static inline v_int16x4     vd_bitselect_s16(v_uint16x4 mask, v_int16x4 v1, v_int16x4 v2);
static inline v_int32x2     vd_bitselect_s32(v_uint32x2 mask, v_int32x2 v1, v_int32x2 v2);
static inline v_int64x1     vd_bitselect_s64(v_uint64x1 mask, v_int64x1 v1, v_int64x1 v2);
static inline v_float32x2   vd_bitselect_f32(v_uint32x2 mask, v_float32x2 v1, v_float32x2 v2);
static inline v_uint8x8     vd_bitselect_u8(v_uint8x8 mask, v_uint8x8 v1, v_uint8x8 v2);
static inline v_uint16x4    vd_bitselect_u16(v_uint16x4 mask, v_uint16x4 v1, v_uint16x4 v2);
static inline v_uint32x2    vd_bitselect_u32(v_uint32x2 mask, v_uint32x2 v1, v_uint32x2 v2);
static inline v_uint64x1    vd_bitselect_u64(v_uint64x1 mask, v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
static inline v_int8x16      vq_bitselect_s8(v_uint8x16 mask, v_int8x16 v1, v_int8x16 v2);
static inline v_int16x8     vq_bitselect_s16(v_uint16x8 mask, v_int16x8 v1, v_int16x8 v2);
static inline v_int32x4     vq_bitselect_s32(v_uint32x4 mask, v_int32x4 v1, v_int32x4 v2);
static inline v_int64x2     vq_bitselect_s64(v_uint64x2 mask, v_int64x2 v1, v_int64x2 v2);
static inline v_float32x4   vq_bitselect_f32(v_uint32x4 mask, v_float32x4 v1, v_float32x4 v2);
static inline v_uint8x16     vq_bitselect_u8(v_uint8x16 mask, v_uint8x16 v1, v_uint8x16 v2);
static inline v_uint16x8    vq_bitselect_u16(v_uint16x8 mask, v_uint16x8 v1, v_uint16x8 v2);
static inline v_uint32x4    vq_bitselect_u32(v_uint32x4 mask, v_uint32x4 v1, v_uint32x4 v2);
static inline v_uint64x2    vq_bitselect_u64(v_uint64x2 mask, v_uint64x2 v1, v_uint64x2 v2);


#endif // PIXEL_SIMD_BASE_H

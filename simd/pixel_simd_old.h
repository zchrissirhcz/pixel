#ifndef PIXEL_SIMD_OLD_H
#define PIXEL_SIMD_OLD_H

// len   abbv
// 64bit  d  double     (2x)
// 128bit q  quadruple  (4x)
// 256bit o  octuple    (8x)
// 512bit s  sexdecuple (16x)
// auto   x  eXtensible (2x/4x/8x/16x)

#if _MSC_VER
//#define PIXEL_SSE
#endif

#if ((defined(__ARM_NEON__) || defined(__ARM_NEON)) && 0)
#define PIXEL_NEON
#endif

// SSE determination
#ifdef PIXEL_SSE
#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif
#elif defined(PIXEL_NEON)
    #include <arm_neon.h>
#else
    #include <stdint.h>
#endif

//----------------------------------------------------------------------
// type definitions
//----------------------------------------------------------------------
#if defined(PIXEL_NEON)

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

#elif defined(PIXEL_SSE)

// 8 byte align required. MMX(FPU) registers (64bit)
#if defined(_M_IX86) // run in 32bit, use real MMX
typedef __m64       v_int8x8;
typedef __m64       v_int16x4;
typedef __m64       v_int32x2;
typedef __m64       v_int64x1;
typedef __m64       v_float32x2;
typedef __m64       v_uint8x8;
typedef __m64       v_uint16x4;
typedef __m64       v_uint32x2;
typedef __m64       v_uint64x1;
#else // run in 64bit, use mimic MMX
typedef struct v_int8x8     { int8_t data_[8];  } v_int8x8;
typedef struct v_int16x4    { int16_t data_[4]; } v_int16x4;
typedef struct v_int32x2    { int32_t data_[2]; } v_int32x2;
typedef struct v_int64x1    { int64_t data_[1]; } v_int64x1;
typedef struct v_float32x2  { float data_[2];   } v_float32x2;
typedef struct v_uint8x8    { uint8_t data_[8]; } v_uint8x8;
typedef struct v_uint16x4   { uint16_t data_[4];} v_uint16x4;
typedef struct v_uint32x2   { uint32_t data_[2];} v_uint32x2;
typedef struct v_uint64x1   { uint64_t data_[1];} v_uint64x1;
#endif

// 16 byte align required. XMM registers. 128bit
typedef __m128i     v_int8x16;
typedef __m128i     v_int16x8;
typedef __m128i     v_int32x4;
typedef __m128i     v_int64x2;
typedef __m128      v_float32x4;
typedef __m128i     v_uint8x16;
typedef __m128i     v_uint16x8;
typedef __m128i     v_uint32x4;
typedef __m128i     v_uint64x2;

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

#endif


//----------------------------------------------------------------------
// function definitions
//----------------------------------------------------------------------

// assign all element to zero
static inline v_int8x8      vd_setzero_s8();
static inline v_int16x4     vd_setzero_s16();
static inline v_int32x2     vd_setzero_s32();
static inline v_int64x1     vd_setzero_s64();
static inline v_float32x2   vd_setzero_f32();
static inline v_uint8x8     vd_setzero_u8();
static inline v_uint16x4    vd_setzero_u16();
static inline v_uint32x2    vd_setzero_u32();
static inline v_uint64x1    vd_setzero_u64();

static inline v_int8x16     vq_setzero_s8();
static inline v_int16x8     vq_setzero_s16();
static inline v_int32x4     vq_setzero_s32();
static inline v_int64x2     vq_setzero_s64();
static inline v_float32x4   vq_setzero_f32();
static inline v_uint8x16    vq_setzero_u8();
static inline v_uint16x8    vq_setzero_u16();
static inline v_uint32x4    vq_setzero_u32();
static inline v_uint64x2    vq_setzero_u64();

// memory => register
static inline v_int8x8      vd_load_s8 (const int8_t*   ptr);
static inline v_int16x4     vd_load_s16(const int16_t*  ptr);
static inline v_int32x2     vd_load_s32(const int32_t*  ptr);
static inline v_int64x1     vd_load_s64(const int32_t*  ptr);
static inline v_float32x2   vd_load_f32(const float*    ptr);
static inline v_uint8x8     vd_load_u8 (const uint8_t*  ptr);
static inline v_uint16x4    vd_load_u16(const uint16_t* ptr);
static inline v_uint32x2    vd_load_u32(const uint32_t* ptr);
static inline v_uint64x1    vd_load_u64(const uint32_t* ptr);

static inline v_int8x16     vq_load_s8 (const int8_t*   ptr);
static inline v_int16x8     vq_load_s16(const int16_t*  ptr);
static inline v_int32x4     vq_load_s32(const int32_t*  ptr);
static inline v_int64x2     vq_load_s64(const int64_t*  ptr);
static inline v_float32x4   vq_load_f32(const float*    ptr);
static inline v_uint8x16    vq_load_u8 (const uint8_t*  ptr);
static inline v_uint16x8    vq_load_u16(const uint16_t* ptr);
static inline v_uint32x4    vq_load_u32(const uint32_t* ptr);
static inline v_uint64x2    vq_load_u64(const uint64_t* ptr);

// register => memory
static inline void  vd_store_s8 (int8_t*   arr,  v_int8x8    v);
static inline void  vd_store_s16(int16_t*  ptr,  v_int16x4   v);
static inline void  vd_store_s32(int32_t*  ptr,  v_int32x2   v);
static inline void  vd_store_s64(int64_t*  ptr,  v_int64x1   v);
static inline void  vd_store_f32(float*    ptr,  v_float32x2 v);
static inline void  vd_store_u8 (uint8_t*  arr,  v_uint8x8   v);
static inline void  vd_store_u16(uint16_t* ptr,  v_uint16x4  v);
static inline void  vd_store_u32(uint32_t* ptr,  v_uint32x2  v);
static inline void  vd_store_u64(uint64_t* ptr,  v_uint64x1  v);

static inline void  vq_store_s8 (int8_t*    arr, v_int8x16   v);
static inline void  vq_store_s16(int8_t*    arr, v_int16x8   v);
static inline void  vq_store_s32(int8_t*    arr, v_int32x4   v);
static inline void  vq_store_s64(int8_t*    arr, v_int64x2   v);
static inline void  vq_store_f32(float*     arr, v_float32x4 v);
static inline void  vq_store_u8 (uint8_t*   arr, v_uint8x16  v);
static inline void  vq_store_u16(uint8_t*   arr, v_uint16x8  v);
static inline void  vq_store_u32(uint8_t*   arr, v_uint32x4  v);
static inline void  vq_store_u64(uint8_t*   arr, v_uint64x2  v);

// return v1[i]*v2[i]
static inline v_float32x4 vq_mul_f32(v_float32x4 v1, v_float32x4 v2);

// return v1[i] + v2[i]
static inline v_float32x4 vq_add_f32(v_float32x4 v1, v_float32x4 v2);

// sigma of v[i]
static inline float vq_reduce_sum(v_float32x4 v);

// (v1*v2)+v3
static inline v_float32x4 vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3);

// compare, less than
static inline v_uint32x4 vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2);

// compare, less than or equal
static inline v_uint32x4 vq_cmple_f32(v_float32x4 v1, v_float32x4 v2);

// compare, greater than
static inline v_uint32x4 vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2);

// compare, greater than or equal
static inline v_uint32x4 vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2);

// bitwise select
static inline v_float32x4 vq_bitselect_f32(v_uint32x4 mask, v_float32x4 v1, v_float32x4 v2);

//----------------------------------------------------------------------
// function implementations
//----------------------------------------------------------------------
//----- NEON
#if defined(PIXEL_NEON)
inline v_float32x4 vq_setzero_f32() {
    return vdupq_n_f32(0);
}

inline v_float32x4 vq_load_f32(const float* data) {
    return vld1q_f32(data);
}

inline v_float32x4 vq_mul_f32(v_float32x4 v1, v_float32x4 v2) {
    return vmulq_f32(v1, v2);
}

inline v_float32x4 vq_add_f32(v_float32x4 v1, v_float32x4 v2) {
    return vaddq_f32(v1, v2);
}

inline void vq_store_f32(float* arr, v_float32x4 v) {
    vst1q_f32(arr, v);
}

inline float vq_reduce_sum(v_float32x4 v) {
    float arr[4];
    vst1q_f32(arr, v);
    return arr[0]+arr[1]+arr[2]+arr[3];
}

inline v_float32x4 vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3) {
#ifdef __ARM_FEATURE_FMA
    return vmlaq_f32(v3, v1, v2);
#else
    return vaddq_f32(vmulq_f32(v1, v2), v3);
#endif
}

inline v_uint32x4 vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2) {
    return vcltq_f32(v1, v2);
}

inline v_uint32x4 vq_cmple_f32(v_float32x4 v1, v_float32x4 v2) {
    return vcleq_f32(v1, v2);
}

inline v_uint32x4 vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2) {
    return vcgtq_f32(v1, v2);
}

inline v_uint32x4 vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2) {
    return vcgeq_f32(v1, v2);
}

inline v_float32x4 vq_bitselect_f32(uint32x4_t mask, float32x4_t v1, float32x4_t v2) {
    return vbslq_f32(mask, v1, v2);
}

//----- SSE 
#elif defined(PIXEL_SSE)
inline v_float32x4 vq_setzero_f32() {
    return _mm_setzero_ps();
}

#include <mmintrin.h>



inline v_int8x8 vd_setzero_s8() {
#if defined(_M_IX86) // 32bit
    return _mm_setzero_si64();
#else // 64bit
    v_int8x8 v;
    v.data_[0] = 0;
#endif
}



inline v_int16x4     vd_setzero_s16();
inline v_int32x2     vd_setzero_s32();
inline v_int64x1     vd_setzero_s64();
inline v_float32x2   vd_setzero_f32();
inline v_uint8x8     vd_setzero_u8();
inline v_uint16x4    vd_setzero_u16();
inline v_uint32x2    vd_setzero_u32();
inline v_uint64x1    vd_setzero_u64();

inline v_int8x16     vq_setzero_s8();
inline v_int16x8     vq_setzero_s16();
inline v_int32x4     vq_setzero_s32();
inline v_int64x2     vq_setzero_s64();
inline v_float32x4   vq_setzero_f32();
inline v_uint8x16    vq_setzero_u8();
inline v_uint16x8    vq_setzero_u16();
inline v_uint32x4    vq_setzero_u32();
inline v_uint64x2    vq_setzero_u64();




inline v_float32x4 vq_load_f32(const float* data) {
    return _mm_loadu_ps(data); // use un-aligned version for debugging case
}

inline v_float32x4 vq_mul_f32(v_float32x4 v1, v_float32x4 v2){
    return _mm_mul_ps(v1, v2);
}

inline v_float32x4 vq_add_f32(v_float32x4 v1, v_float32x4 v2) {
    return _mm_add_ps(v1, v2);
}

inline void vq_store_f32(float* arr, v_float32x4 v) {
    _mm_store_ps(arr, v);
}

inline float vq_reduce_sum(v_float32x4 v) {
    float arr[4];
    _mm_store_ps(arr, v);
    return arr[0]+arr[1]+arr[2]+arr[3];
}

inline v_float32x4 vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3) {
#ifdef __FMA__
    return _mm_fmadd_ps(v1, v2, v3);
#else
    return _mm_add_ps(_mm_mul_ps(v1, v2), v3);
#endif
}

inline v_uint32x4 vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2) {
    return _mm_castps_si128(_mm_cmplt_ps(v1, v2));
}

inline v_uint32x4 vq_cmple_f32(v_float32x4 v1, v_float32x4 v2) {
    return _mm_castps_si128(_mm_cmple_ps(v1, v2));
}

inline v_uint32x4 vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2) {
    return _mm_castps_si128(_mm_cmpgt_ps(v1, v2));
}

inline v_uint32x4 vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2) {
    return _mm_castps_si128(_mm_cmpge_ps(v1, v2));
}

inline v_float32x4 vq_bitselect_f32(v_uint32x4 mask, v_float32x4 v1, v_float32x4 v2) {
    // TODO: use _mm_and_si128, _mm_andnot_si128, _mm_castps_si128, etc
    float v1_arr[4];
    vq_store_f32(v1_arr, v1);
    float v2_arr[4];
    vq_store_f32(v2_arr, v2);

}

//------ NO SIMD
#else

inline v_float32x4 vq_setzero_f32() {
    v_float32x4 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    return v;
}

inline v_float32x4 vq_load_f32(const float* data) {
    v_float32x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_float32x4 vq_mul_f32(v_float32x4 v1, v_float32x4 v2) {
    v_float32x4 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    return v;
}

inline v_float32x4 vq_add_f32(v_float32x4 v1, v_float32x4 v2) {
    v_float32x4 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    return v;
}

inline void vq_store_f32(float* arr, v_float32x4 v) {
    arr[0] = v.data_[0];
    arr[1] = v.data_[1];
    arr[2] = v.data_[2];
    arr[3] = v.data_[3];
}

inline float vq_reduce_sum(v_float32x4 v) {
    return v.data_[0]+v.data_[1]+v.data_[2]+v.data_[3];
}

inline v_float32x4 vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3){
    v_float32x4 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    return v;
}

inline v_uint32x4 vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? 0xffffffff : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? 0xffffffff : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? 0xffffffff : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? 0xffffffff : 0;
    return v;
}

inline v_uint32x4 vq_cmple_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? 0xffffffff : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? 0xffffffff : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? 0xffffffff : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? 0xffffffff : 0;
    return v;
}

inline v_uint32x4 vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? 0xffffffff : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? 0xffffffff : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? 0xffffffff : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? 0xffffffff : 0;
    return v;
}

inline v_uint32x4 vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? 0xffffffff : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? 0xffffffff : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? 0xffffffff : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? 0xffffffff : 0;
    return v;
}

#endif



#endif // PIXEL_SIMD_OLD_H
#ifndef PIXEL_SIMD_H
#define PIXEL_SIMD_H

// 64bit  s  single (1x)
// 128bit d  double (2x)
// 256bit q  quadral(4x)
// 512bit o  octo   (8x)

#if defined(PIXEL_SSE)
#include <xmmintrin.h>
#endif

#if defined(PIXEL_NEON)
#include <arm_neon.h>
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
typedef __m64       v_int8x8;
typedef __m64       v_int16x4;
typedef __m64       v_int32x2;
typedef __m64       v_int64x1;
typedef __m64       v_float32x2;
typedef __m64       v_uint8x8;
typedef __m64       v_uint16x4;
typedef __m64       v_uint32x2;
typedef __m64       v_uint64x1;

// 16 byte align required. XMM registers. 128bit
typedef __m128      v_int8x16;
typedef __m128      v_int16x8;
typedef __m128      v_int32x4;
typedef __m128      v_int64x2;
typedef __m128      v_float32x4;
typedef __m128      v_uint8x16;
typedef __m128      v_uint16x8;
typedef __m128      v_uint32x4;
typedef __m128      v_uint64x2;

#else

typedef struct v_float32x4 {
    float data_[4]; // for compatibility, only API can access _data
} v_float32x4;

#endif


//----------------------------------------------------------------------
// function definitions
//----------------------------------------------------------------------
inline v_float32x4 v_setzero_f32(); // assign all element to zero
inline v_float32x4 v_load_f32(const float* data);
inline v_float32x4 v_mul_f32(v_float32x4 v1, v_float32x4 v2);
inline v_float32x4 v_add_f32(v_float32x4 v1, v_float32x4 v2);
inline void v_store_f32(float* arr, v_float32x4 v);
inline float v_reduce_sum(v_float32x4 v);

//----------------------------------------------------------------------
// function implementations
//----------------------------------------------------------------------
//----- NEON
#if defined(PIXEL_NEON)
inline v_float32x4 v_setzero_f32() {
    return vdupq_n_f32(0);
}

inline v_float32x4 v_load_f32(const float* data) {
    return vld1q_f32(data);
}

inline v_float32x4 v_mul_f32(v_float32x4 v1, v_float32x4 v2) {
    return vmulq_f32(v1, v2);
}

inline v_float32x4 v_add_f32(v_float32x4 v1, v_float32x4 v2) {
    return vaddq_f32(v1, v2);
}

inline void v_store_f32(float* arr, v_float32x4 v) {
    vst1q_f32(arr, v);
}

inline float v_reduce_sum(v_float32x4 v) {
    float arr[4];
    vst1q_f32(arr, v);
    return arr[0]+arr[1]+arr[2]+arr[3];
}

//----- SSE 
#elif defined(PIXEL_SSE)
inline v_float32x4 v_setzero_f32() {
    return _mm_setzero_ps();
}

inline v_float32x4 v_load_f32(const float* data) {
    return _mm_loadu_ps(data); // use un-aligned version for debugging case
}

inline v_float32x4 v_mul_f32(v_float32x4 v1, v_float32x4 v2){
    return _mm_mul_ps(v1, v2);
}

inline v_float32x4 v_add_f32(v_float32x4 v1, v_float32x4 v2) {
    return _mm_add_ps(v1, v2);
}

inline void v_store_f32(float* arr, v_float32x4 v) {
    _mm_store_ps(arr, v);
}

inline float v_reduce_sum(v_float32x4 v) {
    float arr[4];
    _mm_store_ps(arr, v);
    return arr[0]+arr[1]+arr[2]+arr[3];
}

//------ NO SIMD
#else

inline v_float32x4 v_setzero_f32() {
    v_float32x4 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    return v;
}

inline v_float32x4 v_load_f32(const float* data) {
    v_float32x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
}

inline v_float32x4 v_mul_f32(v_float32x4 v1, v_float32x4 v2) {
    v_float32x4 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    return v;
}

inline v_float32x4 v_add_f32(v_float32x4 v1, v_float32x4 v2) {
    v_float32x4 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    return v;
}

inline void v_store_f32(float* arr, v_float32x4 v) {
    arr[0] = v.data_[0];
    arr[1] = v.data_[1];
    arr[2] = v.data_[2];
    arr[3] = v.data_[3];
}

inline float v_reduce_sum(v_float32x4 v) {
    return v.data_[0]+v.data_[1]+v.data_[2]+v.data_[3];
}

#endif



#endif // PIXEL_SIMD_H
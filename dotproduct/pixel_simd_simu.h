#ifndef PIXEL_SIMD_SIMU_H
#define PIXEL_SIMD_SIMU_H

#include "pixel_simd_base.h"

//----------------------------------------------------------------------
// assign all element to zero
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_setzero_s8() {
    v_int8x8 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    v.data_[4] = 0; v.data_[5] = 0; v.data_[6] = 0; v.data_[7] = 0;
    return v;
}

inline v_int16x4 vd_setzero_s16() {
    v_int16x4 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    return v;
}

inline v_int32x2 vd_setzero_s32() {
    v_int32x2 v;
    v.data_[0] = 0; v.data_[1] = 0;
    return v;
}

inline v_int64x1 vd_setzero_s64() {
    v_int64x1 v;
    v.data_[0] = 0;
    return v;
}

inline v_float32x2 vd_setzero_f32() {
    v_float32x2 v;
    v.data_[0] = 0.f; v.data_[1] = 0.f;
    return v;
}

inline v_uint8x8 vd_setzero_u8() {
    v_uint8x8 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    v.data_[4] = 0; v.data_[5] = 0; v.data_[6] = 0; v.data_[7] = 0;
    return v;
}

inline v_uint16x4 vd_setzero_u16() {
    v_uint16x4 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    return v;
}

inline v_uint32x2 vd_setzero_u32() {
    v_uint32x2 v;
    v.data_[0] = 0; v.data_[1] = 0;
    return v;
}

inline v_uint64x1 vd_setzero_u64() {
    v_uint64x1 v;
    v.data_[0] = 0;
    return v;
}

// 16bytes(128bits) part
inline v_int8x16 vq_setzero_s8() {
    v_int8x16 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    v.data_[4] = 0; v.data_[5] = 0; v.data_[6] = 0; v.data_[7] = 0;
    v.data_[8] = 0; v.data_[9] = 0; v.data_[10] = 0; v.data_[11] = 0;
    v.data_[12] = 0; v.data_[13] = 0; v.data_[14] = 0; v.data_[15] = 0;
    return v;
}

inline v_int16x8 vq_setzero_s16() {
    v_int16x8 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    v.data_[4] = 0; v.data_[5] = 0; v.data_[6] = 0; v.data_[7] = 0;
    return v;
}

inline v_int32x4 vq_setzero_s32() {
    v_int32x4 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    return v;
}

inline v_int64x2 vq_setzero_s64() {
    v_int64x2 v;
    v.data_[0] = 0; v.data_[1] = 0;
    return v;
}

inline v_float32x4 vq_setzero_f32() {
    v_float32x4 v;
    v.data_[0] = 0.f; v.data_[1] = 0.f; v.data_[2] = 0.f; v.data_[3] = 0.f;
    return v;
}

inline v_uint8x16 vq_setzero_u8() {
    v_uint8x16 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    v.data_[4] = 0; v.data_[5] = 0; v.data_[6] = 0; v.data_[7] = 0;
    v.data_[8] = 0; v.data_[9] = 0; v.data_[10] = 0; v.data_[11] = 0;
    v.data_[12] = 0; v.data_[13] = 0; v.data_[14] = 0; v.data_[15] = 0;
    return v;
}

inline v_uint16x8 vq_setzero_u16() {
    v_uint16x8 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    v.data_[4] = 0; v.data_[5] = 0; v.data_[6] = 0; v.data_[7] = 0;
    return v;
}

inline v_uint32x4 vq_setzero_u32() {
    v_uint32x4 v;
    v.data_[0] = 0; v.data_[1] = 0; v.data_[2] = 0; v.data_[3] = 0;
    return v;
}

inline v_uint64x2 vq_setzero_u64() {
    v_uint64x2 v;
    v.data_[0] = 0; v.data_[1] = 0;
    return v;
}

//----------------------------------------------------------------------
// assign all element to value
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8      vd_setvalue_s8(int8_t value) {
    v_int8x8 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    v.data_[4] = value; v.data_[5] = value; v.data_[6] = value; v.data_[7] = value;
    return v;
}

inline v_int16x4     vd_setvalue_s16(int16_t value) {
    v_int16x4 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    return v;
}

inline v_int32x2     vd_setvalue_s32(int32_t value) {
    v_int32x2 v;
    v.data_[0] = value; v.data_[1] = value;
    return v;
}

inline v_int64x1     vd_setvalue_s64(int64_t value) { 
    v_int64x1 v;
    v.data_[0] = value;
    return v;
}

inline v_float32x2   vd_setvalue_f32(float value) {
    v_int32x2 v;
    v.data_[0] = value; v.data_[1] = value;
    return v;
}

inline v_uint8x8     vd_setvalue_u8(uint8_t value) { 
    v_uint8x8 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    v.data_[4] = value; v.data_[5] = value; v.data_[6] = value; v.data_[7] = value;
    return v;
}

inline v_uint16x4    vd_setvalue_u16(uint16_t value) {
    v_uint16x4 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    return v;
}

inline v_uint32x2    vd_setvalue_u32(uint32_t value) {
    v_uint32x2 v;
    v.data_[0] = value; v.data_[1] = value;
    return v;
}

inline v_uint64x1    vd_setvalue_u64(uint64_t value) { 
    v_uint64x1 v;
    v.data_[0] = value;
    return v;
}

// 16bytes(128bits) part
inline v_int8x16     vq_setvalue_s8(int8_t value) {
    v_int8x16 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    v.data_[4] = value; v.data_[5] = value; v.data_[6] = value; v.data_[7] = value;
    v.data_[8] = value; v.data_[9] = value; v.data_[10] = value; v.data_[11] = value;
    v.data_[12] = value; v.data_[13] = value; v.data_[14] = value; v.data_[15] = value;
    return v;
}

inline v_int16x8     vq_setvalue_s16(int16_t value) {
    v_int16x8 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    v.data_[4] = value; v.data_[5] = value; v.data_[6] = value; v.data_[7] = value;
    return v;
}

inline v_int32x4     vq_setvalue_s32(int32_t value) {
    v_int32x4 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    return v;
}

inline v_int64x2     vq_setvalue_s64(int64_t value) {
    v_int64x2 v;
    v.data_[0] = value; v.data_[1] = value;
    return v;
}

inline v_float32x4   vq_setvalue_f32(float value) { 
    v_float32x4 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    return v;
}

inline v_uint8x16    vq_setvalue_u8(uint8_t value) { 
    v_uint8x16 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    v.data_[4] = value; v.data_[5] = value; v.data_[6] = value; v.data_[7] = value;
    v.data_[8] = value; v.data_[9] = value; v.data_[10] = value; v.data_[11] = value;
    v.data_[12] = value; v.data_[13] = value; v.data_[14] = value; v.data_[15] = value;
    return v;
}

inline v_uint16x8    vq_setvalue_u16(uint16_t value) {
    v_uint16x8 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    v.data_[4] = value; v.data_[5] = value; v.data_[6] = value; v.data_[7] = value;
    return v;
}

inline v_uint32x4    vq_setvalue_u32(uint32_t value) { 
    v_uint32x4 v;
    v.data_[0] = value; v.data_[1] = value; v.data_[2] = value; v.data_[3] = value;
    return v;
}

inline v_uint64x2    vq_setvalue_u64(uint64_t value) {
    v_uint64x4 v;
    v.data_[0] = value; v.data_[1] = value;
    return v;
}

//----------------------------------------------------------------------
// copy data, memory => register
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_load_s8 (const int8_t* data) {
    v_int8x8 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_int16x4 vd_load_s16(const int16_t* data) {
    v_int16x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_int32x2 vd_load_s32(const int32_t* data) {
    v_int32x2 v;
    v.data_[0] = data[0];
    v.data_[1] = data[1];
    return v;
}

inline v_int64x1 vd_load_s64(const int64_t* data) {
    v_int64x1 v;
    v.data_[0] = data[0];
    return v;
}

inline v_float32x2 vd_load_f32(const float* data) {
    v_float32x2 v;
    v.data_[0] = data[0];
    v.data_[1] = data[1];
    return v;
}

inline v_uint8x8 vd_load_u8 (const uint8_t* data) {
    v_uint8x8 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_uint16x4 vd_load_u16(const uint16_t* data) {
    v_uint16x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_uint32x2 vd_load_u32(const uint32_t* data) {
    v_uint32x2 v;
    v.data_[0] = data[0];
    v.data_[1] = data[1];
    return v;
}

inline v_uint64x1 vd_load_u64(const uint64_t* data) {
    v_uint64x1 v;
    v.data_[0] = data[0];
    return v;
}

// 16bytes(128bits) part
inline v_int8x16 vq_load_s8 (const int8_t* data) {
    v_int8x16 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_int16x8 vq_load_s16(const int16_t* data) {
    v_int16x8 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_int32x4 vq_load_s32(const int32_t* data) {
    v_int32x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_int64x2 vq_load_s64(const int64_t* data) {
    v_int64x2 v;
    v.data_[0] = data[0];
    v.data_[1] = data[1];
    return v;
}

inline v_float32x4 vq_load_f32(const float* data) {
    v_float32x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_uint8x16 vq_load_u8 (const uint8_t* data) {
    v_uint8x16 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_uint16x8 vq_load_u16(const uint16_t* data) {
    v_uint16x8 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_uint32x4 vq_load_u32(const uint32_t* data) {
    v_uint32x4 v;
    memcpy(v.data_, data, sizeof(v.data_));
    return v;
}

inline v_uint64x2 vq_load_u64(const uint64_t* data) {
    v_uint64x2 v;
    v.data_[0] = data[0];
    v.data_[1] = data[1];
    return v;
}

//----------------------------------------------------------------------
// copy-data, register => memory
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline void vd_store_s8 (int8_t* data, v_int8x8 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vd_store_s16(int16_t* data, v_int16x4 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vd_store_s32(int32_t* data, v_int32x2 v) {
    data[0] = v.data_[0];
    data[1] = v.data_[1];
}

inline void vd_store_s64(int64_t* data, v_int64x1 v) {
    data[0] = v.data_[0];
}

inline void vd_store_f32(float* data, v_float32x2 v) {
    data[0] = v.data_[0];
    data[1] = v.data_[1];
}

inline void vd_store_u8 (uint8_t* data, v_uint8x8 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vd_store_u16(uint16_t* data, v_uint16x4 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vd_store_u32(uint32_t* data, v_uint32x2 v) {
    data[0] = v.data_[0];
    data[1] = v.data_[1];
}

inline void vd_store_u64(uint64_t* data, v_uint64x1 v) {
    data[0] = v.data_[0];
}

// 16bytes(128bits) part
inline void vq_store_s8(int8_t* data, v_int8x16 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vq_store_s16(int16_t* data, v_int16x8 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vq_store_s32(int32_t* data, v_int32x4 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vq_store_s64(int64_t* data, v_int64x2 v) {
    data[0] = v.data_[0];
    data[1] = v.data_[1];
}

inline void vq_store_f32(float* data, v_float32x4 v) {
    //memcpy(data, v.data_, sizeof(v.data_));
    data[0] = v.data_[0];
    data[1] = v.data_[1];
    data[2] = v.data_[2];
    data[3] = v.data_[3];
}

inline void vq_store_u8 (uint8_t* data, v_uint8x16 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vq_store_u16(uint16_t* data, v_uint16x8 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vq_store_u32(uint32_t* data, v_uint32x4 v) {
    memcpy(data, v.data_, sizeof(v.data_));
}

inline void vq_store_u64(uint64_t* data, v_uint64x2 v) {
    data[0] = v.data_[0];
    data[1] = v.data_[1];
}

//----------------------------------------------------------------------
// add(v1, v2), return v1[i] + v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_add_s8 (v_int8x8 v1, v_int8x8 v2) {
    v_int8x8 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    v.data_[4] = v1.data_[4] + v2.data_[4];
    v.data_[5] = v1.data_[5] + v2.data_[5];
    v.data_[6] = v1.data_[6] + v2.data_[6];
    v.data_[7] = v1.data_[7] + v2.data_[7];
    return v;
}

inline v_int16x4 vd_add_s16(v_int16x4 v1, v_int16x4 v2) {
    v_int16x4 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    return v;
}

inline v_int32x2 vd_add_s32(v_int32x2 v1, v_int32x2 v2) {
    v_int32x2 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    return v;
}

inline v_int64x1 vd_add_s64(v_int64x1 v1, v_int64x1 v2) {
    v_int64x1 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    return v;
}

inline v_float32x2 vd_add_f32(v_float32x2 v1, v_float32x2 v2) {
    v_float32x2 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    return v;
}

inline v_uint8x8 vd_add_u8 (v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    v.data_[4] = v1.data_[4] + v2.data_[4];
    v.data_[5] = v1.data_[5] + v2.data_[5];
    v.data_[6] = v1.data_[6] + v2.data_[6];
    v.data_[7] = v1.data_[7] + v2.data_[7];
    return v;
}

inline v_uint16x4 vd_add_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    return v;
}

inline v_uint32x2 vd_add_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    return v;
}

inline v_uint64x1 vd_add_u64(v_uint64x1 v1, v_uint64x1 v2) {
    v_uint64x1 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    return v;
}

// 16bytes(128bits) part
inline v_int8x16 vq_add_s8 (v_int8x16 v1, v_int8x16 v2) {
    v_int8x16 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    v.data_[4] = v1.data_[4] + v2.data_[4];
    v.data_[5] = v1.data_[5] + v2.data_[5];
    v.data_[6] = v1.data_[6] + v2.data_[6];
    v.data_[7] = v1.data_[7] + v2.data_[7];
    v.data_[8] = v1.data_[8] + v2.data_[8];
    v.data_[9] = v1.data_[9] + v2.data_[9];
    v.data_[10] = v1.data_[10] + v2.data_[10];
    v.data_[11] = v1.data_[11] + v2.data_[11];
    v.data_[12] = v1.data_[12] + v2.data_[12];
    v.data_[13] = v1.data_[13] + v2.data_[13];
    v.data_[14] = v1.data_[14] + v2.data_[14];
    v.data_[15] = v1.data_[15] + v2.data_[15];
    return v;
}

inline v_int16x8 vq_add_s16(v_int16x8 v1, v_int16x8 v2) {
    v_int16x8 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    v.data_[4] = v1.data_[4] + v2.data_[4];
    v.data_[5] = v1.data_[5] + v2.data_[5];
    v.data_[6] = v1.data_[6] + v2.data_[6];
    v.data_[7] = v1.data_[7] + v2.data_[7];
    return v;
}

inline v_int32x4 vq_add_s32(v_int32x4 v1, v_int32x4 v2) {
    v_int32x4 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    return v;
}

inline v_int64x2 vq_add_s64(v_int64x2 v1, v_int64x2 v2) {
    v_int64x2 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
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

inline v_uint8x16 vq_add_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    v.data_[4] = v1.data_[4] + v2.data_[4];
    v.data_[5] = v1.data_[5] + v2.data_[5];
    v.data_[6] = v1.data_[6] + v2.data_[6];
    v.data_[7] = v1.data_[7] + v2.data_[7];
    v.data_[8] = v1.data_[8] + v2.data_[8];
    v.data_[9] = v1.data_[9] + v2.data_[9];
    v.data_[10] = v1.data_[10] + v2.data_[10];
    v.data_[11] = v1.data_[11] + v2.data_[11];
    v.data_[12] = v1.data_[12] + v2.data_[12];
    v.data_[13] = v1.data_[13] + v2.data_[13];
    v.data_[14] = v1.data_[14] + v2.data_[14];
    v.data_[15] = v1.data_[15] + v2.data_[15];
    return v;
}

inline v_uint16x8 vq_add_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    v.data_[4] = v1.data_[4] + v2.data_[4];
    v.data_[5] = v1.data_[5] + v2.data_[5];
    v.data_[6] = v1.data_[6] + v2.data_[6];
    v.data_[7] = v1.data_[7] + v2.data_[7];
    return v;
}

inline v_uint32x4 vq_add_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    v.data_[2] = v1.data_[2] + v2.data_[2];
    v.data_[3] = v1.data_[3] + v2.data_[3];
    return v;
}

inline v_uint64x2 vq_add_u64(v_uint64x2 v1, v_uint64x2 v2) {
    v_uint64x2 v;
    v.data_[0] = v1.data_[0] + v2.data_[0];
    v.data_[1] = v1.data_[1] + v2.data_[1];
    return v;
}

//----------------------------------------------------------------------
// mul(v1, v2), return v1[i] * v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_mul_s8 (v_int8x8 v1, v_int8x8 v2) {
    v_int8x8 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    v.data_[4] = v1.data_[4] * v2.data_[4];
    v.data_[5] = v1.data_[5] * v2.data_[5];
    v.data_[6] = v1.data_[6] * v2.data_[6];
    v.data_[7] = v1.data_[7] * v2.data_[7];
    return v;
}

inline v_int16x4 vd_mul_s16(v_int16x4 v1, v_int16x4 v2) {
    v_int16x4 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    return v;
}

inline v_int32x2 vd_mul_s32(v_int32x2 v1, v_int32x2 v2) {
    v_int32x2 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    return v;
}

inline v_float32x2 vd_mul_f32(v_float32x2 v1, v_float32x2 v2) {
    v_float32x2 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    return v;
}

inline v_uint8x8 vd_mul_u8(v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    v.data_[4] = v1.data_[4] * v2.data_[4];
    v.data_[5] = v1.data_[5] * v2.data_[5];
    v.data_[6] = v1.data_[6] * v2.data_[6];
    v.data_[7] = v1.data_[7] * v2.data_[7];
    return v;
}

inline v_uint16x4 vd_mul_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    return v;
}

inline v_uint32x2 vd_mul_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    return v;
}

// 16bytes(128bits) part
inline v_int8x16 vq_mul_s8 (v_int8x16 v1, v_int8x16 v2) {
    v_int8x16 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    v.data_[4] = v1.data_[4] * v2.data_[4];
    v.data_[5] = v1.data_[5] * v2.data_[5];
    v.data_[6] = v1.data_[6] * v2.data_[6];
    v.data_[7] = v1.data_[7] * v2.data_[7];
    v.data_[8] = v1.data_[8] * v2.data_[8];
    v.data_[9] = v1.data_[9] * v2.data_[9];
    v.data_[10] = v1.data_[10] * v2.data_[10];
    v.data_[11] = v1.data_[11] * v2.data_[11];
    v.data_[12] = v1.data_[12] * v2.data_[12];
    v.data_[13] = v1.data_[13] * v2.data_[13];
    v.data_[14] = v1.data_[14] * v2.data_[14];
    v.data_[15] = v1.data_[15] * v2.data_[15];
    return v;
}

inline v_int16x8 vq_mul_s16(v_int16x8 v1, v_int16x8 v2) {
    v_int16x8 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    v.data_[4] = v1.data_[4] * v2.data_[4];
    v.data_[5] = v1.data_[5] * v2.data_[5];
    v.data_[6] = v1.data_[6] * v2.data_[6];
    v.data_[7] = v1.data_[7] * v2.data_[7];
    return v;
}

inline v_int32x4 vq_mul_s32(v_int32x4 v1, v_int32x4 v2) {
    v_int32x4 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
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

inline v_uint8x16 vq_mul_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    v.data_[4] = v1.data_[4] * v2.data_[4];
    v.data_[5] = v1.data_[5] * v2.data_[5];
    v.data_[6] = v1.data_[6] * v2.data_[6];
    v.data_[7] = v1.data_[7] * v2.data_[7];
    v.data_[8] = v1.data_[8] * v2.data_[8];
    v.data_[9] = v1.data_[9] * v2.data_[9];
    v.data_[10] = v1.data_[10] * v2.data_[10];
    v.data_[11] = v1.data_[11] * v2.data_[11];
    v.data_[12] = v1.data_[12] * v2.data_[12];
    v.data_[13] = v1.data_[13] * v2.data_[13];
    v.data_[14] = v1.data_[14] * v2.data_[14];
    v.data_[15] = v1.data_[15] * v2.data_[15];
    return v;
}

inline v_uint16x8 vq_mul_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    v.data_[4] = v1.data_[4] * v2.data_[4];
    v.data_[5] = v1.data_[5] * v2.data_[5];
    v.data_[6] = v1.data_[6] * v2.data_[6];
    v.data_[7] = v1.data_[7] * v2.data_[7];
    return v;
}

inline v_uint32x4 vq_mul_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = v1.data_[0] * v2.data_[0];
    v.data_[1] = v1.data_[1] * v2.data_[1];
    v.data_[2] = v1.data_[2] * v2.data_[2];
    v.data_[3] = v1.data_[3] * v2.data_[3];
    return v;
}

//----------------------------------------------------------------------
// sub(v1, v2), return v1[i] - v2[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_sub_s8 (v_int8x8 v1, v_int8x8 v2) {
    v_int8x8 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    v.data_[4] = v1.data_[4] - v2.data_[4];
    v.data_[5] = v1.data_[5] - v2.data_[5];
    v.data_[6] = v1.data_[6] - v2.data_[6];
    v.data_[7] = v1.data_[7] - v2.data_[7];
    return v;
}

inline v_int16x4 vd_sub_s16(v_int16x4 v1, v_int16x4 v2) {
    v_int16x4 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    return v;
}

inline v_int32x2 vd_sub_s32(v_int32x2 v1, v_int32x2 v2) {
    v_int32x2 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    return v;
}

inline v_int64x1 vd_sub_s64(v_int64x1 v1, v_int64x1 v2) {
    v_int64x1 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    return v;
}

inline v_float32x2 vd_sub_f32(v_float32x2 v1, v_float32x2 v2) {
    v_float32x2 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    return v;
}

inline v_uint8x8 vd_sub_u8(v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    v.data_[4] = v1.data_[4] - v2.data_[4];
    v.data_[5] = v1.data_[5] - v2.data_[5];
    v.data_[6] = v1.data_[6] - v2.data_[6];
    v.data_[7] = v1.data_[7] - v2.data_[7];
    return v;
}

inline v_uint16x4 vd_sub_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    return v;
}

inline v_uint32x2 vd_sub_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    return v;
}

inline v_uint64x1 vd_sub_u64(v_uint64x1 v1, v_uint64x1 v2) {
    v_uint64x1 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    return v;
}

// 16bytes(128bits) part
inline v_int8x16 vq_sub_s8(v_int8x16 v1, v_int8x16 v2) {
    v_int8x16 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    v.data_[4] = v1.data_[4] - v2.data_[4];
    v.data_[5] = v1.data_[5] - v2.data_[5];
    v.data_[6] = v1.data_[6] - v2.data_[6];
    v.data_[7] = v1.data_[7] - v2.data_[7];
    v.data_[8] = v1.data_[8] - v2.data_[8];
    v.data_[9] = v1.data_[9] - v2.data_[9];
    v.data_[10] = v1.data_[10] - v2.data_[10];
    v.data_[11] = v1.data_[11] - v2.data_[11];
    v.data_[12] = v1.data_[12] - v2.data_[12];
    v.data_[13] = v1.data_[13] - v2.data_[13];
    v.data_[14] = v1.data_[14] - v2.data_[14];
    v.data_[15] = v1.data_[15] - v2.data_[15];
    return v;
}

inline v_int16x8 vq_sub_s16(v_int16x8 v1, v_int16x8 v2) {
    v_int16x8 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    v.data_[4] = v1.data_[4] - v2.data_[4];
    v.data_[5] = v1.data_[5] - v2.data_[5];
    v.data_[6] = v1.data_[6] - v2.data_[6];
    v.data_[7] = v1.data_[7] - v2.data_[7];
    return v;
}

inline v_int32x4 vq_sub_s32(v_int32x4 v1, v_int32x4 v2) {
    v_int32x4 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    return v;
}

inline v_int64x2 vq_sub_s64(v_int64x2 v1, v_int64x2 v2) {
    v_int64x2 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    return v;
}

inline v_float32x4 vq_sub_f32(v_float32x4 v1, v_float32x4 v2) {
    v_float32x4 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    return v;
}

inline v_uint8x16 vq_sub_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    v.data_[4] = v1.data_[4] - v2.data_[4];
    v.data_[5] = v1.data_[5] - v2.data_[5];
    v.data_[6] = v1.data_[6] - v2.data_[6];
    v.data_[7] = v1.data_[7] - v2.data_[7];
    v.data_[8] = v1.data_[8] - v2.data_[8];
    v.data_[9] = v1.data_[9] - v2.data_[9];
    v.data_[10] = v1.data_[10] - v2.data_[10];
    v.data_[11] = v1.data_[11] - v2.data_[11];
    v.data_[12] = v1.data_[12] - v2.data_[12];
    v.data_[13] = v1.data_[13] - v2.data_[13];
    v.data_[14] = v1.data_[14] - v2.data_[14];
    v.data_[15] = v1.data_[15] - v2.data_[15];
    return v;
}

inline v_uint16x8 vq_sub_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    v.data_[4] = v1.data_[4] - v2.data_[4];
    v.data_[5] = v1.data_[5] - v2.data_[5];
    v.data_[6] = v1.data_[6] - v2.data_[6];
    v.data_[7] = v1.data_[7] - v2.data_[7];
    return v;
}

inline v_uint32x4 vq_sub_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    v.data_[2] = v1.data_[2] - v2.data_[2];
    v.data_[3] = v1.data_[3] - v2.data_[3];
    return v;
}

inline v_uint64x2 vq_sub_u64(v_uint64x2 v1, v_uint64x2 v2) {
    v_uint64x2 v;
    v.data_[0] = v1.data_[0] - v2.data_[0];
    v.data_[1] = v1.data_[1] - v2.data_[1];
    return v;
}

//----------------------------------------------------------------------
// fmadd(v1, v2, v3), return (v1[i]*v2[i])+v3[i]
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_int8x8 vd_fmadd_s8(v_int8x8 v1, v_int8x8 v2, v_int8x8 v3) {
    v_int8x8 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    v.data_[4] = (v1.data_[4] * v2.data_[4]) + v3.data_[4];
    v.data_[5] = (v1.data_[5] * v2.data_[5]) + v3.data_[5];
    v.data_[6] = (v1.data_[6] * v2.data_[6]) + v3.data_[6];
    v.data_[7] = (v1.data_[7] * v2.data_[7]) + v3.data_[7];
    return v;
}

inline v_int16x4 vd_fmadd_s16(v_int16x4 v1, v_int16x4 v2, v_int16x4 v3) {
    v_int16x4 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    return v;
}

inline v_int32x2 vd_fmadd_s32(v_int32x2 v1, v_int32x2 v2, v_int32x2 v3) {
    v_int32x2 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    return v;
}

inline v_int64x1 vd_fmadd_s64(v_int64x1 v1, v_int64x1 v2, v_int64x1 v3) {
    v_int64x1 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    return v;
}

inline v_float32x2 vd_fmadd_f32(v_float32x2 v1, v_float32x2 v2, v_float32x2 v3) {
    v_float32x2 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    return v;
}

inline v_uint8x8 vd_fmadd_u8(v_uint8x8 v1, v_uint8x8 v2, v_uint8x8 v3) {
    v_uint8x8 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    v.data_[4] = (v1.data_[4] * v2.data_[4]) + v3.data_[4];
    v.data_[5] = (v1.data_[5] * v2.data_[5]) + v3.data_[5];
    v.data_[6] = (v1.data_[6] * v2.data_[6]) + v3.data_[6];
    v.data_[7] = (v1.data_[7] * v2.data_[7]) + v3.data_[7];
    return v;
}

inline v_uint16x4 vd_fmadd_u16(v_uint16x4 v1, v_uint16x4 v2, v_uint16x4 v3) {
    v_uint16x4 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    return v;
}

inline v_uint32x2 vd_fmadd_u32(v_uint32x2 v1, v_uint32x2 v2, v_uint32x2 v3) {
    v_uint32x2 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    return v;
}

inline v_uint64x1 vd_fmadd_u64(v_uint64x1 v1, v_uint64x1 v2, v_uint64x1 v3) {
    v_uint64x1 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    return v;
}

// 16bytes(128bits) part
inline v_int8x16 vq_fmadd_s8 (v_int8x16 v1, v_int8x16 v2, v_int8x16 v3) {
    v_int8x16 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    v.data_[4] = (v1.data_[4] * v2.data_[4]) + v3.data_[4];
    v.data_[5] = (v1.data_[5] * v2.data_[5]) + v3.data_[5];
    v.data_[6] = (v1.data_[6] * v2.data_[6]) + v3.data_[6];
    v.data_[7] = (v1.data_[7] * v2.data_[7]) + v3.data_[7];
    v.data_[8] = (v1.data_[8] * v2.data_[8]) + v3.data_[8];
    v.data_[9] = (v1.data_[9] * v2.data_[9]) + v3.data_[9];
    v.data_[10] = (v1.data_[10] * v2.data_[10]) + v3.data_[10];
    v.data_[11] = (v1.data_[11] * v2.data_[11]) + v3.data_[11];
    v.data_[12] = (v1.data_[12] * v2.data_[12]) + v3.data_[12];
    v.data_[13] = (v1.data_[13] * v2.data_[13]) + v3.data_[13];
    v.data_[14] = (v1.data_[14] * v2.data_[14]) + v3.data_[14];
    v.data_[15] = (v1.data_[15] * v2.data_[15]) + v3.data_[15];
    return v;
}

inline v_int16x8 vq_fmadd_s16(v_int16x8 v1, v_int16x8 v2, v_int16x8 v3) {
    v_int16x8 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    v.data_[4] = (v1.data_[4] * v2.data_[4]) + v3.data_[4];
    v.data_[5] = (v1.data_[5] * v2.data_[5]) + v3.data_[5];
    v.data_[6] = (v1.data_[6] * v2.data_[6]) + v3.data_[6];
    v.data_[7] = (v1.data_[7] * v2.data_[7]) + v3.data_[7];
    return v;
}

inline v_int32x4 vq_fmadd_s32(v_int32x4 v1, v_int32x4 v2, v_int32x4 v3) {
    v_int32x4 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    return v;
}

inline v_float32x4 vq_fmadd_f32(v_float32x4 v1, v_float32x4 v2, v_float32x4 v3) {
    v_float32x4 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    return v;
}

inline v_uint8x16 vq_fmadd_u8 (v_uint8x16 v1, v_uint8x16 v2, v_uint8x16 v3) {
    v_uint8x16 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    v.data_[4] = (v1.data_[4] * v2.data_[4]) + v3.data_[4];
    v.data_[5] = (v1.data_[5] * v2.data_[5]) + v3.data_[5];
    v.data_[6] = (v1.data_[6] * v2.data_[6]) + v3.data_[6];
    v.data_[7] = (v1.data_[7] * v2.data_[7]) + v3.data_[7];
    v.data_[8] = (v1.data_[8] * v2.data_[8]) + v3.data_[8];
    v.data_[9] = (v1.data_[9] * v2.data_[9]) + v3.data_[9];
    v.data_[10] = (v1.data_[10] * v2.data_[10]) + v3.data_[10];
    v.data_[11] = (v1.data_[11] * v2.data_[11]) + v3.data_[11];
    v.data_[12] = (v1.data_[12] * v2.data_[12]) + v3.data_[12];
    v.data_[13] = (v1.data_[13] * v2.data_[13]) + v3.data_[13];
    v.data_[14] = (v1.data_[14] * v2.data_[14]) + v3.data_[14];
    v.data_[15] = (v1.data_[15] * v2.data_[15]) + v3.data_[15];
    return v;
}

inline v_uint16x8 vq_fmadd_u16(v_uint16x8 v1, v_uint16x8 v2, v_uint16x8 v3) {
    v_uint16x8 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    v.data_[4] = (v1.data_[4] * v2.data_[4]) + v3.data_[4];
    v.data_[5] = (v1.data_[5] * v2.data_[5]) + v3.data_[5];
    v.data_[6] = (v1.data_[6] * v2.data_[6]) + v3.data_[6];
    v.data_[7] = (v1.data_[7] * v2.data_[7]) + v3.data_[7];
    return v;
}

inline v_uint32x4 vq_fmadd_u32(v_uint32x4 v1, v_uint32x4 v2, v_uint32x4 v3) {
    v_uint32x4 v;
    v.data_[0] = (v1.data_[0] * v2.data_[0]) + v3.data_[0];
    v.data_[1] = (v1.data_[1] * v2.data_[1]) + v3.data_[1];
    v.data_[2] = (v1.data_[2] * v2.data_[2]) + v3.data_[2];
    v.data_[3] = (v1.data_[3] * v2.data_[3]) + v3.data_[3];
    return v;
}

//----------------------------------------------------------------------
// cmplt(v1, v2), return (v1[i] < v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmplt_s8(v_int8x8 v1, v_int8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] < v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] < v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] < v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] < v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmplt_s16(v_int16x4 v1, v_int16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmplt_s32(v_int32x2 v1, v_int32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmplt_s64(v_int64x1 v1, v_int64x1 v2) {
    v_uint64x1 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmplt_f32(v_float32x2 v1, v_float32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint8x8 vd_cmplt_u8(v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] < v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] < v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] < v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] < v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmplt_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmplt_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmplt_u64(v_uint64x1 v1, v_uint64x1 v2) {
    v_uint64x1 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    return v;
}

// 16bytes(128bits) part
inline v_uint8x16 vq_cmplt_s8(v_int8x16 v1, v_int8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] < v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] < v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] < v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] < v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] < v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] < v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] < v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] < v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] < v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] < v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] < v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] < v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmplt_s16(v_int16x8 v1, v_int16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] < v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] < v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] < v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] < v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmplt_s32(v_int32x4 v1, v_int32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmplt_s64(v_int64x2 v1, v_int64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmplt_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint8x16 vq_cmplt_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] < v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] < v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] < v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] < v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] < v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] < v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] < v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] < v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] < v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] < v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] < v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] < v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmplt_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] < v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] < v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] < v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] < v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmplt_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] < v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] < v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmplt_u64(v_uint64x2 v1, v_uint64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] < v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] < v2.data_[1]) ? f : 0;
    return v;
}

//----------------------------------------------------------------------
// cmple(v1, v2), return (v1[i] <= v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmple_s8(v_int8x8 v1, v_int8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] <= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] <= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] <= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] <= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmple_s16(v_int16x4 v1, v_int16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmple_s32(v_int32x2 v1, v_int32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmple_s64(v_int64x1 v1, v_int64x1 v2) {
    v_uint64x1 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmple_f32(v_float32x2 v1, v_float32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint8x8 vd_cmple_u8(v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    uint32_t f = 0xff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] <= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] <= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] <= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] <= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmple_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmple_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmple_u64(v_uint64x1 v1, v_uint64x1 v2) {
    v_uint64x1 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    return v;
}

// 16bytes(128bits) part
inline v_uint8x16 vq_cmple_s8(v_int8x16 v1, v_int8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] <= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] <= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] <= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] <= v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] <= v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] <= v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] <= v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] <= v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] <= v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] <= v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] <= v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] <= v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmple_s16(v_int16x8 v1, v_int16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] <= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] <= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] <= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] <= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmple_s32(v_int32x4 v1, v_int32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmple_s64(v_int64x2 v1, v_int64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f: 0;
    return v;
}

inline v_uint32x4 vq_cmple_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint8x16 vq_cmple_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] <= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] <= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] <= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] <= v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] <= v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] <= v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] <= v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] <= v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] <= v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] <= v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] <= v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] <= v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmple_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] <= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] <= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] <= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] <= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmple_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] <= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] <= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmple_u64(v_uint64x2 v1, v_uint64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] <= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] <= v2.data_[1]) ? f : 0;
    return v;
}

//----------------------------------------------------------------------
// cmpgt(v1, v2), return (v1[i] > v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmpgt_s8(v_int8x8 v1, v_int8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] > v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] > v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] > v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] > v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmpgt_s16(v_int16x4 v1, v_int16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmpgt_s32(v_int32x2 v1, v_int32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmpgt_s64(v_int64x1 v1, v_int64x1 v2) {
    v_uint64x1 v;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? 0xffffffffffffffff : 0;
    return v;
}

inline v_uint32x2 vd_cmpgt_f32(v_float32x2 v1, v_float32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint8x8 vd_cmpgt_u8(v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] > v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] > v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] > v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] > v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmpgt_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmpgt_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmpgt_u64(v_uint64x1 v1, v_uint64x1 v2) {
    v_uint64x1 v;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? 0xffffffffffffffff : 0;
    return v;
}

// 16bytes(128bits) part
inline v_uint8x16 vq_cmpgt_s8(v_int8x16 v1, v_int8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] > v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] > v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] > v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] > v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] > v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] > v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] > v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] > v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] > v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] > v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] > v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] > v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmpgt_s16(v_int16x8 v1, v_int16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] > v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] > v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] > v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] > v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmpgt_s32(v_int32x4 v1, v_int32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmpgt_s64(v_int64x2 v1, v_int64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmpgt_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint8x16 vq_cmpgt_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] > v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] > v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] > v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] > v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] > v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] > v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] > v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] > v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] > v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] > v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] > v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] > v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmpgt_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] > v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] > v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] > v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] > v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmpgt_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] > v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] > v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmpgt_u64(v_uint64x2 v1, v_uint64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] > v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] > v2.data_[1]) ? f : 0;
    return v;
}

//----------------------------------------------------------------------
// cmpge(v1, v2), return (v1[i] > v2[i])? 1...1 : 0...0
//----------------------------------------------------------------------
// 8bytes(64bits) part
inline v_uint8x8 vd_cmpge_s8(v_int8x8 v1, v_int8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] >= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] >= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] >= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] >= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmpge_s16(v_int16x4 v1, v_int16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmpge_s32(v_int32x2 v1, v_int32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmpge_s64(v_int64x1 v1, v_int64x1 v2) {
    v_uint64x1 v;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? 0xffffffffffffffff : 0;
    return v;
}

inline v_uint32x2 vd_cmpge_f32(v_float32x2 v1, v_float32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint8x8 vd_cmpge_u8(v_uint8x8 v1, v_uint8x8 v2) {
    v_uint8x8 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] >= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] >= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] >= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] >= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint16x4 vd_cmpge_u16(v_uint16x4 v1, v_uint16x4 v2) {
    v_uint16x4 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint32x2 vd_cmpge_u32(v_uint32x2 v1, v_uint32x2 v2) {
    v_uint32x2 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint64x1 vd_cmpge_u64(v_uint64x1 v1, v_uint64x1 v2) {
    v_uint64x1 v;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? 0xffffffffffffffff : 0;
    return v;
}

// 16bytes(128bits) part
inline v_uint8x16 vq_cmpge_s8(v_int8x16 v1, v_int8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] >= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] >= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] >= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] >= v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] >= v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] >= v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] >= v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] >= v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] >= v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] >= v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] >= v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] >= v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmpge_s16(v_int16x8 v1, v_int16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] >= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] >= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] >= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] >= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmpge_s32(v_int32x4 v1, v_int32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmpge_s64(v_int64x2 v1, v_int64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmpge_f32(v_float32x4 v1, v_float32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint8x16 vq_cmpge_u8(v_uint8x16 v1, v_uint8x16 v2) {
    v_uint8x16 v;
    uint8_t f = 0xff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] >= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] >= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] >= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] >= v2.data_[7]) ? f : 0;
    v.data_[8] = (v1.data_[8] >= v2.data_[8]) ? f : 0;
    v.data_[9] = (v1.data_[9] >= v2.data_[9]) ? f : 0;
    v.data_[10] = (v1.data_[10] >= v2.data_[10]) ? f : 0;
    v.data_[11] = (v1.data_[11] >= v2.data_[11]) ? f : 0;
    v.data_[12] = (v1.data_[12] >= v2.data_[12]) ? f : 0;
    v.data_[13] = (v1.data_[13] >= v2.data_[13]) ? f : 0;
    v.data_[14] = (v1.data_[14] >= v2.data_[14]) ? f : 0;
    v.data_[15] = (v1.data_[15] >= v2.data_[15]) ? f : 0;
    return v;
}

inline v_uint16x8 vq_cmpge_u16(v_uint16x8 v1, v_uint16x8 v2) {
    v_uint16x8 v;
    uint16_t f = 0xffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    v.data_[4] = (v1.data_[4] >= v2.data_[4]) ? f : 0;
    v.data_[5] = (v1.data_[5] >= v2.data_[5]) ? f : 0;
    v.data_[6] = (v1.data_[6] >= v2.data_[6]) ? f : 0;
    v.data_[7] = (v1.data_[7] >= v2.data_[7]) ? f : 0;
    return v;
}

inline v_uint32x4 vq_cmpge_u32(v_uint32x4 v1, v_uint32x4 v2) {
    v_uint32x4 v;
    uint32_t f = 0xffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    v.data_[2] = (v1.data_[2] >= v2.data_[2]) ? f : 0;
    v.data_[3] = (v1.data_[3] >= v2.data_[3]) ? f : 0;
    return v;
}

inline v_uint64x2 vq_cmpge_u64(v_uint64x2 v1, v_uint64x2 v2) {
    v_uint64x2 v;
    uint64_t f = 0xffffffffffffffff;
    v.data_[0] = (v1.data_[0] >= v2.data_[0]) ? f : 0;
    v.data_[1] = (v1.data_[1] >= v2.data_[1]) ? f : 0;
    return v;
}

//----------------------------------------------------------------------
// bitselect(mask, v1, v2)
// selects each bit for the destination from the first operand
// if the corresponding bit of the destination is 1,
// or from the second operand if the corresponding bit of the destination is 0.
//----------------------------------------------------------------------
// 8bytes(64bits) part

//TODO: improve impl w.r.t. https://stackoverflow.com/a/47990/2999096
inline v_int8x8 vd_bitselect_s8(v_uint8x8 mask, v_int8x8 v1, v_int8x8 v2) {
    v_int8x8 res;
    for (size_t i=0; i<8; i++) {
        res.data_[i] = 0;
        for (size_t j=7; j!=0; j--) {
            int mask_bit = ((mask.data_[i] & (1U<<j)) >> j);
            int value_bit;
            if (mask_bit==1) {
                value_bit = ((v1.data_[i] & (1U<<j)) >> j);
            } else {
                value_bit = ((v2.data_[i] & (1U<<j)) >> j);
            }
            res.data_[i] += (value_bit << j);
        }
    }
    return res;
}

inline v_int16x4 vd_bitselect_s16(v_uint16x4 mask, v_int16x4 v1, v_int16x4 v2);

inline v_int32x2 vd_bitselect_s32(v_uint32x2 mask, v_int32x2 v1, v_int32x2 v2);

inline v_int64x1 vd_bitselect_s64(v_uint64x1 mask, v_int64x1 v1, v_int64x1 v2);

inline v_float32x2 vd_bitselect_f32(v_uint32x2 mask, v_float32x2 v1, v_float32x2 v2);

inline v_uint8x8 vd_bitselect_u8(v_uint8x8 mask, v_uint8x8 v1, v_uint8x8 v2);
inline v_uint16x4 vd_bitselect_u16(v_uint16x4 mask, v_uint16x4 v1, v_uint16x4 v2);
inline v_uint32x2 vd_bitselect_u32(v_uint32x2 mask, v_uint32x2 v1, v_uint32x2 v2);
inline v_uint64x1 vd_bitselect_u64(v_uint64x1 mask, v_uint64x1 v1, v_uint64x1 v2);

// 16bytes(128bits) part
inline v_int8x16 vq_bitselect_s8(v_uint8x16 mask, v_int8x16 v1, v_int8x16 v2);
inline v_int16x8 vq_bitselect_s16(v_uint16x8 mask, v_int16x8 v1, v_int16x8 v2);
inline v_int32x4 vq_bitselect_s32(v_uint32x4 mask, v_int32x4 v1, v_int32x4 v2);
inline v_int64x2 vq_bitselect_s64(v_uint64x2 mask, v_int64x2 v1, v_int64x2 v2);
inline v_float32x4 vq_bitselect_f32(v_uint32x4 mask, v_float32x4 v1, v_float32x4 v2);
inline v_uint8x16 vq_bitselect_u8(v_uint8x16 mask, v_uint8x16 v1, v_uint8x16 v2);
inline v_uint16x8 vq_bitselect_u16(v_uint16x8 mask, v_uint16x8 v1, v_uint16x8 v2);
inline v_uint32x4 vq_bitselect_u32(v_uint32x4 mask, v_uint32x4 v1, v_uint32x4 v2);
inline v_uint64x2 vq_bitselect_u64(v_uint64x2 mask, v_uint64x2 v1, v_uint64x2 v2);


#endif
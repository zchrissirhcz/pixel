#include "matrix_column_max.h"
#include <stdlib.h>
#include <string.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

void matrix_column_max_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals)
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t idx = j * width + i;
            if (src[idx] > max_vals[i]) {
                max_vals[i] = src[idx];
            }
        }
    }
}

void matrix_column_max_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    unsigned char* src_line = src + width;
    for (size_t i = 1; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
            }
        }
        src_line += width;
    }
}

void matrix_column_max_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    unsigned char* src_line = src + width;

#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = width - width % step;
    uint16x8_t vprev_indicies;
    uint16x8_t vcur_indicies;
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vprev_indicies = vdupq_n_u16(i-1);
        vcur_indicies = vdupq_n_u16(i);
        for (size_t j=0; j<vec_size; j+=step) {
            uint8x16_t vsrc = vld1q_u8(src_line + j);
            uint8x16_t vmax_vals = vld1q_u8(max_vals + j);
            uint8x16_t vmask_gt = vcgtq_u8(vsrc, vmax_vals);
            vmax_vals = vbslq_u8(vmask_gt, vsrc, vmax_vals);
            vst1q_u8(max_vals + j, vmax_vals);
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
            }
        }
        src_line += width;
    }
}

//----------------------------------------------------------------------

void matrix_column_max_and_idx_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies)
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t idx = j * width + i;
            if (src[idx] > max_vals[i]) {
                max_vals[i] = src[idx];
                max_indicies[i] = j;
            }
        }
    }
}

void matrix_column_max_and_idx_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    memset(max_indicies, 0, width * sizeof(uint32_t));
    unsigned char* src_line = src + width;
    for (size_t i = 1; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
                max_indicies[j] = i;
            }
        }
        src_line += width;
        //break;
    }
}

#if 0
//结果不对
void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    memset(max_indicies, 0, width*sizeof(uint32_t));
    unsigned char* src_line = src + width;

#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = width - width % step;
    uint32x4x4_t vmax_indicies;
    uint32x4_t vcur_indicies;
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vcur_indicies = vdupq_n_u32(i);
        for (size_t j=0; j<vec_size; j+=step) {
            uint8x16_t vsrc = vld1q_u8(src_line + j);
            uint8x16_t vmax_vals = vld1q_u8(max_vals + j);
            uint8x16_t vmask_gt = vcgtq_u8(vsrc, vmax_vals);
            vmax_vals = vbslq_u8(vmask_gt, vsrc, vmax_vals);
            vst1q_u8(max_vals + j, vmax_vals);

            vmax_indicies = vld4q_u32(max_indicies+j);
            //0
            uint8x8_t vm_u8_low = vget_low_u8(vmask_gt);
            uint8x8_t vm_u8_high = vget_high_u8(vmask_gt);
            uint16x8_t vm_u8_low_u16 = vmovl_u8(vm_u8_low);
            uint16x8_t vm_u8_high_u16 = vmovl_u8(vm_u8_high);

            uint16x4_t vm_u8_low_u16_low = vget_low_u16(vm_u8_low_u16);
            uint16x4_t vm_u8_low_u16_high = vget_high_u16(vm_u8_low_u16);
            uint16x4_t vm_u8_high_u16_low = vget_low_u16(vm_u8_high_u16);
            uint16x4_t vm_u8_high_u16_high = vget_high_u16(vm_u8_high_u16);
            uint32x4_t vm_u8_low_u16_low_u32 = vmovl_u32(vm_u8_low_u16_low);
            uint32x4_t vm_u8_low_u16_high_u32 = vmovl_u32(vm_u8_low_u16_low);
            uint32x4_t vm_u8_high_u16_low_u32 = vmovl_u32(vm_u8_high_u16_low);
            uint32x4_t vm_u8_high_u16_high_u32 = vmovl_u32(vm_u8_high_u16_high);

            vmax_indicies.val[0] = vbslq_u32(vm_u8_low_u16_low_u32, vmax_indicies.val[0], vcur_indicies);
            vmax_indicies.val[1] = vbslq_u32(vm_u8_low_u16_high_u32, vmax_indicies.val[1], vcur_indicies);
            vmax_indicies.val[2] = vbslq_u32(vm_u8_high_u16_low_u32, vmax_indicies.val[2], vcur_indicies);
            vmax_indicies.val[3] = vbslq_u32(vm_u8_high_u16_high_u32, vmax_indicies.val[3], vcur_indicies);

            vst4q_u32(max_indicies+j, vmax_indicies);

            //1

            //2

            //3
            
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
                max_indicies[j] = i;
            }
        }
        src_line += width;
    }
}

#elif 0 
// 3.5 ms
void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    memset(max_indicies, 0, width*sizeof(uint32_t));
    unsigned char* src_line = src + width;

#if __ARM_NEON
    size_t step = 8;
    size_t vec_size = width - width % step;
    //uint32x4_t vmax_indicies;
    uint32x4_t vcur_indicies;
    uint8x8_t vone_u8 = vdup_n_u8(1);
    uint32x4_t vff_u32 = vdupq_n_u32(0xffffffff);
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vcur_indicies = vdupq_n_u32(i);
        for (size_t j=0; j<vec_size; j+=step) {
            uint8x8_t vsrc = vld1_u8(src_line + j);
            uint8x8_t vmax_vals = vld1_u8(max_vals + j);
            uint8x8_t vmask_gt = vcgt_u8(vsrc, vmax_vals);
            vmax_vals = vbsl_u8(vmask_gt, vsrc, vmax_vals);
            vst1_u8(max_vals + j, vmax_vals);

            
            //处理索引：这里用到的技巧是，把u8类型的mask转为u32类型的mask时，不能简单的vmovl
            //需要先把mask+1，然后vmolv，最后再减去1（或者加上0xffffffff)

            // uint8_t temp_mask_gt[8];
            // vst1_u8(temp_mask_gt, vmask_gt);
            // printf("i=%zu, j=%zu, mask_gt:\n", i, j);
            // for (size_t k=0; k<step; k++) {
            //     printf("%3u ", temp_mask_gt[k]);
            // }
            // printf("\n");

            vmask_gt = vadd_u8(vmask_gt, vone_u8);

            uint16x8_t vm_u16 = vmovl_u8(vmask_gt);

            // uint16_t temp_mask_u16[8];
            // vst1q_u16(temp_mask_u16, vm_u16);
            // printf("mask_u16:\n");
            // for (size_t k=0; k<step; k++) {
            //     printf("%3u ", temp_mask_u16[k]);
            // }
            // printf("\n");

            uint16x4_t vm_u16_high = vget_high_u16(vm_u16);
            uint16x4_t vm_u16_low = vget_low_u16(vm_u16);
            uint32x4_t vm_u16_high_u32 = vmovl_u16(vm_u16_high);
            uint32x4_t vm_u16_low_u32 = vmovl_u16(vm_u16_low);

            vm_u16_high_u32 = vaddq_u32(vm_u16_high_u32, vff_u32);
            vm_u16_low_u32 = vaddq_u32(vm_u16_low_u32, vff_u32);

            // uint32_t temp_mask_u32[8];
            // vst1q_u32(temp_mask_u32, vm_u16_low_u32);
            // vst1q_u32(temp_mask_u32+4, vm_u16_high_u32);
            // printf("mask_u32:\n");
            // for (size_t k=0; k<step; k++) {
            //     printf("%3u ", temp_mask_u32[k]);
            // }
            // printf("\n");

            //这里根据u32的mask更新max indicies时，使用uint32x4x2_t类型的，结果不对。。
            //分成两个uint32x4_t做，结果就对了。。好吧，还是这样直观一点。
            uint32x4_t vmax_indicies;

            vmax_indicies = vld1q_u32(max_indicies+j);
            vmax_indicies = vbslq_u32(vm_u16_low_u32, vcur_indicies, vmax_indicies);
            vst1q_u32(max_indicies+j, vmax_indicies);

            vmax_indicies = vld1q_u32(max_indicies+j+4);
            vmax_indicies = vbslq_u32(vm_u16_high_u32, vcur_indicies, vmax_indicies);
            vst1q_u32(max_indicies+j+4, vmax_indicies);

            //printf("\n");
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
                max_indicies[j] = i;
            }
        }
        src_line += width;

        //break;
    }
}
#elif 0
// 上一版的u8 mask转u32 mask有点丑，速度也不快。尝试优化
// 2.9 ms
void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    memset(max_indicies, 0, width*sizeof(uint32_t));
    unsigned char* src_line = src + width;

#if __ARM_NEON
    size_t step = 8;
    size_t vec_size = width - width % step;
    uint32x4_t vcur_indicies;
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vcur_indicies = vdupq_n_u32(i);
        for (size_t j=0; j<vec_size; j+=step) {
            uint8x8_t vsrc = vld1_u8(src_line + j);
            uint8x8_t vmax_vals = vld1_u8(max_vals + j);
            uint8x8_t vmask_gt = vcgt_u8(vsrc, vmax_vals);
            vmax_vals = vbsl_u8(vmask_gt, vsrc, vmax_vals);
            vst1_u8(max_vals + j, vmax_vals);

            uint16x8_t vm_s16 = vmovl_s8(vreinterpret_s8_u8(vmask_gt));
            uint32x4_t vm_u32_low = vreinterpretq_u32_s32(vmovl_s16(vget_low_s16(vm_s16)));
            uint32x4_t vm_u32_high = vreinterpretq_u32_s32(vmovl_s16(vget_high_s16(vm_s16)));

            uint32x4_t vmax_indicies;

            vmax_indicies = vld1q_u32(max_indicies+j);
            vmax_indicies = vbslq_u32(vm_u32_low, vcur_indicies, vmax_indicies);
            vst1q_u32(max_indicies+j, vmax_indicies);

            vmax_indicies = vld1q_u32(max_indicies+j+4);
            vmax_indicies = vbslq_u32(vm_u32_high, vcur_indicies, vmax_indicies);
            vst1q_u32(max_indicies+j+4, vmax_indicies);
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
                max_indicies[j] = i;
            }
        }
        src_line += width;

        //break;
    }
}
#else
// step从8增加到16
//1.46 ms
void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    memset(max_indicies, 0, width*sizeof(uint32_t));
    unsigned char* src_line = src + width;

#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = width - width % step;
    uint32x4_t vcur_indicies;
    uint8x16_t vsrc;
    uint8x16_t vmax_vals;
    uint8x16_t vmask_gt;
    uint32x4x4_t vmax_indicies;
    
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vcur_indicies = vdupq_n_u32(i);
        for (size_t j=0; j<vec_size; j+=step) {
            vsrc = vld1q_u8(src_line + j);
            vmax_vals = vld1q_u8(max_vals + j);
            vmask_gt = vcgtq_u8(vsrc, vmax_vals);
            vmax_vals = vbslq_u8(vmask_gt, vsrc, vmax_vals);
            vst1q_u8(max_vals + j, vmax_vals);

            uint16x8_t vm_s16_0 = vmovl_s8(vget_low_s8(vreinterpretq_s8_u8(vmask_gt)));
            uint16x8_t vm_s16_1 = vmovl_s8(vget_high_s8(vreinterpretq_s8_u8(vmask_gt)));
            uint32x4x4_t vm_u32;
            
            vm_u32.val[0] = vreinterpretq_u32_s32(vmovl_s16(vget_low_s16(vm_s16_0)));
            vm_u32.val[1] = vreinterpretq_u32_s32(vmovl_s16(vget_high_s16(vm_s16_0)));
            vm_u32.val[2] = vreinterpretq_u32_s32(vmovl_s16(vget_low_s16(vm_s16_1)));
            vm_u32.val[3] = vreinterpretq_u32_s32(vmovl_s16(vget_high_s16(vm_s16_1)));

            vmax_indicies.val[0] = vld1q_u32(max_indicies+j);
            vmax_indicies.val[0] = vbslq_u32(vm_u32.val[0], vcur_indicies, vmax_indicies.val[0]);
            vst1q_u32(max_indicies+j, vmax_indicies.val[0]);

            vmax_indicies.val[1] = vld1q_u32(max_indicies+j+4);
            vmax_indicies.val[1] = vbslq_u32(vm_u32.val[1], vcur_indicies, vmax_indicies.val[1]);
            vst1q_u32(max_indicies+j+4, vmax_indicies.val[1]);

            vmax_indicies.val[2] = vld1q_u32(max_indicies+j+8);
            vmax_indicies.val[2] = vbslq_u32(vm_u32.val[2], vcur_indicies, vmax_indicies.val[2]);
            vst1q_u32(max_indicies+j+8, vmax_indicies.val[2]);

            vmax_indicies.val[3] = vld1q_u32(max_indicies+j+12);
            vmax_indicies.val[3] = vbslq_u32(vm_u32.val[3], vcur_indicies, vmax_indicies.val[3]);
            vst1q_u32(max_indicies+j+12, vmax_indicies.val[3]);
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
                max_indicies[j] = i;
            }
        }
        src_line += width;

        //break;
    }
}
#endif
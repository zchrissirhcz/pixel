#include "rgb2gray.h"
#include "dotproduct/pixel_simd.h"

void rgb2bgr_naive(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes)
{
    size_t src_idx = 0;
    size_t dst_idx = 0;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            src_idx = i*src_linebytes + j*3;
            dst_idx = i*dst_linebytes + j*3;
            dst_buf[dst_idx+2] = src_buf[src_idx];
            dst_buf[dst_idx+1] = src_buf[src_idx+1];
            dst_buf[dst_idx]   = src_buf[src_idx+2];
        }
    }
}

void rgb2bgr_idxopt(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes)
{
    size_t src_idx = 0;
    size_t dst_idx = 0;
    unsigned char* src0 = src_buf;
    unsigned char* src1 = src_buf+1;
    unsigned char* src2 = src_buf+2;
    unsigned char* dst0 = dst_buf;
    unsigned char* dst1 = dst_buf+1;
    unsigned char* dst2 = dst_buf+2;
    size_t src_gap = src_linebytes - width*3;
    size_t dst_gap = dst_linebytes - width*3;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            *dst2 = *src0; dst2+=3; src0+=3;
            *dst1 = *src1; dst1+=3; src1+=3;
            *dst0 = *src2; dst0+=3; src2+=3;
        }
        dst0+=dst_gap; dst1+=dst_gap; dst2+=dst_gap;
        src0+=src_gap; src1+=src_gap; src2+=src_gap;
    }
}

void rgb2bgr_asimd(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes)
{
    size_t used_linebytes = width * 3;
    unsigned char* src_linebuf = src_buf;
    unsigned char* dst_linebuf = dst_buf;
    size_t src_line_gap = src_linebytes - used_linebytes;
    size_t dst_line_gap = dst_linebytes - used_linebytes;

#ifdef PIXEL_SIMD_NEON
    uint8x8x3_t v;
    uint8x8_t tmp;
    size_t step = 24;
    size_t neon_vec_size = used_linebytes - used_linebytes % step;
#endif
    for (size_t i=0; i<height; i++) {
        size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
        vec_size = neon_vec_size;
        for (size_t j=0; j<vec_size; j+=3) {
            v = vld3_u8(src_linebuf+j);
            tmp = v.val[0];
            v.val[0] = v.val[2];
            v.val[2] = tmp;
            vst3_u8(dst_linebuf+j, v);
        }
        src_linebuf += vec_size;
        dst_linebuf += vec_size;
#endif
        for (size_t j=vec_size; j<used_linebytes; j+=3) {
            dst_linebuf[2] = src_linebuf[0];
            dst_linebuf[1] = src_linebuf[1];
            dst_linebuf[0] = src_linebuf[2];
            dst_linebuf += 3;
            src_linebuf += 3;
        }
        src_linebuf += src_line_gap;
        dst_linebuf += dst_line_gap;
    }
}

void rgb2bgr_asm_naive(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes)
{
#ifdef __aarch64__
    size_t used_linebytes = width * 3;
    unsigned char* src_linebuf = src_buf;
    unsigned char* dst_linebuf = dst_buf;
    for (size_t i=0; i<height; i++) {
        const size_t step = 48;
        size_t vec_size = used_linebytes - used_linebytes % step;
        for (size_t j=0; j<vec_size; j+=step) {
            __asm__ volatile(
                "ld3    { v0.16b, v1.16b, v2.16b }, [%1]\n"
                "mov    v3.16b, v0.16b       \n"
                "mov    v0.16b, v2.16b       \n"
                "mov    v2.16b, v3.16b        \n"
                "st3    { v0.16b, v1.16b, v2.16b }, [%0]"
                : "=r"(dst_linebuf), //%0
                "=r"(src_linebuf)    //%1
                : "0"(dst_linebuf),
                "1"(src_linebuf)
                : "cc", "memory", "v0", "v1", "v2"
            );
            src_linebuf += step;
            dst_linebuf += step;
        }
        for (size_t j=vec_size; j<used_linebytes; j+=3) {
            dst_linebuf[j] = src_linebuf[j+2];
            dst_linebuf[j+1] = src_linebuf[j+1];
            dst_linebuf[j+2] = src_linebuf[j];
        }
    }
#endif
}

// still slow than naive implementation
// disassembling naive implementation is required.
// ref https://aijishu.com/a/1060000000116431
void rgb2bgr_asm(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes)
{
#ifdef __aarch64__
    size_t used_linebytes = width * 3;
    unsigned char* src_linebuf = src_buf;
    unsigned char* dst_linebuf = dst_buf;
    const size_t step = 48;
    size_t vec_size = used_linebytes - used_linebytes % step;
    size_t pre_neon_len = vec_size / step;
    for (size_t i=0; i<height; i++) {
        size_t neon_len = pre_neon_len;
        __asm__ volatile(
            "0: \n"
            "ld3    { v0.16b, v1.16b, v2.16b }, [%1], #48 \n"
            "mov    v3.16b, v0.16b       \n"
            "mov    v0.16b, v2.16b       \n"
            "mov    v2.16b, v3.16b        \n"
            "st3    { v0.16b, v1.16b, v2.16b }, [%0], #48 \n"
            "subs       %2, %2, #1              \n"
            "bne    0b  \n"
            : "=r"(dst_linebuf), //%0
            "=r"(src_linebuf),    //%1
            "=r"(neon_len)   //%2
            : "0"(dst_linebuf),
            "1"(src_linebuf),
            "2"(neon_len)
            : "cc", "memory", "v0", "v1", "v2"
        );
        for (size_t j=vec_size; j<used_linebytes; j+=3) {
            dst_linebuf[j] = src_linebuf[j+2];
            dst_linebuf[j+1] = src_linebuf[j+1];
            dst_linebuf[j+2] = src_linebuf[j];
        }
    }
#endif
}

void rgb2bgr_inplace_naive(unsigned char* buf, size_t height, size_t width, size_t linebytes)
{
    unsigned char tmp = 0;
    size_t idx = 0;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            idx = i*linebytes + j*3;
            tmp = buf[idx];
            buf[idx] = buf[idx+2];
            buf[idx+2] = tmp;
        }
    }
}

// release mode: nearly same fast as rgb2bgr_inplace_naive
// debug mode: 3x faster than rgb2bgr_inplace_naive
void rgb2bgr_inplace_naive2(unsigned char* buf, size_t height, size_t width, size_t linebytes)
{
    size_t used_linebytes = width * 3;
    size_t step = 12;
    size_t vec_size = used_linebytes - used_linebytes % step;
    uint32_t* p1 = (uint32_t*)buf;
    uint32_t* p2 = p1 + 1;
    uint32_t* p3 = p2 + 1;
    uint32_t v1, v2, v3, nv1, nv2, nv3;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<vec_size; j+=step) {
            v1 = *p1;
            v2 = *p2;
            v3 = *p3;

            // shuffle bits in the pixels
            /*
            // [R1 G1 B1 R2 | G2 B2 R3 G3 | B3 R4 G4 B4]
            // [B1 G1 R1 B2 | G2 R2 B3 G3 | R3 B4 G4 R4]
            nv1 = (
                ((v1 & 0x0000FF00) << 16) |
                (v1 & 0x00FF0000) |
                ((v1 & 0xFF000000) >> 16) |
                ((v2 & 0x00FF0000) >> 16)
            );

            nv2 = (
                (v2 & 0xFF000000) |
                ((v1 & 0x000000FF) << 16) |
                ((v3 & 0xFF000000) >> 16) |
                (v2 & 0x000000FF)
            );

            nv3 = (
                ((v2 & 0x0000FF00) << 16) |
                ((v3 & 0x000000FF) << 16) |
                (v3 & 0x0000FF00) |
                ((v3 & 0x00FF0000) >> 16)
            );
            */

            // [R2 B1 G1 R1 | G3 R3 B2 G2 | B4 G4 R4 B3]
            // [B2 R1 G1 B1 | G3 B3 R2 G2 | R4 G4 B4 R3]
            nv1 = (
                ((v2 & 0x0000FF00) << 16) |
                ((v1 & 0x000000FF) << 16) |
                (v1 & 0x0000FF00) |
                ((v1 & 0x00FF0000) >> 16)
            );

            nv2 = (
                ((v2 & 0xFF000000)) |
                ((v3 & 0x000000FF) << 16) |
                ((v1 & 0xFF000000) >> 16) |
                ((v2 & 0x000000FF))
            );

            nv3 = (
                ((v3 & 0x0000FF00) << 16) |
                ((v3 & 0x00FF0000)) |
                ((v3 & 0xFF000000) >> 16) |
                ((v2 & 0x00FF0000) >> 16)
            );

            *p1 = nv1;
            *p2 = nv2;
            *p3 = nv3;

            p1 += 3;
            p2 += 3;
            p3 += 3;
        }

        unsigned char* b1 = (unsigned char*)p1;
        // unsigned char* b2 = b1 + 1;
        unsigned char* b3 = b1 + 2;
        for (int i=vec_size; i<used_linebytes; i+=3) {
            unsigned char t = *b1;
            *b1 = *b3;
            *b3 = t;
            b1+=3;
            b3+=3;
        }
    }
}

void rgb2bgr_inplace_asm(unsigned char* buf, size_t height, size_t width, size_t linebytes)
{
#ifdef __aarch64__
    size_t used_linebytes = width * 3;
    unsigned char* linebuf = buf;
    const size_t step = 48;
    size_t vec_size = used_linebytes - used_linebytes % step;
    size_t pre_neon_len = vec_size / step;
    for (size_t i=0; i<height; i++) {
        size_t neon_len = pre_neon_len;
        __asm__ volatile(
            "0: \n"
            "ld3    { v0.16b, v1.16b, v2.16b }, [%0] \n"
            "mov    v3.16b, v0.16b       \n"
            "mov    v0.16b, v2.16b       \n"
            "mov    v2.16b, v3.16b        \n"
            "st3    { v0.16b, v1.16b, v2.16b }, [%0], #48 \n"
            "subs       %1, %1, #1              \n"
            "bne    0b  \n"
            : "=r"(linebuf), //%0
            "=r"(neon_len)   //%1
            : "0"(linebuf),
            "1"(neon_len)
            : "cc", "memory", "v0", "v1", "v2"
        );
        for (size_t j=vec_size; j<used_linebytes; j+=3) {
            unsigned char tmp = linebuf[j];
            linebuf[j] = linebuf[j+2];
            linebuf[j+2] = tmp;
        }
    }
#endif
}

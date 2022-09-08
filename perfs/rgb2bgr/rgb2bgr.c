#include "rgb2bgr.h"
#include <stdint.h>

void rgb2bgr_naive(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf)
{
    size_t len = height * width * 3;
    for (size_t i=0; i<len; i+=3) {
        dst_buf[i+2] = src_buf[i];
        dst_buf[i+1] = src_buf[i+1];
        dst_buf[i]   = src_buf[i+2];
    }
}

void rgb2bgr_idxopt(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf)
{
    size_t src_idx = 0;
    size_t dst_idx = 0;
    unsigned char* src0 = src_buf;
    unsigned char* src1 = src_buf+1;
    unsigned char* src2 = src_buf+2;
    unsigned char* dst0 = dst_buf;
    unsigned char* dst1 = dst_buf+1;
    unsigned char* dst2 = dst_buf+2;
    size_t len = height * width;
    for (size_t i=0; i<len; i++) {
        *dst2 = *src0; dst2+=3; src0+=3;
        *dst1 = *src1; dst1+=3; src1+=3;
        *dst0 = *src2; dst0+=3; src2+=3;
    }
}

void rgb2bgr_asimd(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf)
{
    size_t done = 0;
    size_t total_len = width * height * 3;
#ifdef PIXEL_SIMD_NEON
    size_t step = 24;
    size_t vec_size = total_len - total_len % step;

    uint8x8x3_t v;
    uint8x8_t tmp;

    for (size_t i=0; i<vec_size; i+=step) {
        v = vld3_u8(src_buf);
        src_buf += step;
        tmp = v.val[0];
        v.val[0] = v.val[2];
        v.val[2] = tmp;
        vst3_u8(dst_buf, v);
        dst_buf += step;
    }

    done = vec_size;
#endif

    for ( ; done<total_len; done+=3) {
        dst_buf[2] = src_buf[0];
        dst_buf[1] = src_buf[1];
        dst_buf[0] = src_buf[2];
        dst_buf += 3;
        src_buf += 3;
    }
}

// still slow than naive implementation
// disassembling naive implementation is required.
// ref https://aijishu.com/a/1060000000116431
void rgb2bgr_asm(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf)
{
    size_t done = 0;
    size_t total_len = width * height * 3;
#ifdef PIXEL_SIMD_NEON
    size_t vec_size;
#ifdef __aarch64__
    const size_t step = 48;
    vec_size = total_len - total_len % step;
    size_t neon_len = vec_size / step;
    __asm__ volatile(
        "0: \n"
        "ld3    { v0.16b, v1.16b, v2.16b }, [%1], #48 \n"
        "mov    v3.16b, v0.16b       \n"
        "mov    v0.16b, v2.16b       \n"
        "mov    v2.16b, v3.16b        \n"
        "st3    { v0.16b, v1.16b, v2.16b }, [%0], #48 \n"
        "subs       %2, %2, #1              \n"
        "bne    0b  \n"
        : "=r"(dst_buf), //%0
        "=r"(src_buf),   //%1
        "=r"(neon_len)   //%2
        : "0"(dst_buf),
        "1"(src_buf),
        "2"(neon_len)
        : "cc", "memory", "v0", "v1", "v2"
    );
#else
    const size_t step = 24;
    vec_size = total_len - total_len % step;
    size_t neon_len = vec_size / step;
    __asm__ volatile(
        "0: \n"
        "vld3.u8  {d0-d2}, [%1]! \n"
        "vswp.u8  d0, d2 \n"
        "vst3.u8  {d0-d2}, [%0]! \n"
        "subs  %2, %2, #1 \n"
        "bne  0b  \n"
        : "=r"(dst_buf), //%0
        "=r"(src_buf),   //%1
        "=r"(neon_len)   //%2
        : "0"(dst_buf),
        "1"(src_buf),
        "2"(neon_len)
        : "cc", "memory", "q0", "q1", "q2"
    );
#endif
    done = vec_size;
#endif
    for (; done<total_len; done+=3) {
        dst_buf[2] = src_buf[0];
        dst_buf[1] = src_buf[1];
        dst_buf[0] = src_buf[2];
        dst_buf += 3;
        src_buf += 3;
    }
}

void rgb2bgr_inplace_naive(unsigned char* buf, size_t height, size_t width)
{
    unsigned char tmp = 0;
    size_t len = height * width *3;
    for (size_t i=0; i<len; i+=3) {
        tmp = buf[i];
        buf[i] = buf[i+2];
        buf[i+2] = tmp;
    }
}

void rgb2bgr_inplace_naive2(unsigned char* buf, size_t height, size_t width)
{
    size_t total_len = height * width * 3;
    size_t step = 12;
    size_t vec_size = total_len - total_len % step;
    uint32_t* p1 = (uint32_t*)buf;
    uint32_t* p2 = p1 + 1;
    uint32_t* p3 = p2 + 1;
    uint32_t v1, v2, v3, nv1, nv2, nv3;
    for (size_t i=0; i<vec_size; i+=step) {
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

    for (size_t done=vec_size; done<total_len; done+=3) {
        unsigned char t = *b1;
        *b1 = *b3;
        *b3 = t;
        b1+=3;
        b3+=3;
    }
}

void rgb2bgr_inplace_asm(unsigned char* buf, size_t height, size_t width)
{
    size_t done = 0;
    size_t total_len = height * width * 3;
#ifdef PIXEL_SIMD_NEON
    size_t vec_size;
#ifdef __aarch64__
    const size_t step = 48;
    vec_size = total_len - total_len % step;
    size_t neon_len = vec_size / step;
    __asm__ volatile(
        "0: \n"
        "ld3    { v0.16b, v1.16b, v2.16b }, [%0] \n"
        "mov    v3.16b, v0.16b       \n"
        "mov    v0.16b, v2.16b       \n"
        "mov    v2.16b, v3.16b        \n"
        "st3    { v0.16b, v1.16b, v2.16b }, [%0], #48 \n"
        "subs       %1, %1, #1              \n"
        "bne    0b  \n"
        : "=r"(buf), //%0
        "=r"(neon_len)   //%1
        : "0"(buf),
        "1"(neon_len)
        : "cc", "memory", "v0", "v1", "v2"
    );
#else
    const size_t step = 24;
    vec_size = total_len - total_len % step;
    size_t neon_len = vec_size / step;
    __asm__ volatile(
        "0: \n"
        "vld3.u8  {d0-d2}, [%0] \n"
        "vswp.u8  d0, d2 \n"
        "vst3.u8  {d0-d2}, [%0]! \n"
        "subs  %1, %1, #1 \n"
        "bne  0b \n"
        : "=r"(buf), //%0
        "=r"(neon_len)   //%1
        : "0"(buf),
        "1"(neon_len)
        : "cc", "memory", "q0", "q1", "q2"
    );
#endif // __aarch64__
    done = vec_size;
#endif // PIXEL_SIMD_NEON
    for ( ; done<total_len; done+=3) {
        unsigned char tmp = buf[0];
        buf[0] = buf[2];
        buf[2] = tmp;
        buf += 3;
    }
}

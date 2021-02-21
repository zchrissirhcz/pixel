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

// void rgb2bgr_inplace_asm(unsigned char* buf, size_t height, size_t width, size_t linebytes) {
//     size_t len = height * width;
//     int segments = len / 8;
//     for (int i=0; i+7<len; i+=8) {
//         asm volatile(
//             "0:                          \n"
//             "vld3.u8 {d0-d2}, [%1]!       \n"
//             "vswp d0, d2                 \n"
//             "vst3.u8 {d0-d2}, [%1]!  \n"
//             : "+r"(buf)
//             :
//             :"cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"
//         );

//         buf += 24;
//     }

//     int remain = len % 8;
//     for (int i=0; i<remain; i++) {
//         unsigned char t = buf[0];
//         buf[0] = buf[2];
//         buf[2] = t;
//         buf += 3;
//     }
// }
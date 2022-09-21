#include "px_image.h"
#include "px_assert.h"

#include <stdlib.h>
#include <string.h>

// copy_make_border_naive( ) 仅支持单通道图像的补边、且只补0；用来形象的说明补边的基本原理
// 对于多通道图像、多种类型补边策略的支持，请用 copy_make_border2_naive( )
void copy_make_border_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right)
{
    int dst_height = src_height + top + bottom;
    int dst_width = src_width + left + right;

    int src_linebytes = src_width * channels * sizeof(unsigned char);
    int dst_linebytes = dst_width * channels * sizeof(unsigned char);
    for (int i=0; i<top; i++) {
        // for (int j=0; j<dst_width; j++){
        // }
        memset(dst+i*dst_linebytes, 0, dst_linebytes);
    }

    int left_bytes = left * sizeof(unsigned char) * channels;
    int right_bytes = right * sizeof(unsigned char) * channels;
    for (int i=0; i<src_height; i++) {
        // for (int j=0; j<left; j++) {
        // }
        int dst_i = i+top;
        memset(dst+dst_i*dst_linebytes, 0, left_bytes);

        int src_i = i;
        memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

        // for (int j=0; j<left; j++) {
        // }
        memset(dst+dst_i*dst_linebytes + left_bytes + src_linebytes, 0, right_bytes);
    }

    for (int i=0; i<bottom; i++) {
        // for (int j=0; j<dst_width; j++){
        // }
        int dst_i = i + top + src_height;
        memset(dst+dst_i*dst_linebytes, 0, dst_linebytes);
    }
}

void copy_make_border2_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right, px_border_type border_type)
{
    assert(border_type==kBorderConstant || 
           border_type==kBorderReflect  ||
           border_type==kBorderWrap     ||
           border_type==kBorderReflect  ||
           border_type==kBorderReplicate);

    int dst_height = src_height + top + bottom;
    int dst_width = src_width + left + right;

    int left_bytes = left * sizeof(unsigned char) * channels;
    int right_bytes = right * sizeof(unsigned char) * channels;

    int src_linebytes = src_width * channels * sizeof(unsigned char);
    int dst_linebytes = dst_width * channels * sizeof(unsigned char);
    for (int i=0; i<top; i++) {
        // for (int j=0; j<left; j++) {
        // }
        if (border_type==kBorderConstant) {
            memset(dst+i*dst_linebytes, 0, dst_linebytes);
        }
        else {
            int src_i = px_border_clip(border_type, -top+i, src_height);

            for (int j=0; j<left; j++) {
                int src_j = px_border_clip(border_type, -left+j, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+i*dst_linebytes+j*channels+k) = *(src+src_i*src_linebytes+src_j*channels+k);
                }
            }

            // 0  -top
            // 1  -top+1
            // 2  -top+2
            
            memcpy(dst+i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

            for (int j=0; j<right; j++) {
                int src_j = px_border_clip(border_type, src_width+j, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+i*dst_linebytes+left_bytes+src_linebytes+j*channels + k) = *(src+src_i*src_linebytes+src_j*channels + k);
                }
            }
        }
    }

    for (int i=0; i<src_height; i++) {
        // for (int j=0; j<left; j++) {
        // }
        int dst_i = i+top;
        if (border_type==kBorderConstant) {
            memset(dst+dst_i*dst_linebytes, 0, left_bytes);
        } else {
            for (int j=0; j<left; j++) {
                int src_j = px_border_clip(border_type, j-left, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+dst_i*dst_linebytes+j*channels + k) = *(src+i*src_linebytes+src_j*channels + k);
                }
            }
        }

        int src_i = i;
        memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

        // for (int j=0; j<left; j++) {
        // }
        if (border_type==kBorderConstant) {
            memset(dst+dst_i*dst_linebytes + left_bytes + src_linebytes, 0, right_bytes);
        } else {
            for (int j=0; j<right; j++) {
                int src_j = px_border_clip(border_type, src_width+j, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+dst_i*dst_linebytes + left_bytes + src_linebytes + j*channels + k) = *(src+i*src_linebytes+src_j*channels + k);
                }
            }
        }
    }

    for (int i=0; i<bottom; i++) {
        // for (int j=0; j<dst_width; j++){
        // }
        int dst_i = i + top + src_height;
        if (border_type==kBorderConstant) {
            memset(dst+dst_i*dst_linebytes, 0, dst_linebytes);
        }
        else {
            int src_i = px_border_clip(border_type, src_height+i, src_height);

            //memset(dst+dst_i*dst_linebytes, 0, left_bytes);
            for (int j=0; j<left; j++) {
                int src_j = px_border_clip(border_type, j-left, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+dst_i*dst_linebytes+j*channels+k) = *(src+src_i*src_linebytes+src_j*channels+k);
                }
            }

            // 0 - size
            // 1 - size+1
            // 2 - size+2
            memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

            //memset(dst+dst_i*dst_linebytes+left_bytes+src_linebytes, 0, right_bytes);
            for (int j=0; j<right; j++) {
                int src_j = px_border_clip(border_type, src_width+j, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+dst_i*dst_linebytes+left_bytes+src_linebytes+j*channels+k) = *(src+src_i*src_linebytes+src_j*channels+k);
                }
            }
        }
    }
}
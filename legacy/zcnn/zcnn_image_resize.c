#include "zcnn_image_resize.h"
#include <math.h>

//==============================================================
// function implementations
//==============================================================
inline FcBGR* fc_bgr_pixels(const image_t* im, const int x, const int y)
{
    const int line_elem = im->width * im->channels;
    return ((FcBGR*)((unsigned char*)im->data + y*line_elem)) + x;
}


//--------------------------------------------------------------
// fc_bgr_resize_nearest
//--------------------------------------------------------------
// @description: nearest neighbor interpolation for image resize
// @param image_tBGR* src: source image
// @param image_tBGR* dst: result image
//--------------------------------------------------------------
//#define FC_BGR_RESIZE_NEAREST_NAIVE //0.2 fps
//#define FC_BGR_RESIZE_NEAREST_OPT1  //0.3 fps
//#define FC_BGR_RESIZE_NEAREST_OPT2 //0.48 fps
//#define FC_BGR_RESIZE_NEAREST_OPT2_2 //0.532 fps
//#define FC_BGR_RESIZE_NEAREST_OPT2_3 //0.495 fps
#define FC_BGR_RESIZE_NEAREST_OPT3 //0.794 fps
//#define FC_BGR_RESIZE_NEAREST_FIXPT1 //0.549 fps
//#define FC_BGR_RESIZE_NEAREST_FIXPT2 //0.98 fps
void bgr_resize_nearest(const image_t* src, image_t* dst) {

    const int src_width = src->width;
    const int src_height = src->height;
    const int dst_height = dst->height;
    const int dst_width = dst->width;
    const int channels = src->channels;
    const int src_line_elem = src->width * src->channels;
    const int dst_line_elem = dst->width * dst->channels;


#ifdef FC_BGR_RESIZE_NEAREST_NAIVE
    for (int h = 0; h < dst->h; h++) {
        int sh = h*src->h / dst->h;
        for (int w = 0; w < dst->w; w++) {
            int sw = w*src->w / dst->w;
            for (int c = 0; c < src->c; c++) {
                int src_idx = sh * src->linebytes + sw * src->c + c;
                int dst_idx = h * dst->linebytes + w * dst->c + c;
                dst->data[dst_idx] = src->data[src_idx];
            }
        }
    }
#elif defined(FC_BGR_RESIZE_NEAREST_OPT1) // accurate
    for (int h = 0; h < dst->h; h++) {
        int sh = h*src->h / dst->h;
        for (int w = 0; w < dst->w; w++) {
            int sw = w*src->w / dst->w;
            *fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
        }
    }
#elif defined(FC_BGR_RESIZE_NEAREST_OPT2) //accurate
    float scale_h = 1.0 * src->h / dst->h;
    float scale_w = 1.0 * src->w / dst->w;
    for (int h = 0; h < dst->h; h++) {
        int sh = h * scale_h;
        for (int w = 0; w < dst->w; w++) {
            int sw = w * scale_w;
            *fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
        }
    }
#elif defined(FC_BGR_RESIZE_NEAREST_OPT2_2) //accurate
    float scale_h = 1.0 * src->h / dst->h;
    float scale_w = 1.0 * src->w / dst->w;

    int* w_table = (int*)malloc(sizeof(int) * dst->w);
    int* h_table = (int*)malloc(sizeof(int) * dst->h);

    int* sh = h_table;
    int* sw = w_table;

    for (int w = 0; w < dst->w; w++) {
        //w_table[w] = w * scale_w;
        *sw++ = w * scale_w;
    }
    for (int h = 0; h < dst->h; h++) {
        //h_table[h] = h * scale_h;
        *sh++ = h * scale_h;
    }

    //int* sh = h_table;
    sh = h_table;
    for (int h = 0; h < dst->h; h++, sh++) {
        //int sh = h * scale_h;
        //int* sw = w_table;
        sw = w_table;
        for (int w = 0; w < dst->w; w++, sw++) {
            //int sw = w * scale_w;
            //*fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
            *fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, *sw, *sh);
        }
    }

    free(w_table);
    free(h_table);
#elif defined(FC_BGR_RESIZE_NEAREST_OPT3) //accurate
    float scale_h = 1.0 * src_height / dst_height;
    float scale_w = 1.0 * src_width / dst_width;
    FcBGR* dst_line = (FcBGR*)dst->data;
    for (int h = 0; h < dst_height; h++) {
        int sh = h * scale_h;
        FcBGR* src_line = ((FcBGR*)((unsigned char*)src->data + src_line_elem*sh));
        for (int w = 0; w < dst_width; w++) {
            int sw = w * scale_w;
            //*fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
            dst_line[w] = src_line[sw];
        }
        //((unsigned char*&)dst_line) += dst->linebytes;
        //((unsigned char*)dst_line) += dst->linebytes;
        unsigned char* tt = (unsigned char*)(void*)dst_line;
        tt += dst_line_elem;
        dst_line = (FcBGR*)(void*)tt;
    }
#elif defined(FC_BGR_RESIZE_NEAREST_FIXPT1) // not very accruate
    unsigned int fix_shift = 16;
    unsigned int scale_h_fix = (src->h << fix_shift) / dst->h;
    unsigned int scale_w_fix = (src->w << fix_shift) / dst->w;
    for (int h = 0; h < dst->h; h++) {
        int sh = ((unsigned int)(h)*scale_h_fix) >> fix_shift;
        for (int w = 0; w < dst->w; w++) {
            int sw = ((unsigned int)(w)*scale_w_fix) >> fix_shift;
            *fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
        }
    }
#elif defined(FC_BGR_RESIZE_NEAREST_OPT2_3) // accruate
    unsigned int fix_shift = 16;
    unsigned int scale_h_fix = (src->h << fix_shift) / dst->h;
    unsigned int scale_w_fix = (src->w << fix_shift) / dst->w;
    float scale_h = src->h * 1.0 / dst->h;
    float scale_w = src->w * 1.0 / dst->w;

    char* plus = (char*)calloc(1, sizeof(char)*dst->w);
    float scalw_w = src->w * 1.0 / dst->w;
    //printf("### ");
    for (int w = 1; w < dst->w; w++) {
        int sw = (w*((src->w<<fix_shift)/dst->w)) >> fix_shift;
        int sw2 = w * scale_w;
        if (sw != sw2) {
            plus[w] = 1;
        }
    }
    //printf("\n");
    //plus[413] = 1;
    //plus[600] = 1;
    //plus[826] = 1;
    //plus[1013] = 1;
    //plus[1052] = 1;

    for (int h = 0; h < dst->h; h++) {
        int sh = ((unsigned int)(h)*scale_h_fix) >> fix_shift;
        char* cur_plus = plus;
        for (int w = 0; w < dst->w; w++) {
            int sw = (((unsigned int)(w)*scale_w_fix) >> fix_shift) + *cur_plus++;
            //int sw2 = w*src->w / dst->w;
            //if (sw != sw2) {
                //printf("-- w=%d, sw=%d, sw2=%d\n", w, sw, sw2);
                //sw = sw2;
            //}
            *fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
        }
    }
#elif defined(FC_BGR_RESIZE_NEAREST_FIXPT2) // not very accruate
    unsigned int scale_h_fix = (src->h << 16) / dst->h;
    unsigned int scale_w_fix = (src->w << 16) / dst->w;
    FcBGR* dst_line = (FcBGR*)dst->data;
    unsigned int sh_fix = 0;
    for (int h = 0; h < dst->h; h++) {
        FcBGR* src_line = ((FcBGR*)((unsigned char*)src->data + src->linebytes*(sh_fix >> 16)));
        //unsigned int sh = (h*scale_h_fix) >> 16;
        unsigned int sw_fix = 0;
        for (int w = 0; w < dst->w; w++) {
            //unsigned int sw = (w*scale_w_fix) >> 16;
            //*fc_bgr_pixels(dst, w, h) = *fc_bgr_pixels(src, sw, sh);
            dst_line[w] = src_line[sw_fix >> 16];
            sw_fix += scale_w_fix;
        }
        sh_fix += scale_h_fix;
        //((unsigned char*&)dst_line) += dst->linebytes;
        ((unsigned char*)dst_line) += dst->linebytes;
    }
#endif
}




//--------------------------------------------------------------
// fc_bgr_resize_bilinear
//--------------------------------------------------------------
// @description: bi-linear interpolation for image resize
// @param image_tBGR* src: source image
// @param image_tBGR* dst: result image
//--------------------------------------------------------------
#define FC_BGR_RESIZE_BILINEAR_NAIVE //0.03 fps
//#define FC_BGR_RESIZE_BILINEAR_OPT1  //  fps
//#define FC_BGR_RESIZE_BILINEAR_FIXPT1
//#define FC_BGR_RESIZE_BILINEAR_FIXPT2
void bgr_resize_bilinear(const image_t* src, image_t* dst) {
#ifdef FC_BGR_RESIZE_BILINEAR_NAIVE
    //   A---- u ---M--------- 1-u -------B
    //   |          |                     |
    //   v          |                     |
    //   |          |                     |
    //   |----------P---------------------|
    //   |          |                     |
    //   |          |                     |
    //   C----------N---------------------D
    //     AM=u, MP=v, MB=1-u, PN=1-v
    //     M=(1-u)A+uB,  N=(1-u)C+uD
    //     P=(1-v)M+vN = (1-v)(1-u)A+(1-v)uB+v(1-u)C+vuD

    const int src_width = src->width;
    const int src_height = src->height;
    const int dst_height = dst->height;
    const int dst_width = dst->width;
    const int channels = src->channels;
    const int src_line_elem = src->width * src->channels;
    const int dst_line_elem = dst->width * dst->channels;
    for (int h = 0; h < dst_height; h++) {
        float sh = (h+0.5) * src_height / dst_height - 0.5;
        float v = sh - floor(sh);
        for (int w = 0; w < dst_width; w++) {
            float sw = (w+0.5) * src_width / dst_width - 0.5;
            float u = sw - floor(sw);

            float wA = (1 - v)*(1 - u);
            float wB = (1 - v)*u;
            float wC = v * (1 - u);
            float wD = v * u;

            for (int c = 0; c < channels; c++) {
                int dst_idx = h * dst_line_elem + w * channels + c;
                
                float sum = 0;

                int A_h = (int)sh;
                int A_w = (int)sw;
                if (A_h>=0 && A_h<src_height && A_w>=0 && A_w<src_width) {
                    int idx_A = A_h*src_line_elem + A_w* channels + c;
                    sum += src->data[idx_A] * wA;
                }

                int B_h = (int)sh;
                int B_w = (int)sw+1;
                if (B_h>=0 && B_h<src_height && B_w>=0 && B_w<src_width) {
                    int idx_B = B_h*src_line_elem + B_w*channels + c;
                    sum += src->data[idx_B] * wB;
                }
                
                int C_h = (int)sh + 1;
                int C_w = (int)sw;
                if (C_h>=0 && C_h<src_height && C_w>=0 && C_w<src_width) {
                    int idx_C = C_h * src_line_elem + C_w * channels + c;
                    sum += src->data[idx_C] * wC;
                }
                
                int D_h = (int)sh + 1;
                int D_w = (int)sw + 1;
                if (D_h >= 0 && D_h < src_height && D_w >= 0 && D_w < src_width) {
                    int idx_D = D_h * src_line_elem + D_w * channels + c;
                    sum += src->data[idx_D] * wD;
                }

                dst->data[dst_idx] = sum;
            }
        }
    }
#endif
}
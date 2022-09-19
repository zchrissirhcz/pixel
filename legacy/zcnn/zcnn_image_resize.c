#include "zcnn_image_resize.h"
#include <math.h>

//==============================================================
// function implementations
//==============================================================
inline FcBGR* fc_bgr_pixels(const px_image_t* im, const int x, const int y)
{
    const int line_elem = im->width * im->channel;
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
void bgr_resize_nearest(const px_image_t* src, px_image_t* dst) {

    const int src_width = src->width;
    const int src_height = src->height;
    const int dst_height = dst->height;
    const int dst_width = dst->width;
    const int channels = src->channel;
    const int src_line_elem = src->width * src->channel;
    const int dst_line_elem = dst->width * dst->channel;


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

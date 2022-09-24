#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
//#include <opencv2/opencv.hpp>

#include "image.h"

namespace fc {

static inline int align_up(int x, int n)
{
    return ((x+n-1)/n)*n;
}

Image make_image(int w, int h, int c, int align)
{
    assert(w>=0 && h>=0 && c>=0 && (align==1 || align==4));

    Image im;
    im.height = h;
    im.width = w;
    im.channel = c;
    im.align = align;
    im.linebytes = align_up(w*c, align);
    im.mass = im.linebytes * im.height;
    im.data = (unsigned char*)malloc(im.mass);
    im.data_mem_type = kHEAP;
    im.mem_type = kSTACK;
    return im;
}

Image* make_image_ptr(int w, int h, int c, int align)
{
    assert(w>=0 && h>=0 && c>=0 && (align==1 || align==4));

    Image* im = (Image*)malloc(sizeof(Image));
    if (im == NULL)
        return NULL;
    im->height = h;
    im->width = w;
    im->channel = c;
    im->align = align;
    im->linebytes = align_up(w*c, align);
    im->mass = im->linebytes * im->height;
    im->data = (unsigned char*)malloc(im->mass);
    im->data_mem_type = kHEAP;
    im->mem_type = kHEAP;
    return im;
}

Image* make_image_ptr(int w, int h, int c, unsigned char* data, int align)
{
    assert(w>=0 && h>=0 && c>=0 && (align==1 || align==4));

    Image* im = (Image*)malloc(sizeof(Image));
    if (im == NULL)
        return NULL;
    im->height = h;
    im->width = w;
    im->channel = c;
    im->align = align;
    im->linebytes = align_up(w*c, align);
    im->mass = im->linebytes * im->height;
    im->data = data;
    im->data_mem_type = kSTACK;
    im->mem_type = kHEAP;
    return im;
}

void free_image(Image* im)
{
    if(im==NULL) return;
    if(im->data!=NULL && im->data_mem_type==kHEAP) {
        free(im->data);
    }
    if(im->mem_type==kHEAP) {
        free(im);
    }
}

void rgb_bgr_swap(Image* im)
{
    if(im==NULL || im->data==NULL ||
            im->height<=0 || im->width<=0) {
        fprintf(stderr, "error! rgb_bgr_swap() input invalid\n");
        return;
    }
    if(im->channel!=3 && im->channel!=4) return; //gray image, ignore

    unsigned char tmp;
    for(int i=0; i<im->height; i++) {
        for(int j=0; j<im->linebytes-im->channel; j+=im->channel) {
            int idx = i*im->linebytes + j;
            tmp = im->data[idx];
            im->data[idx] = im->data[idx+2];
            im->data[idx+2] = tmp;
        }
    }
}

void bgr_to_gray(Image* im, Image* gray)
{
    if(im==NULL || im->data==NULL || gray==NULL || gray->data==NULL ||
            im->channel!=3 || gray->channel!=1) {
        fprintf(stderr, "invalid input for bgr_to_gray()\n");
        return;
    }

    float b, g, r, gray_val;
    int src_idx, dst_idx;
    for(int i=0; i<im->height; i++) {
        for(int j=0; j<im->width; j++) {
            int src_idx = i*im->linebytes + j*3;
            int dst_idx = i*im->width + j;
            b = im->data[src_idx];
            g = im->data[src_idx + 1];
            r = im->data[src_idx + 2];
            gray_val = r*0.299 + g*0.587 + b*0.114;
            gray->data[dst_idx] = (int)(gray_val);
        }
    }
}

void gray_to_bw(Image* gray, Image* bw, unsigned char thresh)
{
    if(gray==NULL || gray->data==NULL || bw==NULL || bw->data==NULL ||
            gray->channel!=1 || bw->channel!=1) {
        fprintf(stderr, "invalid input for gray_to_bw()\n");
        return;
    }
    for(int i=0; i<bw->height; i++) {
        for(int j=0; j<bw->width; j++) {
            int idx = i*bw->width + j;
            if(gray->data[idx]>thresh) {
                bw->data[idx] = 255;
            } else {
                bw->data[idx] = 0;
            }
        }
    }

}

static void resize_nearest(Image* im_src, Image* im_dst)
{
    for(int i=0; i<im_dst->height; i++) {
        for(int j=0; j<im_dst->width; j++) {
            for(int k=0; k<im_src->channel; k++) {
                int src_i = i * im_src->height / im_dst->height ; // i/scale_h;
                int src_j = j * im_src->width / im_dst->width;    // j/scale_w;
                int src_idx = src_i*im_src->linebytes + src_j*im_src->channel + k;
                int dst_idx = i*im_dst->linebytes + j*im_dst->channel + k;
                im_dst->data[dst_idx] = im_src->data[src_idx];
            }
        }
    }
}

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
static void resize_bilinear(Image* im_src, Image* im_dst)
{
    for(int i=0; i<im_dst->height; i++) {
        float src_i = (i+0.5)*im_src->height/im_dst->height-0.5;
        float v = src_i - floor(src_i);
        for(int j=0; j<im_dst->width; j++) {
            float src_j = (j+0.5) * im_src->width / im_dst->width - 0.5;
            float u = src_j - floor(src_j);

            float wA = (1-v)*(1-u);
            float wB = (1-v)*u;
            float wC = v*(1-u);
            float wD = v*u;

            for(int k=0; k<im_dst->channel; k++) {
                int dst_idx = i*im_dst->linebytes + j*im_dst->channel + k;
                float sum = 0;
                int src_idx;
                int xi, xj;

                // A
                xi = (int)(src_i);
                xj = (int)(src_j);
                if (xi>=0 && xi<im_src->height && xj>=0 && xj<im_src->width) {
                    src_idx = xi*im_src->linebytes + xj*im_src->channel + k;
                    sum += im_src->data[src_idx] * wA;
                }

                // B
                xi = (int)(src_i);
                xj = (int)(src_j+1);
                if (xi>=0 && xi<im_src->height && xj>=0 && xj<im_src->width) {
                    src_idx = xi*im_src->linebytes + xj*im_src->channel + k;
                    sum += im_src->data[src_idx] * wB;
                }

                // C
                xi = (int)(src_i+1);
                xj = (int)(src_j);
                if (xi>=0 && xi<im_src->height && xj>=0 && xj<im_src->width) {
                    src_idx = xi*im_src->linebytes + xj*im_src->channel + k;
                    sum += im_src->data[src_idx] * wC;
                }

                // D
                xi = (int)(src_i+1);
                xj = (int)(src_j+1);
                if (xi>=0 && xi<im_src->height && xj>=0 && xj<im_src->width) {
                    src_idx = xi*im_src->linebytes + xj*im_src->channel + k;
                    sum += im_src->data[src_idx] * wD;
                }

                im_dst->data[dst_idx] = sum;
            }
        }
    }
}

void resize(Image* im_src, Image* im_dst, ResizeMethod method)
{
    if(im_src==NULL || im_dst==NULL || im_src->data==NULL || im_dst->data==NULL ||
            im_src->channel!=im_dst->channel) {
        fprintf(stderr, "error! resize() input invalid\n");
        return;
    }
#if 0
    stbir_resize_uint8(im_src->data, im_src->width, im_src->height, 0,
            im_dst->data, im_dst->width, im_dst->height, 0, im_dst->channel);
#endif

    if(method==kNEAREST) {
        resize_nearest(im_src, im_dst);
    }
    else if(method==kBILINEAR) {
        resize_bilinear(im_src, im_dst);
    }
}

Image* histgram(Image* im)
{
    bool is_gray = false;
    if(im->channel==1) {
        is_gray = true;
    }
    Image* gray;
    if(is_gray) {
        gray = im;
    } else {
        gray = make_image_ptr(im->width, im->height, 1);
        bgr_to_gray(im, gray);
    }

    int len = gray->height * gray->width;
    int gray_lut[256] = {0};
    for(int i=0; i<len; i++) {
        gray_lut[gray->data[i]]++;
    }
    // release gray
    if(!is_gray) {
        free(gray->data);
        free(gray);
    }

    int max_gray = 0;
    for(int i=0; i<256; i++) {
        if(gray_lut[i]>max_gray) max_gray=gray_lut[i];
    }

    int width = 256;
    int height = 100;
    int channel = 3;
    Image* hist = make_image_ptr(width, height, channel);
    memset(hist->data, 255, hist->mass);

    int linebytes = hist->linebytes;
    for(int i=0; i<hist->width; i++) {
        int sum = gray_lut[i] * hist->height / max_gray;
        for(int j=0; j<sum; j++) {
            int idx = i*hist->channel + (hist->height-1-j)*linebytes;
            hist->data[idx] = 128;
            hist->data[idx+1] = 128;
            hist->data[idx+2] = 128;
        }
    }
    return hist;
}

} // namespace fc

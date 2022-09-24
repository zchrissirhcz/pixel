#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

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

#include "imdraw.h"
#include <algorithm>
#include <ctype.h>
#include "glyph.h"

namespace fc {


//square root by Carmack
static float fast_sqrt(float number)
{
    int i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( int * ) &y;
    i  = 0x5f375a86 - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );
    return number*y;
}

static void circle(unsigned char* src, int w, int h, int channels, int stride, int dx, int dy, int dr, unsigned char* dcolor)
{
    int up_limit = std::max(dy-dr, 0);
    int down_limit = std::min(h, dy+dr);
    //unsigned char ddcolor[channels*2*dr];
    unsigned char* ddcolor = (unsigned char*)malloc(channels * 2 * dr);
    if (ddcolor == NULL)
        return;
    auto size = sizeof(unsigned char);
    double ddr = dr*dr;
    int offset = channels*size;
    for(int i=0; i<2*dr; i++) {
        memcpy(ddcolor+i*channels, dcolor, offset);
    }

    for(int y=up_limit; y<down_limit; y++) {
        int ddx = sqrt(ddr-(y-dy)*(y-dy));
        //int ddx = fast_sqrt(ddr-(y-dy)*(y-dy));
        int left_limit = std::max(dx-ddx, 0);
        int right_limit = std::min(w, dx+ddx);

        memcpy(src+y*stride+left_limit*channels, ddcolor, (right_limit-left_limit)*offset);
    }
    free(ddcolor);
}

void circle(Image* im, const Point* center, int radius, Color color)
{
    unsigned char dcolor[3] = {color.b, color.g, color.r};
    circle(im->data, im->width, im->height, im->channel, im->linebytes, center->x, center->y, radius, dcolor);
}

static void draw_solid_rect(Image* im, int dx1, int dy1, int dx2, int dy2, Color color)
{
    int linebytes = im->linebytes;
    int channel = im->channel;
    for (int h = dy1; h <= dy2; h++) {
        for (int w = dx1; w <= dx2; w++) {
            im->data[h*linebytes + w*channel] = color.b;
            im->data[h*linebytes + w*channel + 1] = color.g;
            im->data[h*linebytes + w*channel + 2] = color.r;
        }
    }
}

void rectangle(Image* im, Rect r, Color color, int thickness)
{
    //draw rect
    int dx1, dx2, dy1, dy2;
    int half_thick_small = thickness / 2;
    int half_thick_big = thickness - thickness / 2;

    // up horizontal line
    dy1 = r.y1 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = r.y1 + half_thick_big;
    if (dy2 >= im->height - 1) {
        dy2 = im->height - 1;
    }

    dx1 = r.x1 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = r.x2 + half_thick_big;
    if (dx2 >= im->width - 1) {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // down horizontal line
    dy1 = r.y2 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = r.y2 + half_thick_big;
    if (dy2 >= im->height - 1) {
        dy2 = im->height - 1;
    }

    dx1 = r.x1 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = r.x2 + half_thick_big;
    if (dx2 >= im->width - 1) {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // left vertical line
    dy1 = r.y1 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = r.y2 + half_thick_big;
    if (dy2 >= im->height - 1) {
        dy2 = im->height - 1;
    }
    dx1 = r.x1 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = r.x1 + half_thick_big;
    if (dx2 >= im->width - 1) {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // right vertical line
    dy1 = r.y1 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = r.y2 + half_thick_big;
    if (dy2 >= im->height - 1) {
        dy2 = im->height - 1;
    }

    dx1 = r.x2 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = r.x2 + half_thick_big;
    if (dx2 >= im->width - 1) {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);
}

static void draw_text(fc::Image* im, const unsigned char* glyph, int font_size, int h_start,
                      int w_start, unsigned char b, unsigned char g, unsigned char r) {
    int linebytes = im->linebytes;
    for (int i = h_start; i < h_start + font_size; i++) {
        for (int j = w_start; j < w_start + font_size; j++) {
            int src_idx = (i - h_start) * font_size + (j - w_start);
            int dst_idx = i * linebytes + j * 3;
            if (glyph[src_idx] == 1) {
                im->data[dst_idx] = b;
                im->data[dst_idx + 1] = g;
                im->data[dst_idx + 2] = r;
            }
        }
    }
}

static const unsigned char* get_glyph(int font_size, const char ch) {
    if (font_size != 10 && font_size != 15) {
        fprintf(stderr, "Not supported font size %d. Only 10 and 15 are supported\n", font_size);
        return NULL;
    }
    if (!isdigit(ch) && !islower(ch) && ch!=' ') {
        fprintf(stderr, "Invalid text. Only digit(0-9) and lowercase letter(a-z) supported\n");
        return NULL;
    }

    static const unsigned char* glyph_10_lst[] = {
        glyph_10_0, glyph_10_1, glyph_10_2, glyph_10_3, glyph_10_4, glyph_10_5,
        glyph_10_6, glyph_10_7, glyph_10_8, glyph_10_9, glyph_10_a, glyph_10_b,
        glyph_10_c, glyph_10_d, glyph_10_e, glyph_10_f, glyph_10_g, glyph_10_h,
        glyph_10_i, glyph_10_j, glyph_10_k, glyph_10_l, glyph_10_m, glyph_10_n,
        glyph_10_o, glyph_10_p, glyph_10_q, glyph_10_r, glyph_10_s, glyph_10_t,
        glyph_10_u, glyph_10_v, glyph_10_w, glyph_10_x, glyph_10_y, glyph_10_z,
        glyph_10_space
    };

    static const unsigned char* glyph_15_lst[] = {
        glyph_15_0, glyph_15_1, glyph_15_2, glyph_15_3, glyph_15_4, glyph_15_5,
        glyph_15_6, glyph_15_7, glyph_15_8, glyph_15_9, glyph_15_a, glyph_15_b,
        glyph_15_c, glyph_15_d, glyph_15_e, glyph_15_f, glyph_15_g, glyph_15_h,
        glyph_15_i, glyph_15_j, glyph_15_k, glyph_15_l, glyph_15_m, glyph_15_n,
        glyph_15_o, glyph_15_p, glyph_15_q, glyph_15_r, glyph_15_s, glyph_15_t,
        glyph_15_u, glyph_15_v, glyph_15_w, glyph_15_x, glyph_15_y, glyph_15_z,
        glyph_15_space
    };

    int idx = -233;

    if (isdigit(ch)) {
        idx = ch - '0';
    } else if (islower(ch)) {
        idx = ch - 'a' + 10;
    } else if (ch==' ') {
        idx = 10+26;
    }

    if (font_size == 10) {
        return glyph_10_lst[idx];
    }
    else if (font_size == 15) {
        return glyph_15_lst[idx];
    }
    else {
        fprintf(stderr, "invalid font_size\n");
        return NULL;
    }
}


void put_text(fc::Image* im, fc::Point org, fc::Color color, int font_size, const char* text) {
    if (font_size != 10 && font_size != 15) {
        fprintf(stderr, "Not supported font size %d. Only 10 and 15 are supported\n", font_size);
        return;
    }
    int len = strlen(text);
    bool valid = true;
    for (int i = 0; i < len; i++) {
        if (!isdigit(text[i]) && !islower(text[i]) && text[i]!=' ') {
            valid = false;
        }
    }
    if (!valid) {
        fprintf(stderr, "Invalid text. Only digit(0-9) and lowercase letter(a-z) supported\n");
        return;
    }

    int h_start = org.y;
    int w_start;
    for (int i = 0; i < len; i++) {
        const unsigned char* glyph = get_glyph(font_size, text[i]);
        w_start = org.x + font_size * i;
        draw_text(im, glyph, font_size, h_start, w_start, color.b, color.g, color.r);
    }
}

} // namespace fc
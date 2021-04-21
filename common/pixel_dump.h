#ifndef PIXEL_DUMP_H
#define PIXEL_DUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C"{
#endif

// save float array as binary file
// first store its len, then stores its data
static int save_array_f32_as_bin(float* data, int len, const char* filename);

// load float array from binary file, together with array length
static float* load_array_f32_as_bin(int* _len, const char* filename);

#ifdef __cplusplus
}
#endif


//----------------------------------------------------------------------

int save_array_f32_as_bin(float* data, int len, const char* filename) {
    FILE* fout = fopen(filename, "wb");
    fwrite(&len, sizeof(int), 1, fout);
    fwrite(data, sizeof(float), len, fout);
    fclose(fout);
    
    return 0;
}

float* load_array_f32_as_bin(int* _len, const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    int len;
    fread(&len, sizeof(int), 1, fin);
    float* data = (float*)malloc(sizeof(float)*len);
    fread(data, sizeof(float), len, fin);

    *_len = len;
    return data;
}

#endif // PIXEL_DUMP_H
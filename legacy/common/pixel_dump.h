#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

// save array's length and elements
static int save_array_f32_to_binary_file(float* data, int len, const char* filename);

// load array's length and elements
static float* load_array_f32_from_binary_file(int* _len, const char* filename);

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------

int save_array_f32_to_binary_file(float* data, int len, const char* filename)
{
    FILE* fout = fopen(filename, "wb");
    fwrite(&len, sizeof(int), 1, fout);
    fwrite(data, sizeof(float), len, fout);
    fclose(fout);
    
    return 0;
}

float* load_array_f32_from_binary_file(int* _len, const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    int len;
    fread(&len, sizeof(int), 1, fin);
    float* data = (float*)malloc(sizeof(float)*len);
    fread(data, sizeof(float), len, fin);

    *_len = len;
    return data;
}

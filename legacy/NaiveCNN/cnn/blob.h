#pragma once

#include "px_typebase.h"
#include "px_cnn.h"
#include <stdio.h>

void clear_blob1d(float* data, int len);
void clear_blob2d(float** data, px_size_t size);
void clear_blob3d(float*** data, px_cube_dim_t cube_dim);
void clear_blob4d(float**** data, px_tensor_dim_t tensor_dim);

float* create_blob1d(int len);
float** create_blob2d(px_size_t size);
float*** create_blob3d(px_cube_dim_t cube_dim);
float**** create_blob4d(px_tensor_dim_t tensor_dim);

void destroy_blob1d(float* data);
void destroy_blob2d(float** data, px_size_t size);
void destroy_blob3d(float*** data, px_cube_dim_t cube_dim);
void destroy_blob4d(float**** data, px_tensor_dim_t tensor_dim);

void save_blob1d_to_file(float* data, int len, FILE* fout);
void save_blob2d_to_file(float** data, px_size_t size, FILE* fout);
void save_blob3d_to_file(float*** data, px_cube_dim_t cube_dim, FILE* fout);
void save_blob4d_to_file(float**** data, px_tensor_dim_t tensor_dim, FILE* fout);
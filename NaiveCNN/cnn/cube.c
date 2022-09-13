#include "cnn.h"
#include "blob.h"

cube_t* create_cube(px_cube_dim_t cube_dim)
{
    float*** data = create_blob3d(cube_dim);
    cube_t* cube = (cube_t*)malloc(sizeof(cube_t));
    cube->data = data;
    cube->channel = cube_dim.channel;
    cube->height = cube_dim.height;
    cube->width = cube_dim.width;
    return cube;
}

void clear_cube(cube_t* cube)
{
    px_cube_dim_t cube_dim = px_create_cube_dim(cube->channel, cube->height, cube->width);
    clear_blob3d(cube->data, cube_dim);
}

void save_cube_to_file(cube_t* cube, FILE* fout)
{
    px_cube_dim_t cube_dim = px_create_cube_dim(cube->channel, cube->height, cube->width);
    save_blob3d_to_file(cube->data, cube_dim, fout);
}

void destroy_cube(cube_t* cube)
{
    free(cube->data);
    free(cube);
}
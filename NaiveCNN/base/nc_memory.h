#ifndef NC_MEMORY_H
#define NC_MEMORY_H

#include "nc_types.h"

uint nc_align_up(uint x, uint n) {
    return ((x+n-1)/n)*n;
}

#endif


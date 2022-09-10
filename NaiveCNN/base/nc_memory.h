#pragma once

#include "nc_types.h"

uint nc_align_up(uint x, uint n) {
    return ((x+n-1)/n)*n;
}


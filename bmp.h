#pragma once

#include "image.h"
#include <stdio.h>

static inline size_t ppm_from_ppi(size_t ppi)
{
    return ppi / 0.0254;
}

int bmp_write(struct rgb_image *image, size_t pixel_density, FILE *file);

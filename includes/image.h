#pragma once

#include "utils/alloc.h"

#include <stddef.h>
#include <stdint.h>

struct __attribute__((packed)) rgb_pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct rgb_image
{
    size_t width;
    size_t height;
    struct rgb_pixel data[];
};

struct rgb_image *rgb_image_alloc(size_t width, size_t height);
void rgb_image_clear(struct rgb_image *image, const struct rgb_pixel *pix);

static inline void rgb_image_set(struct rgb_image *image, size_t x, size_t y,
                                 struct rgb_pixel pixel)
{
    image->data[image->width * y + x] = pixel;
}

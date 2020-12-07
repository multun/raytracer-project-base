#include <string.h>

#include "image.h"

struct rgb_image *rgb_image_alloc(size_t width, size_t height)
{
    size_t alloc_size = sizeof(struct rgb_image);
    alloc_size += sizeof(struct rgb_pixel) * width * height;

    struct rgb_image *res = xalloc(alloc_size);
    res->width = width;
    res->height = height;
    return res;
}

void rgb_image_clear(struct rgb_image *image, const struct rgb_pixel *pix)
{
    for (size_t y = 0; y < image->height; y++)
        for (size_t x = 0; x < image->width; x++)
            memcpy(&image->data[image->width * y + x], pix, sizeof(*pix));
}

#include "bmp.h"
#include "image.h"
#include "utils/align.h"
#include "utils/alloc.h"
#include "utils/static_assert.h"

#include <stdint.h>

enum bmp_compression
{
    /* no compression */
    BI_RGB = 0,
};

struct __attribute__((packed)) bmp_file_header
{
    /* Bitmap File Header */
    char signature[2];
    uint32_t file_size;
    uint32_t unused;
    uint32_t data_file_offset;
};

struct __attribute__((packed)) bmp_bim
{
    /* Bitmap Information Header */
    uint32_t header_size;
    uint32_t width;
    uint32_t height;

    uint16_t planes; // must be one
    uint16_t bits_per_pixel;

    uint32_t compression_method;
    /* size of the compressed image */
    uint32_t image_size;
    uint32_t pixels_per_m_x;
    uint32_t pixels_per_m_y;
    uint32_t colors_used;
    uint32_t important_colors; // 0 if all matter;
};

struct __attribute__((packed)) bmp_header
{
    struct bmp_file_header file;
    struct bmp_bim bim;
};

STATIC_ASSERT(bmp_header_size, sizeof(struct bmp_header) == 54);

int bmp_write(struct rgb_image *image, size_t pixel_density, FILE *file)
{
    size_t unpadded_stride = image->width * sizeof(struct rgb_pixel);
    size_t stride = align_up(unpadded_stride, 4);
    size_t data_size = stride * image->height;
    struct bmp_header header;
    header.file = (struct bmp_file_header){
        .signature[0] = 'B',
        .signature[1] = 'M',
        .file_size = sizeof(header) + data_size,
        .data_file_offset = sizeof(header),
    };

    header.bim = (struct bmp_bim){
        .header_size = sizeof(struct bmp_bim),
        .width = image->width,
        .height = image->height,

        .planes = 1,
        .bits_per_pixel = 24,

        .compression_method = BI_RGB,
        .image_size = 0, // only applies for compressed images

        .pixels_per_m_x = pixel_density,
        .pixels_per_m_y = pixel_density,
        .colors_used = 0, // no color palette
        .important_colors = 0, // obsolete and ignored field
    };

    fwrite(&header, sizeof(header), 1, file);

    size_t padding_size = stride - unpadded_stride;
    char padding[4] = {0};

    for (size_t line_i = 0; line_i < image->height; line_i++)
    {
        // bmp images are written from the bottom up
        struct rgb_pixel *line = &image->data[image->width * line_i];
        for (size_t col = 0; col < image->width; col++)
        {
            struct rgb_pixel *pixel = &line[col];
            uint8_t out_data[3];
            out_data[0] = pixel->b;
            out_data[1] = pixel->g;
            out_data[2] = pixel->r;
            fwrite(out_data, 3, 1, file);
        }
        fwrite(padding, padding_size, 1, file);
    }
    return 0;
}

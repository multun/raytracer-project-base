#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "image.h"
#include "vec3.h"

struct sphere
{
    struct vec3 center;
    double radius;
};

struct ray
{
    struct vec3 source;
    struct vec3 direction;
};

struct camera
{
    struct vec3 center;
    struct vec3 forward;
    struct vec3 up;

    size_t width;
    size_t height;

    double pixel_size;

    double fov; // angle in radians
};

void camera_cast_ray(struct ray *ray, const struct camera *camera, size_t x,
                     size_t y)
{
    struct vec3 right = vec3_cross(&camera->forward, &camera->up);
    // x_coeff = (x - width / 2) * pixel_size
    double x_coeff
        = ((double)x - (double)camera->width / 2) * camera->pixel_size;
    // y_coeff = (y - height / 2) * pixel_size
    double y_coeff
        = ((double)y - (double)camera->height / 2) * camera->pixel_size;
    // right_offset = right * x_coeff
    struct vec3 right_offset = vec3_mul(&right, x_coeff);
    // up_offset = up * y_coeff
    struct vec3 up_offset = vec3_mul(&camera->up, y_coeff);
    // offset = right_offset + up_offset
    struct vec3 offset = vec3_add(&right_offset, &up_offset);
    // ray->source = center + offset
    ray->source = vec3_add(&camera->center, &offset);

    double focal_distance = ((double)camera->width / 2) * camera->pixel_size
                            / tan(camera->fov / 2);
    struct vec3 vantage_point_offset
        = vec3_mul(&camera->forward, -focal_distance);
    struct vec3 vantage_point
        = vec3_add(&vantage_point_offset, &camera->center);
    ray->direction = vec3_sub(&ray->source, &vantage_point);
    vec3_normalize(&ray->direction);
}

bool sphere_ray_intersect(const struct ray *ray, const struct sphere *sphere)
{
    struct vec3 hypothenuse = vec3_sub(&sphere->center, &ray->source);
    double hyp_len = vec3_length(&hypothenuse);
    double projection = vec3_dot(&hypothenuse, &ray->direction);
    double d = sqrt(hyp_len * hyp_len - projection * projection);

    bool has_intersection = d <= sphere->radius;
    return has_intersection;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(1, "Usage: OUTPUT.bmp");

    struct rgb_image *image = rgb_image_alloc(100, 100);
    struct rgb_pixel bg_color = {0};
    rgb_image_clear(image, &bg_color);

    struct sphere sphere = {
        .center = {0, 10, 0},
        .radius = 4,
    };

    struct camera camera = {
        .center = {0, 0, 0},
        .forward = {0, 1, 0},
        .up = {0, 0, 1},
        .pixel_size = 0.1,
        .width = image->width,
        .height = image->height,
        // convert to radians
        .fov = 80 /* degrees */ * M_PI / (360 / 2),
    };

    for (size_t y = 0; y < image->height; y++)
        for (size_t x = 0; x < image->width; x++)
        {
            struct ray ray;
            camera_cast_ray(&ray, &camera, x, y);

            // if there's no intersection between the ray and this object, skip
            // it
            if (!sphere_ray_intersect(&ray, &sphere))
                continue;

            struct rgb_pixel white = {255, 255, 255};
            rgb_image_set(image, x, y, white);
        }

    FILE *fp = fopen(argv[1], "w");
    if (fp == NULL)
        err(1, "failed to open the output file");

    return bmp_write(image, ppm_from_ppi(80), fp);
}

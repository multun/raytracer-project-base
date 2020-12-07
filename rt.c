#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "image.h"
#include "vec3.h"
#include "camera.h"
#include "scene.h"


struct rgb_pixel normal_color(const struct vec3 *normal)
{
    struct rgb_pixel res;
    //       -1 => 1
    // + 1   0 => 2
    // / 2   0 => 1
    // * 255 0 => 255
    //       0 => 255

    double nx = (normal->x + 1.) / 2.;
    double ny = (normal->y + 1.) / 2.;
    double nz = (normal->z + 1.) / 2.;
    res.r = nx * 255;
    res.g = ny * 255;
    res.b = nz * 255;
    return res;
}


/*
** The color of a light is encoded inside a float, from 0 to +inf,
** where 0 is no light, and +inf a lot more light. Unfortunately,
** regular images can't hold such a huge range, and each color channel
** is usualy limited to [0,255]. This function does the (lossy) translation
** by mapping the float [0,1] range to [0,255]
*/
static inline uint8_t translate_light_component(double light_comp)
{
    if (light_comp < 0.)
        light_comp = 0.;
    if (light_comp > 1.)
        light_comp = 1.;

    return light_comp * 255;
}


/*
** Converts an rgb floating point light color to 24 bit rgb.
*/
struct rgb_pixel rgb_color_from_light(const struct vec3 *light)
{
    struct rgb_pixel res;
    res.r = translate_light_component(light->x);
    res.g = translate_light_component(light->y);
    res.b = translate_light_component(light->z);
    return res;
}


int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(1, "Usage: OUTPUT.bmp");

    struct rgb_image *image = rgb_image_alloc(1920, 1080);
    struct rgb_pixel bg_color = {0};
    rgb_image_clear(image, &bg_color);

    struct object_vect objects;
    object_vect_init(&objects, 42);

    double cam_width = 10;
    double cam_height = cam_width * image->height / image->width;

    struct camera camera = {
        .center = {0, 0, 0},
        .forward = {0, 1, 0},
        .up = {0, 0, 1},
        .width = cam_width,
        .height = cam_height,
        .focal_distance = focal_distance_from_fov(cam_width, 80),
    };

    struct vec3 light_color = {1, 1, 0}; // yellow
    struct vec3 light_direction = {-1, 1, 1};
    double light_intensity = 5;

    vec3_normalize(&light_direction);

    for (size_t y = 0; y < image->height; y++)
        for (size_t x = 0; x < image->width; x++)
        {
            struct ray ray;

            double cam_x = ((double)x / image->width) - 0.5;
            double cam_y = ((double)y / image->height) - 0.5;

            camera_cast_ray(&ray, &camera, cam_x, cam_y);

            struct object_intersection best_intersection;
            double best_intersection_dist = INFINITY;

            for (size_t i = 0; i < object_vect_size(&objects); i++)
            {
                struct object *obj = object_vect_get(&objects, i);
                struct object_intersection intersection;
                // if there's no intersection between the ray and this object,
                // skip
                double intersection_dist = obj->intersect(&intersection, obj, &ray);
                if (intersection_dist >= best_intersection_dist)
                    continue;

                best_intersection_dist = intersection_dist;
                best_intersection = intersection;
            }

            // if the intersection distance is infinite, do not shade the pixel
            if (isinf(best_intersection_dist))
                continue;

            // a coefficient teaking how much diffuse light to add
            double diffuse_kn = 0.20;
            struct vec3 surface_color = {0.75, 0.125, 0.125};

            struct vec3 light = vec3_mul(&light_color, light_intensity);
            struct vec3 diffuse_light_color
                = vec3_mul_vec(&light, &surface_color);

            // compute the diffuse lighting contribution by applying the cosine
            // law
            double diffuse_intensity
                = -vec3_dot(&best_intersection.location.normal, &light_direction);
            if (diffuse_intensity < 0)
                diffuse_intensity = 0;

            struct vec3 diffuse_contribution
                = vec3_mul(&diffuse_light_color, diffuse_intensity * diffuse_kn);

            // compute the specular reflection contribution

            // these two should be material specific, but aren't to keep
            // things simple
            // how wide the reflection is
            double spec_n = 10;
            // how much the specular reflection contributes
            double spec_ks = 0.20;

            struct vec3 light_reflection_dir
                = vec3_reflect(&light_direction, &best_intersection.location.normal);
            struct vec3 specular_contribution = {0};
            // computes how much the reflection goes in the direction of the
            // camera
            double light_reflection_proj
                = -vec3_dot(&light_reflection_dir, &ray.direction);
            if (light_reflection_proj < 0.0)
                light_reflection_proj = 0.0;
            else
            {
                double spec_coeff
                    = pow(light_reflection_proj, spec_n) * spec_ks;
                specular_contribution = vec3_mul(&light_color, spec_coeff);
            }

            double ambient_intensity = 0.1;
            struct vec3 ambient_contribution = vec3_mul(&surface_color, ambient_intensity);

            struct vec3 pix_color = {0};
            pix_color = vec3_add(&pix_color, &ambient_contribution);
            pix_color = vec3_add(&pix_color, &diffuse_contribution);
            pix_color = vec3_add(&pix_color, &specular_contribution);
            rgb_image_set(image, x, y, rgb_color_from_light(&pix_color));
        }

    FILE *fp = fopen(argv[1], "w");
    if (fp == NULL)
        err(1, "failed to open the output file");

    return bmp_write(image, ppm_from_ppi(80), fp);
}

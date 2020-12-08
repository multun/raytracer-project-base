#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "camera.h"
#include "image.h"
#include "phong_material.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "vec3.h"

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

static void build_test_scene(struct scene *scene, double aspect_ratio)
{
    // create a sample red material
    struct phong_material *red_material = zalloc(sizeof(*red_material));
    phong_material_init(red_material);
    red_material->surface_color = (struct vec3){0.75, 0.125, 0.125};
    red_material->diffuse_Kn = 0.2;
    red_material->spec_n = 10;
    red_material->spec_Ks = 0.2;
    red_material->ambient_intensity = 0.1;

    // create a single sphere with the above material, and add it to the scene
    struct sphere *sample_sphere
        = sphere_create((struct vec3){0, 10, 0}, 4, &red_material->base);
    object_vect_push(&scene->objects, &sample_sphere->base);

    // go the same with a triangle
    // points are listed counter-clockwise
    //     a
    //    /|
    //   / |
    //  b--c
    struct vec3 points[3] = {
        {6, 10, 1}, // a
        {5, 10, 0}, // b
        {6, 10, 0}, // c
    };

    struct triangle *sample_triangle
        = triangle_create(points, &red_material->base);
    object_vect_push(&scene->objects, &sample_triangle->base);

    // setup the scene lighting
    scene->light_intensity = 5;
    scene->light_color = (struct vec3){1, 1, 0}; // yellow
    scene->light_direction = (struct vec3){-1, 1, -1};
    vec3_normalize(&scene->light_direction);

    // setup the camera
    double cam_width = 10;
    double cam_height = cam_width / aspect_ratio;

    scene->camera = (struct camera){
        .center = {0, 0, 0},
        .forward = {0, 1, 0},
        .up = {0, 0, 1},
        .width = cam_width,
        .height = cam_height,
        .focal_distance = focal_distance_from_fov(cam_width, 80),
    };

    // release the reference to the material
    material_put(&red_material->base);
}

/* For all the pixels of the image, try to find the closest object
** intersecting the camera ray. If an object is found, shade the pixel to
** find its color.
*/
static void render_pixel(struct rgb_image *image, struct scene *scene, size_t x,
                         size_t y)
{
    // find the position of the current pixel in the image plane
    // camera_cast_ray takes camera relative positions, from -0.5 to 0.5 for
    // both axis
    double cam_x = ((double)x / image->width) - 0.5;
    double cam_y = ((double)y / image->height) - 0.5;

    // find the starting point and direction of this ray
    struct ray ray;
    camera_cast_ray(&ray, &scene->camera, cam_x, cam_y);

    // we will now try to find the closest object in the scene
    // intersecting this ray
    struct object_intersection closest_intersection;
    double closest_intersection_dist = INFINITY;

    for (size_t i = 0; i < object_vect_size(&scene->objects); i++)
    {
        struct object *obj = object_vect_get(&scene->objects, i);
        struct object_intersection intersection;
        // if there's no intersection between the ray and this object, skip it
        double intersection_dist = obj->intersect(&intersection, obj, &ray);
        if (intersection_dist >= closest_intersection_dist)
            continue;

        closest_intersection_dist = intersection_dist;
        closest_intersection = intersection;
    }

    // if the intersection distance is infinite, do not shade the pixel
    if (isinf(closest_intersection_dist))
        return;

    struct material *mat = closest_intersection.material;
    struct vec3 pix_color
        = mat->shade(mat, &closest_intersection.location, scene, &ray);
    rgb_image_set(image, x, y, rgb_color_from_light(&pix_color));
}

int main(int argc, char *argv[])
{
    int rc;

    if (argc != 2)
        errx(1, "Usage: OUTPUT.bmp");

    // initialize the frame buffer (the buffer that will store the result of the
    // rendering)
    struct rgb_image *image = rgb_image_alloc(1920, 1080);

    // set all the pixels of the image to black
    struct rgb_pixel bg_color = {0};
    rgb_image_clear(image, &bg_color);

    double aspect_ratio = (double)image->width / image->height;

    // build the scene
    struct scene scene;
    scene_init(&scene);
    build_test_scene(&scene, aspect_ratio);

    // render all pixels
    for (size_t y = 0; y < image->height; y++)
        for (size_t x = 0; x < image->width; x++)
            render_pixel(image, &scene, x, y);

    // write the rendered image to a bmp file
    FILE *fp = fopen(argv[1], "w");
    if (fp == NULL)
        err(1, "failed to open the output file");

    rc = bmp_write(image, ppm_from_ppi(80), fp);
    fclose(fp);

    // release resources
    scene_destroy(&scene);
    free(image);
    return rc;
}

#pragma once

#include "image.h"
#include "vec3.h"

#include <stdint.h>

#define GAMMA_COEFF 2.2

/*
** The color of a light is encoded inside a float, from 0 to +inf,
** where 0 is no light, and +inf a lot more light. Unfortunately,
** regular images can't hold such a huge range, and each color channel
** is usualy limited to [0,255]. This function does the (lossy) translation
** by mapping the float [0,1] range to [0,255]
*/
static inline uint8_t gamma_encode(double light_comp)
{
    if (light_comp < 0.)
        light_comp = 0.;
    if (light_comp > 1.)
        light_comp = 1.;

    return pow(light_comp, 1 / GAMMA_COEFF) * 255;
}

/*
** Turns a rgb color component into a linear floating point intensity.
*/
static inline double gamma_decode(uint8_t component)
{
    return pow((double)component / 255, GAMMA_COEFF);
}

/*
** Converts an rgb floating point light color to 24 bit rgb.
*/
static inline struct rgb_pixel rgb_color_from_light(const struct vec3 *light)
{
    struct rgb_pixel res;
    res.r = gamma_encode(light->x);
    res.g = gamma_encode(light->y);
    res.b = gamma_encode(light->z);
    return res;
}

/*
** Converts 24 bit rgb to floating point light color.
*/
static inline struct vec3 light_from_rgb_color(uint8_t r, uint8_t g, uint8_t b)
{
    struct vec3 res;
    res.x = gamma_decode(r);
    res.y = gamma_decode(g);
    res.z = gamma_decode(b);
    return res;
}

#pragma once

#include "object.h"
#include "vec3.h"

#include <stddef.h>

struct phong_material
{
    // the base class structure
    struct material base;

    struct vec3 surface_color;
    // the diffuse light intensity coefficient
    double diffuse_Kn;

    // the specular exponential focus coefficient
    double spec_n;
    // the specular intensity coefficient
    double spec_Ks;

    // the specular intensity coefficient
    double ambient_intensity;
};

struct vec3 phong_metarial_shade(const struct material *material,
                                 const struct intersection *inter,
                                 const struct scene *scene,
                                 const struct ray *ray);

static inline void phong_material_init(struct phong_material *mat)
{
    material_init(&mat->base, NULL, phong_metarial_shade);
}

#include "phong_material.h"
#include "scene.h"

struct vec3 phong_metarial_shade(const struct material *base_material,
                                 const struct intersection *inter,
                                 const struct scene *scene,
                                 const struct ray *ray)
{
    const struct phong_material *mat
        = (const struct phong_material *)base_material;

    // a coefficient teaking how much diffuse light to add
    struct vec3 light = vec3_mul(&scene->light_color, scene->light_intensity);
    struct vec3 diffuse_light_color = vec3_mul_vec(&light, &mat->surface_color);

    // compute the diffuse lighting contribution by applying the cosine
    // law
    double diffuse_intensity
        = -vec3_dot(&inter->normal, &scene->light_direction);
    if (diffuse_intensity < 0)
        diffuse_intensity = 0;

    struct vec3 diffuse_contribution
        = vec3_mul(&diffuse_light_color, diffuse_intensity * mat->diffuse_Kn);

    // compute the specular reflection contribution

    struct vec3 light_reflection_dir
        = vec3_reflect(&scene->light_direction, &inter->normal);
    struct vec3 specular_contribution = {0};
    // computes how much the reflection goes in the direction of the
    // camera
    double light_reflection_proj
        = -vec3_dot(&light_reflection_dir, &ray->direction);
    if (light_reflection_proj < 0.0)
        light_reflection_proj = 0.0;
    else
    {
        double spec_coeff
            = pow(light_reflection_proj, mat->spec_n) * mat->spec_Ks;
        specular_contribution = vec3_mul(&scene->light_color, spec_coeff);
    }

    struct vec3 ambient_contribution
        = vec3_mul(&mat->surface_color, mat->ambient_intensity);

    struct vec3 pix_color = {0};
    pix_color = vec3_add(&pix_color, &ambient_contribution);
    pix_color = vec3_add(&pix_color, &diffuse_contribution);
    pix_color = vec3_add(&pix_color, &specular_contribution);
    return pix_color;
}

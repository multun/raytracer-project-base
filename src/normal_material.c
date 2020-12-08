#include "normal_material.h"
#include "image.h"
#include "vec3.h"

static struct vec3 normal_color(const struct vec3 *normal)
{
    struct vec3 res;
    res.x = (normal->x + 1.) / 2.;
    res.y = (normal->y + 1.) / 2.;
    res.z = (normal->z + 1.) / 2.;
    return res;
}

struct vec3 normal_shader(const struct material *base_material,
                          const struct intersection *inter,
                          const struct scene *scene, const struct ray *ray)
{
    (void)base_material;
    (void)scene;
    (void)ray;
    return normal_color(&inter->normal);
}

struct material normal_material = MATERIAL_STATIC_INIT(normal_shader);

#pragma once

#include <math.h>

struct vec3
{
    double x;
    double y;
    double z;
};

static inline struct vec3 vec3_add(const struct vec3 *a, const struct vec3 *b)
{
    return (struct vec3){
        .x = a->x + b->x,
        .y = a->y + b->y,
        .z = a->z + b->z,
    };
}

static inline struct vec3 vec3_sub(const struct vec3 *a, const struct vec3 *b)
{
    return (struct vec3){
        .x = a->x - b->x,
        .y = a->y - b->y,
        .z = a->z - b->z,
    };
}

static inline void vec3_neg(struct vec3 *v)
{
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

static inline struct vec3 vec3_mul(const struct vec3 *a, double c)
{
    return (struct vec3){
        .x = a->x * c,
        .y = a->y * c,
        .z = a->z * c,
    };
}

static inline struct vec3 vec3_mul_vec(const struct vec3 *a,
                                       const struct vec3 *b)
{
    return (struct vec3){
        .x = a->x * b->x,
        .y = a->y * b->y,
        .z = a->z * b->z,
    };
}

static inline double vec3_length(const struct vec3 *v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline void vec3_normalize(struct vec3 *v)
{
    double len = vec3_length(v);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}

static inline double vec3_dot(const struct vec3 *a, const struct vec3 *b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

static inline struct vec3 vec3_cross(const struct vec3 *a, const struct vec3 *b)
{
    return (struct vec3){a->y * b->z - b->y * a->z, a->z * b->x - b->z * a->x,
                         a->x * b->y - b->x * a->y};
}

/*
** Computes the reflection of a vector, given the normal of a surface
*/
static inline struct vec3 vec3_reflect(const struct vec3 *incident_dir,
                                       const struct vec3 *normal)
{
    double correction_coeff = -2 * vec3_dot(incident_dir, normal);
    struct vec3 corrector = vec3_mul(normal, correction_coeff);
    return vec3_add(incident_dir, &corrector);
}

static inline void vec3_update_min_components(struct vec3 *self,
                                              const struct vec3 *o)
{
    if (o->x < self->x)
        self->x = o->x;

    if (o->y < self->y)
        self->y = o->y;

    if (o->z < self->z)
        self->z = o->z;
}

static inline void vec3_update_max_components(struct vec3 *self,
                                              const struct vec3 *o)
{
    if (o->x > self->x)
        self->x = o->x;

    if (o->y > self->y)
        self->y = o->y;

    if (o->z > self->z)
        self->z = o->z;
}

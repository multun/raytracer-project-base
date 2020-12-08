#pragma once

struct refcnt;

typedef void (*refcnt_free_f)(struct refcnt *refcnt);

/*
** A reference counter.
** It's used to track the number of pointers to some object at a given time.
** When a new reference (pointer) is acquired, one should call ref_get.
** When a reference (pointer) is lost, one should call ref_put.
** The initial value of the counter is one.
** When the reference count reaches 0, the object is freed using the free()
** function pointer.
*/
struct refcnt
{
    // the current number of references to some object
    int count;
    // the function to call when the counter reaches 0, or NULL.
    // when this field is NULL, stdlib's free is used by default
    refcnt_free_f free;
};

#define REFCNT_STATIC_INIT                                                     \
    {                                                                          \
        .count = 1, .free = ref_nofree                                         \
    }

void ref_nofree(struct refcnt *refcnt);

void ref_init(struct refcnt *refcnt, refcnt_free_f free);
void ref_get(struct refcnt *refcnt);
void ref_put(struct refcnt *refcnt);

#pragma once

#define STATIC_ASSERT(Name, X)                                                 \
    struct __assert_##Name                                                     \
    {                                                                          \
        char Name : (1 - 2 * (!(X)));                                          \
    }

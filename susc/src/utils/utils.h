#pragma once

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
#define SUSC_UNUSED(x) (void)(x)

#define SUSC_REQUIRE_EXIT(f, x)     \
    do                              \
    {                               \
        int susc_require_ret = (f); \
        if (susc_require_ret != 0)  \
        {                           \
            x;                      \
        }                           \
    } while (0)                     \

#define SUSC_REQUIRE(f) \
    SUSC_REQUIRE_EXIT(f, return susc_require_ret)

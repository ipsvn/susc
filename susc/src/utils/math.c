#include "math.h"

#include <stdio.h>
float susc_lerp(float min, float max, float t)
{
    float clamp = SUSC_CLAMP(t, 0.f, 1.0f);
    return min + (max - min) * clamp;
}

float susc_reverse_lerp(float min, float max, float t)
{
    float num = max - min;
    if (num == 0.f)
    {
        return 0.f;
    }
    return SUSC_CLAMP((t - min) / num, 0.f, 1.f);
}

#pragma once

#define SUSC_CLAMP(v, lo, hi) v < lo ? lo : hi < v ? hi : v

float susc_lerp(float min, float max, float t);
float susc_reverse_lerp(float min, float max, float t);

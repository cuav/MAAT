#include <math.h>

#include "safemath.h"

const float MY_PI = 3.141593f;

float safe_asin(float v)
{
    if (isnan(v))
    {
        return 0.0;
    }
    if (v >= 1.0)
    {
        return MY_PI/2;
    }
    if (v <= -1.0)
    {
        return -MY_PI/2;
    }
    return asin(v);
}

float safe_sqrt(float v)
{
    float ret = sqrt(v);
    if (isnan(ret))
    {
        return 0;
    }
    return ret;
}
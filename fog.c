// fog.c
#include "fog.h"
#include "config.h" 
#include <math.h>

float GetExponentialFogFactor(float fogDensity, int z) {
    return (1 / exp(z * fogDensity));
}

Color GetScaledPixel(Color pixel, Color fog, float fogFactor) {
    pixel.r = pixel.r * fogFactor;
    pixel.g = pixel.g * fogFactor;
    pixel.b = pixel.b * fogFactor;
    pixel.a = pixel.a * fogFactor;
    fog.r = fog.r * (1 - fogFactor);
    fog.g = fog.g * (1 - fogFactor);
    fog.b = fog.b * (1 - fogFactor);
    fog.a = fog.a * (1 - fogFactor);
    pixel.r = pixel.r + fog.r;
    pixel.g = pixel.g + fog.g;
    pixel.b = pixel.b + fog.b;
    pixel.a = pixel.a + fog.a;

    return pixel;
}

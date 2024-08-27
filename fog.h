// fog.h
#ifndef FOG_H
#define FOG_H

#include "config.h" 
#include <raylib.h>

float GetExponentialFogFactor(float fogDensity, int z);
Color GetScaledPixel(Color pixel, Color fog, float fogFactor);

#endif // FOG_H

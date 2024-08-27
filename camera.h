// camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include "config.h"  // Include the config header file

typedef struct {
    float x;
    float y;
    float height;
    float angle;
    float horizon;
    float speed;
    float rotspeed;
    float heightspeed;
    float horizonspeed;
    float tiltspeed;
    float tilt;
    float zfar;
} camera_t;

extern camera_t camera;

void ProcessInput(float timeDelta);

#endif // CAMERA_H

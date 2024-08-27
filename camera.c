// camera.c
#include "camera.h"
#include "config.h" 
#include <math.h>


camera_t camera = {
    .x = 512,
    .y = 512,
    .height = 60,
    .angle = 1.5 * 3.141592,
    .horizon = 100,
    .speed = 60,
    .rotspeed = 0.5,
    .heightspeed = 100,
    .horizonspeed = 100,
    .tiltspeed = 1.5,
    .tilt = 0,
    .zfar = 600
};

void ProcessInput(float timeDelta) {
    if (IsKeyDown(KEY_UP)) {
        camera.x += camera.speed * cos(camera.angle) * timeDelta;
        camera.y += camera.speed * sin(camera.angle) * timeDelta;
    }
    if (IsKeyDown(KEY_DOWN)) {
        camera.x -= camera.speed * cos(camera.angle) * timeDelta;
        camera.y -= camera.speed * sin(camera.angle) * timeDelta;
    }
    if (IsKeyDown(KEY_LEFT)) {
        camera.angle -= camera.rotspeed * timeDelta;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        camera.angle += camera.rotspeed * timeDelta;
    }
    if (IsKeyDown(KEY_Q)) {
        camera.height += camera.heightspeed * timeDelta;
    }
    if (IsKeyDown(KEY_E)) {
        camera.height -= camera.heightspeed * timeDelta;
    }
    if (IsKeyDown(KEY_W)) {
        camera.horizon += camera.horizonspeed * timeDelta;
    }
    if (IsKeyDown(KEY_S)) {
        camera.horizon -= camera.horizonspeed * timeDelta;
    }
    if (IsKeyDown(KEY_R)) {
        camera.angle = 1.5 * 3.141592;
        camera.tilt = 0;
        camera.height = 150;
        camera.horizon = 100;
    }
}

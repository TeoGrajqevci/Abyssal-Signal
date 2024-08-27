// game.c
#include "game.h"
#include "fog.h"
#include "camera.h"
#include <math.h>

void GameLoop(Color *colorMap, Color *heightMap, RenderTexture2D renderTexture, Shader postprocessingShader) {
    float timeDelta = GetFrameTime();
    ProcessInput(timeDelta);

    float sinAngle = sinf(camera.angle);
    float cosAngle = cosf(camera.angle);

    float plx = cosAngle * camera.zfar + sinAngle * camera.zfar;
    float ply = sinAngle * camera.zfar - cosAngle * camera.zfar;
    float prx = cosAngle * camera.zfar - sinAngle * camera.zfar;
    float pry = sinAngle * camera.zfar + cosAngle * camera.zfar;

    BeginTextureMode(renderTexture);
    ClearBackground(DEFAULT_FOG_COLOR);  // Use default fog color

    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float deltaX = (plx + (prx - plx) / SCREEN_WIDTH * i) / camera.zfar;
        float deltaY = (ply + (pry - ply) / SCREEN_WIDTH * i) / camera.zfar;

        float rx = camera.x;
        float ry = camera.y;

        float maxHeight = SCREEN_HEIGHT;

        for (int z = 1; z < camera.zfar; z++) {
            rx += deltaX;
            ry += deltaY;

            int mapOffset = (MAP_N * ((int)ry & (MAP_N - 1))) + ((int)rx & (MAP_N - 1));
            int projHeight = (int)((camera.height - heightMap[mapOffset].r) / z * SCALE_FACTOR + camera.horizon);
            projHeight = projHeight < 0 ? 0 : projHeight;
            projHeight = projHeight > SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : projHeight;

            if (projHeight < maxHeight) {
                float lean = (camera.tilt * (i / (float)SCREEN_WIDTH - 0.5) + 0.5) * SCREEN_HEIGHT / 6;

                for (int y = (projHeight + lean); y < (maxHeight + lean); y++) {
                    Color pixel = colorMap[mapOffset];
                    Color scaledPixel = GetScaledPixel(pixel, DEFAULT_FOG_COLOR, GetExponentialFogFactor(DEFAULT_FOG_DENSITY, z));
                    DrawPixel(i, y, scaledPixel);
                }
                maxHeight = projHeight;
            }
        }
    }

    EndTextureMode();

    Vector2 resolution = {SCREEN_WIDTH * SCALE_FACTOR_WINDOW, SCREEN_HEIGHT * SCALE_FACTOR_WINDOW};
    SetShaderValue(postprocessingShader, GetShaderLocation(postprocessingShader, "iResolution"), &resolution, SHADER_UNIFORM_VEC2);

    BeginDrawing();
    BeginShaderMode(postprocessingShader);
    DrawTexturePro(renderTexture.texture, (Rectangle){0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height},
                   (Rectangle){0, 0, SCREEN_WIDTH * SCALE_FACTOR_WINDOW, SCREEN_HEIGHT * SCALE_FACTOR_WINDOW}, 
                   (Vector2){0, 0}, 0.0f, WHITE);
    EndShaderMode();
    EndDrawing();
}

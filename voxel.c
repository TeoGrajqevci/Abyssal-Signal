#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#define GUI_CONTROLS 1
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400
#define FPS 60
#define MAP_N 1024
#define SCALE_FACTOR 200.0
#define NUM_MAPS 29

// Double size scaling factor
#define SCALE_FACTOR_WINDOW 2

Color *colorMap = NULL;
Color *heightMap = NULL;

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

camera_t camera = {
    .x = 512,
    .y = 512,
    .height = 150,
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

typedef struct {
    char colorMap[50];
    char heightMap[50];
} map_t;

map_t maps[NUM_MAPS];

int fogType = 0;
float fogDensity = 0.01;
float fogStart = 100.0;
float fogEnd = 600.0;
int selectedMap = 0;
int currentSelectedMap = 0;
int mapSelectorMode = 0;

Color FogColor = {2, 1, 18, 255};

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
    if (IsKeyDown(KEY_A)) {
        camera.tilt -= camera.tiltspeed * timeDelta;
        camera.tilt = camera.tilt < -1 ? -1 : camera.tilt;
    }
    if (IsKeyDown(KEY_D)) {
        camera.tilt += camera.tiltspeed * timeDelta;
        camera.tilt = camera.tilt > 1 ? 1 : camera.tilt;
    }

    if (IsKeyDown(KEY_R)) {
        camera.angle = 1.5 * 3.141592;
        camera.tilt = 0;
        camera.height = 150;
        camera.horizon = 100;
    }
}

int GetLinearFogFactor(int fogEnd, int fogStart, int z) {
    return (int)((fogEnd - z) / (fogEnd - fogStart));
}

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

void LoadMaps() {
    for (size_t i = 0; i < NUM_MAPS; i++) {
        map_t map;
        sprintf(map.colorMap, "resource/map%d.color.gif", (int)i);
        sprintf(map.heightMap, "resource/map%d.height.gif", (int)i);
        maps[i] = map;
    }
}

int main() {
    LoadMaps();
    InitWindow(SCREEN_WIDTH * SCALE_FACTOR_WINDOW, SCREEN_HEIGHT * SCALE_FACTOR_WINDOW, "Voxel Space");

    SetWindowTitle("Abyssal-Signal");

    // Load initial map
    Image colorMapImage = LoadImage(maps[selectedMap].colorMap);
    Image heightMapImage = LoadImage(maps[selectedMap].heightMap);
    colorMap = LoadImageColors(colorMapImage);
    heightMap = LoadImageColors(heightMapImage);

    SetTargetFPS(FPS);

    // Set up render texture
    RenderTexture2D renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!WindowShouldClose()) {

        // Handle map switching
        if (currentSelectedMap != selectedMap) {
            selectedMap = currentSelectedMap;
            colorMapImage = LoadImage(maps[selectedMap].colorMap);
            heightMapImage = LoadImage(maps[selectedMap].heightMap);
            colorMap = LoadImageColors(colorMapImage);
            heightMap = LoadImageColors(heightMapImage);
        }

        // Time delta for movement
        float timeDelta = GetFrameTime();
        ProcessInput(timeDelta);

        float sinangle = sin(camera.angle);
        float cosangle = cos(camera.angle);

        // Calculate the extreme rays
        float plx = cosangle * camera.zfar + sinangle * camera.zfar;
        float ply = sinangle * camera.zfar - cosangle * camera.zfar;
        float prx = cosangle * camera.zfar - sinangle * camera.zfar;
        float pry = sinangle * camera.zfar + cosangle * camera.zfar;

        // Start rendering to texture
        BeginTextureMode(renderTexture);
        ClearBackground(FogColor);

        // Render the scene
        for (size_t i = 0; i < SCREEN_WIDTH; i++) {
            float deltaX = (plx + (prx - plx) / SCREEN_WIDTH * i) / camera.zfar;
            float deltaY = (ply + (pry - ply) / SCREEN_WIDTH * i) / camera.zfar;

            float rx = camera.x;
            float ry = camera.y;

            float maxHeight = SCREEN_HEIGHT;

            for (size_t z = 1; z < camera.zfar; z++) {
                rx += deltaX;
                ry += deltaY;

                int mapoffset = (MAP_N * ((int)(ry) & (MAP_N - 1))) + ((int)(rx) & (MAP_N - 1));
                int projHeight = (int)((camera.height - heightMap[mapoffset].r) / z * SCALE_FACTOR + camera.horizon);
                projHeight = projHeight < 0 ? 0 : projHeight;
                projHeight = projHeight > SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : projHeight;

                if (projHeight < maxHeight) {
                    float lean = (camera.tilt * (i / (float)SCREEN_WIDTH - 0.5) + 0.5) * SCREEN_HEIGHT / 6;

                    for (size_t y = (projHeight + lean); y < (maxHeight + lean); y++) {
                        Color pixel = colorMap[mapoffset];

                        // Apply fog scaling
                        Color scaledPixel = GetScaledPixel(pixel, FogColor, GetExponentialFogFactor(fogDensity, z));
                        DrawPixel(i, y, scaledPixel);
                    }
                    maxHeight = projHeight;
                }
            }
        }

          if (IsKeyPressed(KEY_TAB)) {
                currentSelectedMap = (currentSelectedMap + 1) % NUM_MAPS;
          }

        EndTextureMode();

        // Scale the render texture to fit the window
        BeginDrawing();

        // Draw the texture scaled up to 2x
        DrawTexturePro(renderTexture.texture,
                       (Rectangle){0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height},
                       (Rectangle){0, 0, SCREEN_WIDTH * SCALE_FACTOR_WINDOW, SCREEN_HEIGHT * SCALE_FACTOR_WINDOW},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);

        EndDrawing();

        // Toggle fullscreen
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
        }
    }

    UnloadRenderTexture(renderTexture);
    UnloadImageColors(colorMap);
    UnloadImageColors(heightMap);
    CloseWindow();

    return 0;
}

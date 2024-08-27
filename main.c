// main.c
#include <raylib.h>
#include <stdlib.h>  
#include "camera.h"
#include "menu.h"
#include "map.h"
#include "game.h"
#include "config.h" 

int main() {
    LoadMaps();

    // Initialize the selected map to 2 (default map)
    selectedMap = 2;

    InitWindow(SCREEN_WIDTH * SCALE_FACTOR_WINDOW, SCREEN_HEIGHT * SCALE_FACTOR_WINDOW, "Voxel Space");
    SetWindowTitle("Abyssal-Signal");
    SetTargetFPS(FPS);

    Font font = LoadFont("resources/GothicPixels.ttf");
    Shader postprocessingShader = LoadShader(0, "shaders/postprocessing.fs");

    // Load default map (map 2)
    Image colorMapImage = LoadImage(maps[selectedMap].colorMap);
    Image heightMapImage = LoadImage(maps[selectedMap].heightMap);
    colorMap = LoadImageColors(colorMapImage);
    heightMap = LoadImageColors(heightMapImage);
    UnloadImage(colorMapImage);
    UnloadImage(heightMapImage);

    RenderTexture2D renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!WindowShouldClose()) {
        if (!startGame) {
            if (mapsMenu) {
                MapSelectionScreen(font);
            } else {
                StartScreen(font);
            }
        } else {
            GameLoop(colorMap, heightMap, renderTexture, postprocessingShader);

            if (IsKeyPressed(KEY_F)) {
                ToggleFullscreen();
            }

            if (IsWindowFullscreen()) {
                SetMousePosition(SCREEN_WIDTH * SCALE_FACTOR_WINDOW / 2, SCREEN_HEIGHT * SCALE_FACTOR_WINDOW / 2);
                HideCursor();
            }
        }
    }

    // Cleanup
    UnloadRenderTexture(renderTexture);
    if (colorMap != NULL) UnloadImageColors(colorMap);
    if (heightMap != NULL) UnloadImageColors(heightMap);
    CloseWindow();
    UnloadShader(postprocessingShader);
    UnloadFont(font);

    return 0;
}

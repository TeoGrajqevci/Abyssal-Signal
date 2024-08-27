// menu.c
#include "menu.h"
#include "config.h"
#include "map.h"
#include <stdio.h>

extern Color *colorMap;
extern Color *heightMap;
extern int selectedMap;
extern int currentSelectedMap;
extern bool startGame;
extern bool mapsMenu;

static MenuOption selectedOption = MENU_START;

void DrawMenuOption(Font font, const char* text, Vector2 position, bool selected, bool blink) {
    Color color = selected ? (Color){255, 0, 0, 255} : (Color){255, 255, 255, 255};

    if (blink && selected) {
        static float alpha = 1.0f;
        static bool increasing = false;
        static float blinkTimer = 0.0f;

        blinkTimer += GetFrameTime();

        if (blinkTimer >= 0.02f) {
            blinkTimer = 0.0f;
            if (increasing) {
                alpha += 0.05f;
                if (alpha >= 1.0f) {
                    alpha = 1.0f;
                    increasing = false;
                }
            } else {
                alpha -= 0.05f;
                if (alpha <= 0.0f) {
                    alpha = 0.0f;
                    increasing = true;
                }
            }
        }

        color.a = (unsigned char)(255 * alpha);
    }

    DrawTextEx(font, text, position, 40, 0, color);
}

void StartScreen(Font font) {
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (MenuOption)((selectedOption + MENU_COUNT - 1) % MENU_COUNT);
    } else if (IsKeyPressed(KEY_UP)) {
        selectedOption = (MenuOption)((selectedOption + 1) % MENU_COUNT);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedOption) {
            case MENU_START:
                startGame = true;
                break;
            case MENU_OPTIONS:
                startGame = true;  // For now, just start the game
                break;
            case MENU_MAPS:
                mapsMenu = true;
                break;
            default:
                break;
        }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    Vector2 center = { (SCREEN_WIDTH * SCALE_FACTOR_WINDOW) / 2, (SCREEN_HEIGHT * SCALE_FACTOR_WINDOW) / 2 };

    DrawMenuOption(font, "Start", (Vector2){center.x - MeasureTextEx(font, "Start", 40, 0).x / 2, center.y - 60}, selectedOption == MENU_START, selectedOption == MENU_START);
    DrawMenuOption(font, "Options", (Vector2){center.x - MeasureTextEx(font, "Options", 40, 0).x / 2, center.y}, selectedOption == MENU_OPTIONS, selectedOption == MENU_OPTIONS);
    DrawMenuOption(font, "Maps", (Vector2){center.x - MeasureTextEx(font, "Maps", 40, 0).x / 2, center.y + 60}, selectedOption == MENU_MAPS, selectedOption == MENU_MAPS);

    EndDrawing();
}

void MapSelectionScreen(Font font) {
    static Texture2D colorMapTexture = {0};  // Ensure texture is initialized to 0
    static int lastSelectedMap = -1;         // Keep track of the last selected map to avoid reloading unnecessarily

    // Check if the map has changed and load the new map texture
    if (currentSelectedMap != lastSelectedMap) {
        // Unload previous texture if it exists
        if (colorMapTexture.id != 0) {
            UnloadTexture(colorMapTexture);
            colorMapTexture.id = 0;
        }

        // Load the new map's colormap texture
        Image colorMapImage = LoadImage(maps[currentSelectedMap].colorMap);
        if (colorMapImage.width == 0 || colorMapImage.height == 0) {
            printf("Error: Failed to load color map image.\n");
        } else {
            colorMapTexture = LoadTextureFromImage(colorMapImage);
            UnloadImage(colorMapImage);  // Free the image after creating the texture
        }

        lastSelectedMap = currentSelectedMap;  // Update the last selected map
    }

    // Handle input for map selection
    if (IsKeyPressed(KEY_DOWN)) {
        currentSelectedMap = (currentSelectedMap + NUM_MAPS - 1) % NUM_MAPS;
    } else if (IsKeyPressed(KEY_UP)) {
        currentSelectedMap = (currentSelectedMap + 1) % NUM_MAPS;
    }

    // When Enter is pressed, start the game with the selected map
    if (IsKeyPressed(KEY_ENTER)) {
        selectedMap = currentSelectedMap;
        startGame = true;
        mapsMenu = false;

        // Unload previous textures and image data
        if (colorMapTexture.id != 0) {
            UnloadTexture(colorMapTexture);
            colorMapTexture.id = 0;
        }
        if (colorMap != NULL) {
            UnloadImageColors(colorMap);
            colorMap = NULL;
        }
        if (heightMap != NULL) {
            UnloadImageColors(heightMap);
            heightMap = NULL;
        }

        // Load the selected map data
        Image colorMapImage = LoadImage(maps[selectedMap].colorMap);
        colorMap = LoadImageColors(colorMapImage);
        Image heightMapImage = LoadImage(maps[selectedMap].heightMap);
        heightMap = LoadImageColors(heightMapImage);

        UnloadImage(colorMapImage);
        UnloadImage(heightMapImage);

        if (!colorMap || !heightMap) {
            printf("Error loading map images.\n");
            startGame = false;
            mapsMenu = true;  // Stay in the maps menu if there's an error
        }
    }

    // Start drawing the screen
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw the colormap texture centered in the window with a white outline
    if (colorMapTexture.id != 0) {
        // Calculate the center position
        float scaleFactor = 0.5f;  // Adjust if you want to scale the texture
        float textureWidth = colorMapTexture.width * scaleFactor;
        float textureHeight = colorMapTexture.height * scaleFactor;

        Vector2 texturePosition = {
            (SCREEN_WIDTH * SCALE_FACTOR_WINDOW - textureWidth) / 2,
            (SCREEN_HEIGHT * SCALE_FACTOR_WINDOW - textureHeight) / 2
        };

        // Draw the white outline
        DrawRectangleLinesEx(
            (Rectangle){ texturePosition.x - 10, texturePosition.y - 10, textureWidth + 20, textureHeight + 20 },
            10, WHITE
        );

        // Draw the texture
        DrawTextureEx(colorMapTexture, texturePosition, 0.0f, scaleFactor, WHITE);
    }

    EndDrawing();
}

// game.h
#ifndef GAME_H
#define GAME_H

#include "config.h" 
#include <raylib.h>

void GameLoop(Color *colorMap, Color *heightMap, RenderTexture2D renderTexture, Shader postprocessingShader);

#endif // GAME_H

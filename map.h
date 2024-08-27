// map.h
#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "config.h"

typedef struct {
    char colorMap[50];
    char heightMap[50];
} map_t;

extern map_t maps[NUM_MAPS];
extern int selectedMap;
extern int currentSelectedMap;
extern Color *colorMap;
extern Color *heightMap;
extern bool startGame;
extern bool mapsMenu;

void LoadMaps();

#endif // MAP_H

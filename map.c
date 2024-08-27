// map.c
#include "map.h"
#include "config.h"
#include <stdio.h>

map_t maps[NUM_MAPS];
Color *colorMap = NULL;
Color *heightMap = NULL;
int selectedMap = 0;
int currentSelectedMap = 0;
bool startGame = false;
bool mapsMenu = false;

void LoadMaps() {
    for (size_t i = 0; i < NUM_MAPS; i++) {
        map_t map;
        sprintf(map.colorMap, "resources/map%d.color.gif", (int)i);
        sprintf(map.heightMap, "resources/map%d.height.gif", (int)i);
        maps[i] = map;
    }
}

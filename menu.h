// menu.h
#ifndef MENU_H
#define MENU_H

#include <raylib.h>

typedef enum {
    MENU_START,
    MENU_MAPS,
    MENU_OPTIONS,
    MENU_COUNT
} MenuOption;

void StartScreen(Font font);
void MapSelectionScreen(Font font);

#endif // MENU_H

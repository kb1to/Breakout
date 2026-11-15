#pragma once
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "game.h"

struct Menu{
    struct Button playButton;
    struct Button settingsButton;
    struct Button playButton_hover;
    struct Button settingsButton_hover;
    SDL_Color backgroundColor;
    struct Button** textList;
    int textCount; 
};

int create_menu(struct Game* game);
void destroy_menu(struct Game* game);
#pragma once
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define NAME "Breakout"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGT 640

struct Menu{
    SDL_Texture* Background;
    SDL_Rect play_rect;
    SDL_Texture* play_text;
    SDL_Rect settings_rect;
    SDL_Texture* settings_text;
};

struct Game{
    SDL_Window* gameWindow;
    SDL_Renderer* gameRenderer;
    TTF_Font* textFont;
    struct Menu* menu;
    SDL_Color textColor;
};


int initSDL(struct Game* game);
int freeSDL(struct Game* game);
int initMenu(struct Game* game);
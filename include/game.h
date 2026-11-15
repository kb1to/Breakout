#pragma once
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define NAME "Breakout"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640


enum State{
    Menu = 0,
    Settings = 1,
    Playing = 2,
};

struct Button{
    SDL_Texture* text;
    SDL_Rect rectange;
    SDL_Color color;
};



struct Game{
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* global_font;
    bool running;
    enum State state;
    struct Menu* menu;
    Uint64 previous;
    double DT; 
    double collector;
    Uint8 input[256];
};



int initialize_sdl(struct Game *game);
int cleanupGame(struct Game* game);
int game_run(struct Game *game,Uint64 *previous, double *DT, double* collector);
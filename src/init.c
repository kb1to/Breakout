#include "init.h"
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#define IMAGE_FLAGS IMG_INIT_PNG
#define TEXT_SIZE 40

int initSDL(struct Game* game){
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        freeSDL(game);
        return  EXIT_FAILURE;
    }

    int img_init = IMG_Init(IMAGE_FLAGS);
    if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
        fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    game->textFont = TTF_OpenFont("media/font.ttf", TEXT_SIZE);
    if (!game->textFont) {
        fprintf(stderr, "Error creating Font: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    game->gameWindow = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGT, 0);
      
    if(!game->gameWindow)
    {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        freeSDL(game);
        return  EXIT_FAILURE;
    }

    game->gameRenderer = SDL_CreateRenderer(game->gameWindow, -1, 0);
    if (!game->gameRenderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        freeSDL(game);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int freeSDL(struct Game *game){
    SDL_DestroyTexture(game->menu->settings_text);
    SDL_DestroyTexture(game->menu->play_text);
    SDL_DestroyTexture(game->menu->Background);
    SDL_DestroyRenderer(game->gameRenderer);
    SDL_DestroyWindow(game->gameWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}

int initMenu(struct Game* game){
    game->menu->Background = IMG_LoadTexture(game->gameRenderer, "media/background.png");
    if (!game->menu->Background) {
        fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
        freeSDL(game);
        return EXIT_FAILURE;
    }
    SDL_Surface *surface = TTF_RenderText_Blended(game->textFont, "Play", game->textColor);
    game->menu->play_rect.w = surface->w;
    game->menu->play_rect.h = surface->h;
    game->menu->play_text = SDL_CreateTextureFromSurface(game->gameRenderer, surface);
    SDL_FreeSurface(surface);
    if(!game->menu->play_text){
        fprintf(stderr,"error initalizing texture of play %s", SDL_GetError());
        freeSDL(game);
        return EXIT_FAILURE;
    }

    SDL_Surface *surfaceB = TTF_RenderText_Blended(game->textFont, "Settings", game->textColor);
    game->menu->settings_rect.w = surfaceB->w;
    game->menu->settings_rect.h = surfaceB->h;
    game->menu->settings_text = SDL_CreateTextureFromSurface(game->gameRenderer, surfaceB);
    SDL_FreeSurface(surfaceB);
    if(!game->menu->settings_text){
        fprintf(stderr,"error initalizing texture of settings %s", SDL_GetError());
        freeSDL(game);
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

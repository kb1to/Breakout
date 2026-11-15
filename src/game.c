#include "game.h"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "menu.h"

#define FONT_PATH "media/font.ttf"
#define FONT_SIZE 40


int initialize_sdl(struct Game *game){
    //basic init
    if (SDL_Init(SDL_INIT_VIDEO) != EXIT_SUCCESS)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
    {
        fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    
    if (TTF_Init() != EXIT_SUCCESS) 
    {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }

    //window, renderer,font

    game->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!game->window){
        fprintf(stderr, "Error init window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_CreateRenderer(game->window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(!game->renderer){
        fprintf(stderr,"error init render %s", SDL_GetError());
        return EXIT_FAILURE;    
    }

    game->global_font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if(!game->global_font){
        fprintf(stderr, "FONT OPEN %s", TTF_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



int cleanupGame(struct Game* game){
    destroy_menu(game);
    if(game->renderer)
    {
        SDL_DestroyRenderer(game->renderer);
    }
    if(game->window)
    {
        SDL_DestroyWindow(game->window);
    }
    if(game->global_font)
    {
        TTF_CloseFont(game->global_font);
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    free(game);
    return EXIT_SUCCESS;
}


void handleEvents(struct Game* game){
    SDL_Event event;
    
   if (event.type == SDL_KEYDOWN) {
       game->input[event.key.keysym.scancode] = true;
    
   }
   if (event.type == SDL_KEYUP) {
       game->input[event.key.keysym.scancode] = false;
       
   }
   if(event.type == SDL_QUIT){
        game->running = false;
   }
   if(game->input[SDL_SCANCODE_S] || game->input[SDL_SCANCODE_DOWN]){
        switch(game->state){
            case Menu:
                
                break;
        }
   }
}



int game_run(struct Game *game,Uint64 *previous, double *DT, double* collector){
    Uint64 current = SDL_GetPerformanceCounter();
    double secConverter = SDL_GetPerformanceFrequency();
    double elapsed = (double)(current  - *previous) / secConverter;
    *collector +=  elapsed;
    *previous = current;
    while(*collector >= *DT){
        *collector -= *DT;
    }
    
    return EXIT_SUCCESS;
}
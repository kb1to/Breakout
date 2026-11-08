#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_platform.h"
#include "SDL_stdinc.h"
#include "SDL_timer.h"
#include "init.h"
#include <stdint.h>

uint8_t safeClose(struct Game* game); 

enum States{
    Exit = 0,
    Menu = 2,
    Menu_Settings = 1,
    Playing = 3,
};

int main(){
    struct Game* game = (struct Game*)calloc(1, sizeof(struct Game));
    game->gameWindow = NULL;
    game->gameRenderer = NULL; 
    game->menu = (struct Menu*)calloc(1, sizeof(struct Menu));
    game->textFont = NULL;
    
    game->textColor = (SDL_Color){255, 255, 255, 255};
    
    game->menu->Background = NULL; 
    
    game->menu->play_rect = (SDL_Rect){350, 300, 0, 0}; 
    
    game->menu->play_text = NULL;

    game->menu->settings_rect = (SDL_Rect){350, 450, 0, 0}; 
    
    game->menu->settings_text = NULL;

    if (initSDL(game) == EXIT_FAILURE) {
        return EXIT_FAILURE; 
    }

    enum States state = Menu;
    initMenu(game);

    int running = 1; 

    while(running){
        SDL_Event event;
        
        // 3. Fix the missing brace and handle core events independently of the state
        while (SDL_PollEvent(&event))
        {   
            if (event.type == SDL_QUIT) {
                running = 0; // Break the main loop safely
            }
        }

        switch (state) {
            case Menu:
                break;
            case Exit:
                running = 0; 
                break;
            case Playing:
                break;
            case Menu_Settings:
                break;
        }

        SDL_RenderClear(game->gameRenderer);
        SDL_RenderCopy(game->gameRenderer, game->menu->Background, NULL, NULL);
        SDL_RenderCopy(game->gameRenderer, game->menu->play_text, NULL, &game->menu->play_rect);
        SDL_RenderCopy(game->gameRenderer, game->menu->settings_text, NULL, &game->menu->settings_rect);
        SDL_RenderPresent(game->gameRenderer);

        SDL_Delay(16);
    }

    safeClose(game);
    return 0;
}

uint8_t safeClose(struct Game* game){
    freeSDL(game);
    free(game);
    game = NULL;
    return EXIT_SUCCESS;
}
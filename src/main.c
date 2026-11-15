#include "SDL_error.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

int main(void){

    struct Game* game = (struct Game*)calloc(1,sizeof(struct Game));

    if (!game) {
        fprintf(stderr, "Failed to allocate Game\n");
        return EXIT_FAILURE;
}

    if(initialize_sdl(game))
    {
        fprintf(stderr,"Error with SDL_initalization! %s", SDL_GetError());
        cleanupGame(game);
        return EXIT_FAILURE;
    }
    else 
    {
        game->running = true;
        game->state = Menu;
    }

      game->previous = SDL_GetPerformanceCounter();;
      game->DT = 1.0/60.0; 
      game->collector = 0;
    while (game->running) {
     
       game_run(game, &game->previous, &game->DT, &game->collector);
       
    }

    cleanupGame(game);

    return EXIT_SUCCESS;
}
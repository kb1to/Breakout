#include "menu.h"
#include "SDL_platform.h"
#include "SDL_stdinc.h"
#include "game.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

#define TEXT_COUNTER 4

SDL_Texture *createText(TTF_Font *font,const char *text,SDL_Color color,SDL_Renderer *ren,SDL_Rect *rect) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);

    if (!surface) {
        fprintf(stderr, "Text rendering failed: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);

    if (!texture) {
        fprintf(stderr, "Texture creation failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);

    return texture;
}


int create_menu(struct Game* game){
    game->menu = (struct Menu*)calloc(1,sizeof(struct Menu));

    if (!game->menu) {
    return EXIT_FAILURE;
}

    //playtext
    game->menu->playButton.color = (SDL_Color){255,255,255,255};
    game->menu->playButton.text = createText(game->global_font, "play", game->menu->playButton.color, game->renderer, &game->menu->playButton.rectange);
    game->menu->playButton_hover.color = (SDL_Color){25,215,105,255};
    game->menu->playButton_hover.text = createText(game->global_font, "play", game->menu->playButton.color, game->renderer, &game->menu->playButton_hover.rectange);

    //settings text
    game->menu->settingsButton.color = (SDL_Color){255,255,255,255};
    game->menu->settingsButton.text = createText(game->global_font, "settings", game->menu->playButton.color, game->renderer, &game->menu->settingsButton.rectange);
    game->menu->settingsButton_hover.color = (SDL_Color){25,215,105,255};
    game->menu->settingsButton_hover.text = createText(game->global_font, "settings", game->menu->playButton.color, game->renderer, &game->menu->settingsButton_hover.rectange);


    game->menu->textList = (struct Button**)calloc(TEXT_COUNTER,sizeof(struct Button*));

    if (!game->menu->textList) {
        destroy_menu(game);
        return EXIT_FAILURE;
    }
    game->menu->textList[game->menu->textCount] = &game->menu->playButton;
    game->menu->textCount += 1;
    game->menu->textList[game->menu->textCount] = &game->menu->playButton_hover;
    game->menu->textCount += 1;
    game->menu->textList[game->menu->textCount] = &game->menu->settingsButton;
    game->menu->textCount += 1;
    game->menu->textList[game->menu->textCount] = &game->menu->settingsButton_hover;
    game->menu->textCount += 1;


    game->menu->backgroundColor = (SDL_Color){0,0,0,255};

    for(int i = 0; i < game->menu->textCount; ++i){
        game->menu->textList[i]->rectange.x = (SCREEN_WIDTH / 2) - (game->menu->textList[i]->rectange.w / 2); 
    }

    game->menu->playButton.rectange.y = (SCREEN_HEIGHT / 2) - (game->menu->playButton.rectange.h / 2); 
    game->menu->playButton_hover.rectange.y = (SCREEN_HEIGHT / 2) - (game->menu->playButton_hover.rectange.h / 2);
    game->menu->settingsButton.rectange.y = game->menu->playButton.rectange.y - (game->menu->settingsButton.rectange.h) - 100; 
    game->menu->settingsButton_hover.rectange.y = game->menu->playButton.rectange.y - (game->menu->settingsButton_hover.rectange.h) - 100; 


    return EXIT_SUCCESS;
}


void destroy_menu(struct Game* game){
    if (!game || !game->menu) {
        return;
    }
    if(game->menu->textCount != 0){
        for(int i = 0; i < game->menu->textCount; ++i){
            SDL_DestroyTexture(game->menu->textList[i]->text);;
        }
    }
    free(game->menu->textList);
    free(game->menu);
    game->menu = NULL;
}

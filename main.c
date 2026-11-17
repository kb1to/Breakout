
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define width 860
#define height 640

struct Paddle {
  SDL_Rect rect;
  SDL_Color color;
  SDL_Texture *texture;
};

void handleKeyEvents(SDL_Event *event, struct Paddle *p, Uint8 *keys,
                     double DT);

void DrawCircle(SDL_Renderer *Renderer, int _x, int _y, int radius);

int main(void) {

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *win = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, width, height, 0);
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

  int game = 1;

  struct Paddle paddle = {
      .rect = {width - width / 2, height - height / 2, 150, 100},
      .color = {100, 102, 20, 255},
      .texture = NULL};

  Uint8 keys[256] = {0};

  Uint64 previous = SDL_GetPerformanceCounter();
  Uint64 freq = SDL_GetPerformanceFrequency();
  double accum = 0.0;
  const double frame = 1.0 / 60.0;

  while (game) {
    Uint64 current = SDL_GetPerformanceCounter();
    double elapsed = (double)(current - previous) / (double)freq;
    previous = current;
    accum += elapsed;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        game = 0;
      if (event.type == SDL_KEYDOWN)
        keys[event.key.keysym.scancode] = true;
      if (event.type == SDL_KEYUP)
        keys[event.key.keysym.scancode] = false;
    }

    while (accum >= frame) {
      accum -= frame;
      double speed = 300.0; // px per second
      if (keys[SDL_SCANCODE_RIGHT])
        paddle.rect.x += (int)(speed * frame);
      if (keys[SDL_SCANCODE_LEFT])
        paddle.rect.x -= (int)(speed * frame);
    }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, paddle.color.r, paddle.color.g, paddle.color.b,
                           paddle.color.a);
    DrawCircle(ren, 300, 400, 50);
    SDL_RenderFillRect(ren, &paddle.rect);
    SDL_RenderPresent(ren);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return EXIT_SUCCESS;
}

void handleKeyEvents(SDL_Event *event, struct Paddle *p, Uint8 *keys,
                     double DT) {
  if (event->type == SDL_KEYDOWN) {
    keys[event->key.keysym.scancode] = true;
  }
  if (event->type == SDL_KEYUP) {
    keys[event->key.keysym.scancode] = false;
  }

  if (keys[SDL_SCANCODE_RIGHT]) {
    p->rect.x += (int)(300 * DT);
  }
  if (keys[SDL_SCANCODE_LEFT]) {
    p->rect.x -= (int)(300 * DT);
  }
}

void DrawCircle(SDL_Renderer *Renderer, int _x, int _y, int radius) {
  int x = radius - 1;
  int y = 0;
  int tx = 1;
  int ty = 1;
  int err = tx - (radius << 1); // shifting bits left by 1 effectively
                                // doubles the value. == tx - diameter
  while (x >= y) {
    //  Each of the following renders an octant (1/8th) of the circle
    SDL_RenderDrawPoint(Renderer, _x + x, _y - y);
    SDL_RenderDrawPoint(Renderer, _x + x, _y + y);
    SDL_RenderDrawPoint(Renderer, _x - x, _y - y);
    SDL_RenderDrawPoint(Renderer, _x - x, _y + y);
    SDL_RenderDrawPoint(Renderer, _x + y, _y - x);
    SDL_RenderDrawPoint(Renderer, _x + y, _y + x);
    SDL_RenderDrawPoint(Renderer, _x - y, _y - x);
    SDL_RenderDrawPoint(Renderer, _x - y, _y + x);

    if (err <= 0) {
      y++;
      err += ty;
      ty += 2;
    } else {
      x--;
      tx += 2;
      err += tx - (radius << 1);
    }
  }
}

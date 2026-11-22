#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define ROWS 4
#define COLS 11
#define TOTAL_BRICKS (ROWS * COLS)


struct Ball {
    SDL_Texture* texture;
    SDL_Rect rect;
    double ball_speed_x;
    double ball_speed_y; 
};

struct Paddle {
    SDL_Texture* texture;
    SDL_Rect rect;
    double speed;
};

struct Brick {
    SDL_Texture* texture;
    SDL_Rect rect;
    bool destroyed; 
};

struct AllBricks {
    struct Brick bricks[TOTAL_BRICKS]; 
};

struct AssetManager {
    SDL_Texture* brick_textures[4]; 
    SDL_Texture* crack_textures[4];
    SDL_Texture* ball;
    SDL_Texture* paddle;
    SDL_Texture* background;
};


void load_assets(struct AssetManager* am, SDL_Renderer* ren) {
    // Make sure these paths match your folder structure!
    am->ball = IMG_LoadTexture(ren, "bricks/ball.png");
    am->paddle = IMG_LoadTexture(ren, "bricks/paddle.png"); 
    am->background = IMG_LoadTexture(ren, "bricks/bg.png");
    
    am->brick_textures[0] = IMG_LoadTexture(ren, "bricks/green.png");
    am->brick_textures[1] = IMG_LoadTexture(ren, "bricks/orange.png"); 
    am->brick_textures[2] = IMG_LoadTexture(ren, "bricks/red.png"); 
    am->brick_textures[3] = IMG_LoadTexture(ren, "bricks/yellow.png");

    if (!am->ball) printf("Failed to load ball: %s\n", IMG_GetError());
    if (!am->paddle) printf("Failed to load paddle: %s\n", IMG_GetError());
    if (!am->background) printf("Failed to load background: %s\n", IMG_GetError());
}

void free_assets(struct AssetManager* am) {
    SDL_DestroyTexture(am->ball);
    SDL_DestroyTexture(am->paddle);
    SDL_DestroyTexture(am->background);
    
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(am->brick_textures[i]);
        if (am->crack_textures[i]) SDL_DestroyTexture(am->crack_textures[i]);
    }
    free(am);
}


void init_bricks(struct AllBricks* all_bricks, struct AssetManager* am) {
    int brick_w = 60;
    int brick_h = 20;
    int padding = 10;
    
    int start_x = (WIDTH - (COLS * (brick_w + padding) - padding)) / 2;
    int start_y = 80; 

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int index = row * COLS + col;
            struct Brick* brick = &all_bricks->bricks[index];
            
            brick->texture = am->brick_textures[row];
            brick->rect = (SDL_Rect){
                .h = brick_h,
                .w = brick_w,
                .x = start_x + col * (brick_w + padding),
                .y = start_y + row * (brick_h + padding),
            };
            brick->destroyed = false; 
        }
    }
}


void checkBrickCollision(struct AllBricks* bricks, struct Ball* ball) {
    for(int i = 0; i < TOTAL_BRICKS; ++i) {
        struct Brick* brick = &bricks->bricks[i];
        
        if (brick->destroyed) continue;

        if (ball->rect.x < brick->rect.x + brick->rect.w &&
            ball->rect.x + ball->rect.w > brick->rect.x &&
            ball->rect.y < brick->rect.y + brick->rect.h &&
            ball->rect.y + ball->rect.h > brick->rect.y) 
        {
            int overlap_x = (ball->rect.w / 2 + brick->rect.w / 2) - abs((ball->rect.x + ball->rect.w / 2) - (brick->rect.x + brick->rect.w / 2));
            int overlap_y = (ball->rect.h / 2 + brick->rect.h / 2) - abs((ball->rect.y + ball->rect.h / 2) - (brick->rect.y + brick->rect.h / 2));

            if (overlap_x < overlap_y) {
                ball->ball_speed_x = -ball->ball_speed_x;
                if (ball->rect.x < brick->rect.x) ball->rect.x -= overlap_x;
                else ball->rect.x += overlap_x;
            } else {
                ball->ball_speed_y = -ball->ball_speed_y;
                if (ball->rect.y < brick->rect.y) ball->rect.y -= overlap_y;
                else ball->rect.y += overlap_y;
            }

            brick->destroyed = true; 
            break; 
        }
    }
}

void checkPaddleCollision(struct Ball* ball, struct Paddle* paddle) {
    if (ball->rect.x < paddle->rect.x + paddle->rect.w &&
        ball->rect.x + ball->rect.w > paddle->rect.x &&
        ball->rect.y < paddle->rect.y + paddle->rect.h &&
        ball->rect.y + ball->rect.h > paddle->rect.y) 
    {
        float hit_pos = (ball->rect.x + (ball->rect.w / 2.0f)) - (paddle->rect.x + (paddle->rect.w / 2.0f));
        float normalized_hit = hit_pos / (paddle->rect.w / 2.0f);

        ball->ball_speed_x = normalized_hit * 400.0f; 
        ball->ball_speed_y = -ball->ball_speed_y; 

        ball->rect.y = paddle->rect.y - ball->rect.h;
    }
}


int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG); 

    SDL_Window *win = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

    struct AssetManager* am = (struct AssetManager*)calloc(1, sizeof(struct AssetManager));
    load_assets(am, ren);

    struct AllBricks allbricks;
    init_bricks(&allbricks, am);

    struct Paddle paddle = {
        .texture = am->paddle,
        .rect = { (WIDTH / 2) - 50, HEIGHT - 50, 100, 20 }, 
        .speed = 500.0,
    };

    struct Ball ball = {
        .texture = am->ball,
        .rect = { (WIDTH / 2) - 10, HEIGHT - 100, 20, 20 }, 
        .ball_speed_x = 200.0,
        .ball_speed_y = -300.0, 
    };

    Uint8 keys[256] = {0};
    Uint64 previous = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();
    double accum = 0.0;
    const double frame = 1.0 / 60.0;
    
    int game = 1;

    while (game) {
        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed = (double)(current - previous) / (double)freq;
        previous = current;
        accum += elapsed;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) game = 0;
            if (event.type == SDL_KEYDOWN) keys[event.key.keysym.scancode] = true;
            if (event.type == SDL_KEYUP) keys[event.key.keysym.scancode] = false;
        }

        while (accum >= frame) {
            accum -= frame;

            if (keys[SDL_SCANCODE_RIGHT] && paddle.rect.x + paddle.rect.w < WIDTH) {
                paddle.rect.x += paddle.speed * frame;
            }
            if (keys[SDL_SCANCODE_LEFT] && paddle.rect.x > 0) {
                paddle.rect.x -= paddle.speed * frame;
            }

            ball.rect.x += ball.ball_speed_x * frame;
            ball.rect.y += ball.ball_speed_y * frame;

            if (ball.rect.x >= WIDTH - ball.rect.w) {
                ball.ball_speed_x = -ball.ball_speed_x;
                ball.rect.x = WIDTH - ball.rect.w - 1;
            }
            if (ball.rect.x <= 0) {
                ball.ball_speed_x = -ball.ball_speed_x;
                ball.rect.x = 1;
            }
            if (ball.rect.y <= 0) {
                ball.ball_speed_y = -ball.ball_speed_y;
                ball.rect.y = 1;
            }

            if (ball.rect.y >= HEIGHT) {
                printf("Game Over!\n");
                game = 0; 
            }

            checkBrickCollision(&allbricks, &ball);
            checkPaddleCollision(&ball, &paddle);
        }

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, am->background, NULL, NULL);
        
        for(int i = 0; i < TOTAL_BRICKS; ++i){
            if (!allbricks.bricks[i].destroyed) {
                SDL_RenderCopy(ren, allbricks.bricks[i].texture, NULL, &allbricks.bricks[i].rect);
            }
        }
        
        SDL_RenderCopy(ren, paddle.texture, NULL, &paddle.rect);
        SDL_RenderCopy(ren, ball.texture, NULL, &ball.rect);
        
        SDL_RenderPresent(ren);
    }

    free_assets(am);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit(); 
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
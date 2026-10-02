#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "snake.h"

#define TITLE "SNAKE GAME"
int width=-1;
int height=-1;
enum Starting{
    START,STOP
};
struct SnakeDirection
{
    float xDir;
    float yDir;
};

typedef struct Game{
SDL_Window* window;
SDL_Renderer* renderer;
struct Snake* snake;
float speed;
enum Starting start;
struct SnakeDirection direction;
int snakelength;
}Game;
bool Init_SDL(Game* game);
void createSnakeAt(Game* game,uint8_t size,int posx, int posy);
void fillCircle(SDL_Renderer* r, int cx, int cy, int radius);
void drawSnake(Game* game);
void moveSnake(Game* game);
void changeSnakeDirection(Game* game);
void eat(Game* game);
void destroyGame(Game* game,int exit_code);
int main(){
    Game _={
        .snakelength=3,
        .direction={.xDir=-1.0,.yDir=0.0},
        .start=STOP,
        .speed=3.0,
        .snake=NULL,
        .renderer=NULL,
        .window=NULL};
        Game* game=&_;
        bool successINit=Init_SDL(game);
        if(successINit){
        createSnakeAt(game,40,800,200);
        bool running=true;
       while (running)
       {
         SDL_Event events;
         while (SDL_PollEvent(&events))
         {
            switch (events.type)
            {
                case SDL_QUIT:
                running=false;
                break;
                case SDL_KEYDOWN:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_X:
                        running=false;
                        break;
                    case SDL_SCANCODE_E://for testing
                        eat(game);
                        break;
                    case SDL_SCANCODE_SPACE:
                        if(game->start==START){
                            game->start=STOP;
                        }else{
                            game->start=START;
                        }
                        break;
                    case SDL_SCANCODE_LEFT:
                        game->direction.xDir=-1.0;
                        game->direction.yDir=0.0;
                        changeSnakeDirection(game);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        game->direction.xDir=1.0;
                        game->direction.yDir=0.0;
                        changeSnakeDirection(game);                    
                        break;
                    case SDL_SCANCODE_UP:
                        game->direction.xDir=0.0;
                        game->direction.yDir=-1.0;
                        changeSnakeDirection(game);                    
                        break;
                    case SDL_SCANCODE_DOWN:
                        game->direction.xDir=0.0;
                        game->direction.yDir=1.0;
                        changeSnakeDirection(game);                    
                        break;
                    default:
                    break;
                }
                default:
                break;
            }
        }
         moveSnake(game);
         SDL_SetRenderDrawColor(game->renderer,40,40,40,0);
         SDL_RenderClear(game->renderer);
         drawSnake(game);
         SDL_RenderPresent(game->renderer);
         SDL_Delay(16);
         
       }
        
        //end
        destroyGame(game,EXIT_SUCCESS);
    }else{
        destroyGame(game,EXIT_FAILURE);
    }
    return 0;
}

bool Init_SDL(Game *game)
{
    int init_sdlError=SDL_Init(SDL_INIT_EVERYTHING);
    if(init_sdlError!=0){
        fprintf(stderr,"error at init sdl:%s\n",SDL_GetError());
        return false;
    }
    //fetch size
    SDL_DisplayMode displayer;
    int init_sdlDispError=  SDL_GetDesktopDisplayMode(0,&displayer);
    if(init_sdlDispError!=0){
        fprintf(stderr,"error on reading display mode for size:%s\n",SDL_GetError());
        return false;
    }
    //set width and height
    width=displayer.w - 200;
    height=displayer.h;
   //prepare window
    game->window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
    if(game->window==NULL){
        fprintf(stderr,"error at window ceation:%s\n",SDL_GetError());
        return false;
    }
    game->renderer=SDL_CreateRenderer(game->window,-1,0);
    if(game->renderer==NULL){
        fprintf(stderr,"error at renderer ceation:%s\n",SDL_GetError());
        return false;
    }
    return true;
}

void createSnakeAt(Game *game,uint8_t size,int posx, int posy)
{
    struct Snake* snake=createSmallSnake(size,posx,posy);
    game->snake=snake;

}
void fillCircle(SDL_Renderer* r, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrtf((float)(radius * radius - dy * dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void drawSnake(Game *game)
{
   
    int MaxRadius = (int)(game->snake->side_length * 0.6f);
    int i=0;
    for (struct SnakeNode* n = game->snake->head; n != NULL; n = n->next) {
        int radius=MaxRadius*(1.0 - (0.1*i)/game->snakelength);
        if (n == game->snake->head){
            SDL_SetRenderDrawColor(game->renderer, 2, 10, 10, 255);
            
        }
        else{
            SDL_SetRenderDrawColor(game->renderer, 250, 0, 0, 255);
        }
        fillCircle(game->renderer,
            (int)n->vector->centerX,
            (int)n->vector->centerY,
            radius);
            i+=1;
        }
}

void moveSnake(Game *game)
{
    if(game->start==START){
        move(game->snake,game->speed);
    }
}

void changeSnakeDirection(Game *game)
{
    if(game->start==START){
        changeDirection(game->snake,game->direction.xDir,game->direction.yDir);
    }
}

void eat(Game *game)
{
    if(game->start==START){
        game->snakelength+=1;
        addBox(game->snake);

    }
}

void destroyGame(Game *game,int exit_code)
{
    destroySnake(game->snake);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    exit(exit_code);
}

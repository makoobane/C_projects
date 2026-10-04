#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "snake.h"
#include "mouse.h"

#define MOUSE_RADIUS 50
#define SNAKE_RADIUS 40
#define TITLE "SNAKE GAME"
#define MIXER_FLAG MIX_INIT_WAVPACK
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
//mouse
struct Mouse* mouse;
//sound
Mix_Chunk* eatSound;
}Game;
bool Init_SDL(Game* game);
void createSnakeAt(Game* game,int posx, int posy);
void fillCircle(SDL_Renderer* r, int x, int y, int radius);
void drawSnake(Game* game);
void moveSnake(Game* game);
void changeSnakeDirection(Game* game);
//create mouse
void createMouseWithSize(Game* game,uint8_t size);
void showMouse(Game* game);
void killMouse(Game* game);
//eat mouse
bool doesSnakeBitenMouse(Game* game);
void eatMouse(Game* game);
//end game and clean
void destroyGame(Game* game,int exit_code);
int main(){
    srand(time(NULL));
    Game _={
        
        .eatSound=NULL,
        .mouse=NULL,
        .snakelength=0,
        .direction={.xDir=-1.0,.yDir=0.0},
        .start=STOP,
        .speed=3.0,
        .snake=NULL,
        .renderer=NULL,
        .window=NULL};
        Game* game=&_;
        bool successINit=Init_SDL(game);
        if(successINit){
        //create snake
        createSnakeAt(game,800,200);
        //create mouse initially
        createMouseWithSize(game,MOUSE_RADIUS);
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
         showMouse(game);
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
    int init_sdlError=SDL_Init(SDL_INIT_VIDEO);
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
    // //init mixer
    // int mixflag= Mix_Init(MIXER_FLAG);
    // if((mixflag&MIXER_FLAG) !=MIXER_FLAG){
    //     fprintf(stderr,"error on mixer init:%s\n",Mix_GetError());
    //     return false;
    // }
    //openAudio
    int errAudio=Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    if(errAudio!=0){
        fprintf(stderr,"errorn on open audio:%s\n",Mix_GetError());
        return false;
    }
    //load sound
    game->eatSound=Mix_LoadWAV("eat.wav");
    if(game->eatSound==NULL){
        fprintf(stderr,"error on load wav file:%s\n",Mix_GetError());
        return false;
    }
    return true;
}

void createSnakeAt(Game *game,int posx, int posy)
{
    struct Snake* snake=createSmallSnake(SNAKE_RADIUS,posx,posy);
    if(snake==NULL){
        puts("failed to create snake");
    }else{
        game->snakelength=3;
        game->snake=snake;
    }

}
void fillCircle(SDL_Renderer* r, int x, int y, int radius)
{
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrtf((float)(radius * radius - dy * dy));
        SDL_RenderDrawLine(r, x - dx, y + dy, x + dx, y + dy);
    }
}

void drawSnake(Game *game)
{
   
    int MaxRadius = (int)(game->snake->side_length*0.6f);
    int i=0;
    for (struct SnakeNode* n = game->snake->head; n != NULL; n = n->next) {
        int radius=MaxRadius*(1.0 - (0.3*i)/game->snakelength);
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
        bool mouseBitten=doesSnakeBitenMouse(game);
        if(mouseBitten){
            killMouse(game);
            Mix_PlayChannel(-1,game->eatSound,0);
            eatMouse(game);
            createMouseWithSize(game,MOUSE_RADIUS);
        }
        
    }
}

void changeSnakeDirection(Game *game)
{
    if(game->start==START){
        changeDirection(game->snake,game->direction.xDir,game->direction.yDir);
    }
}

void createMouseWithSize(Game* game,uint8_t size)
{
    struct Mouse* mouse=createMouseAtRandomPositionIn(width,height,size);
    if(mouse!=NULL){
        game->mouse=mouse;
    }else{
        puts("error on creating mouse");
    }
}

void showMouse(Game *game)
{
    if(game->mouse!=NULL){
        SDL_SetRenderDrawColor(game->renderer,100,250,40,255);
        fillCircle(game->renderer,game->mouse->xPosition,game->mouse->yPosition,game->mouse->size);
    }
}

void killMouse(Game *game)
{
    destroyMouse(game->mouse);
    game->mouse=NULL;
}
bool doesSnakeBitenMouse(Game *game)
{
    if (game->mouse == NULL) return false;
    float headR = game->snake->side_length * 0.6f;
    float minDist = game->mouse->size + headR;
    float dx = game->mouse->xPosition - game->snake->head->vector->centerX;
    float dy = game->mouse->yPosition - game->snake->head->vector->centerY;
    return dx * dx + dy * dy <= minDist * minDist;
}

void eatMouse(Game *game)
{
    if(game->start==START){
        game->snakelength+=1;
        addBox(game->snake);
    }
}

void destroyGame(Game *game,int exit_code)
{
    Mix_HaltChannel(-1);
    Mix_FreeChunk(game->eatSound);
    Mix_CloseAudio();
    // Mix_Quit();

    destroyMouse(game->mouse);
    destroySnake(game->snake);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    exit(exit_code);
}

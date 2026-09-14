#include<stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#define Window_title "open window"
#define windows_width 600
#define windows_height 600
struct Game{
SDL_Window * window;
SDL_Renderer * renderer;
};
bool initialize_SDL(struct Game* game);
void freeing(struct Game* game,int exit_status);
int main(){
    struct Game game={.window=NULL,.renderer=NULL,};
    bool initflag=initialize_SDL(&game);
    //NOTE: init flag will be true if main init or renderer or windows one of them fails so it is true on error state
    if(initflag==true){
        //error occured 
        freeing(&game,EXIT_FAILURE);
    }//else if there is no error on initializing it will return false then we will continue 
    while (true)// keep screen alive
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                //you can close screen while closing
            case SDL_QUIT:
                freeing(&game,EXIT_SUCCESS);
                break;
                //or click M
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_M:
                    freeing(&game,EXIT_SUCCESS);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        
        SDL_RenderClear(game.renderer);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(16); //16X60 frames/second=960ms so it fit for 60fps
    }
    
    puts("end");
    freeing(&game,EXIT_SUCCESS); 
    return 0;
}
bool initialize_SDL(struct Game* game){
    //initailize sdl
    int error=SDL_Init(SDL_INIT_EVERYTHING);
    if(error!=0){
        fprintf(stderr, "error happened at main initalization off sdl: %s\n",SDL_GetError());
        return true;
    }
    //create window
    game->window=SDL_CreateWindow(Window_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windows_width,windows_height,0);
    if(game->window==NULL){
        fprintf(stderr,"Error happened at window creation:%s\n",SDL_GetError());
        return true;
    }
    //create renderer on window
    game->renderer=SDL_CreateRenderer(game->window,-1,0);
    if(game->renderer==NULL){
        fprintf(stderr,"error happened at render creation: %s\n",SDL_GetError());
        return true;
    }
    return false;
}
void freeing(struct Game* game,int exit_status){
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    exit(exit_status);
}
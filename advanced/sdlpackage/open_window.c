#include<stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
struct Game{
SDL_Window * window;
SDL_Renderer * renderer;
};
bool initialize_SDL(struct Game* game);
int main(){
    
    return 0;
}
bool initialize_SDL(struct Game* game){
    int error=SDL_Init(SDL_INIT_EVERYTHING);
    if(error==0){
        return true;
    }else{
        fprintf(stderr,"error happened at initialization: %s\n",SDL_GetError);
        return false;
    }
}
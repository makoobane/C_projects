#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#define WIDTH 900
#define HEIGHT 600
#define TITLE "Image Viewer"

typedef struct Screen{
SDL_Window* window;
SDL_Surface* surface;
}Screen;

bool init_SDL(Screen * screen);
void destroyAll(Screen* screen,int exit_code);
int main(){
    Screen screen={.surface=NULL,.window=NULL};
    bool initSuccess=init_SDL(&screen);
    if(initSuccess){
        printf("bismillah\n");
       //do stuff
        SDL_FillRect(screen.surface,NULL,0X0000FFFF);//fill red rect
        SDL_UpdateWindowSurface(screen.window);//update change
       //delay to exist
       SDL_Delay(7000);
       //destroy finally
       destroyAll(&screen,EXIT_SUCCESS);

    }else{
        destroyAll(&screen,EXIT_FAILURE);
    }
    return 0;
}

bool init_SDL(Screen* screen)
{
    int init_error=SDL_Init(SDL_INIT_EVERYTHING);
    if(init_error!=0){
        //error exists
        fprintf(stderr,"error on init_SDL:%s\n",SDL_GetError());
        return false;
    }
   screen-> window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
    if(screen->window==NULL){
        fprintf(stderr,"error on window:%s\n",SDL_GetError());
        return false;
    }
   screen-> surface=SDL_GetWindowSurface(screen->window);
    if(screen->surface==NULL){
        fprintf(stderr,"error on render creation:%s\n",SDL_GetError());
        return false;
    }

    return true;
    
}

void destroyAll(Screen* screen,int exit_code)
{
    SDL_FreeSurface(screen->surface);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
    exit(exit_code);
}

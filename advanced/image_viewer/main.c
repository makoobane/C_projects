#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#define WIDTH 900
#define HEIGHT 600
#define TITLE "Image Viewer"

typedef struct Screen{
SDL_Window* window;
SDL_Surface* surface;
}Screen;

bool init_SDL(Screen * screen);
void fillColor(Screen* screen,uint32_t color);
void destroyAll(Screen* screen,int exit_code);
int main(){
    Screen screen={.surface=NULL,.window=NULL};
    bool initSuccess=init_SDL(&screen);
    if(initSuccess){
        printf("bismillah\n");
       //do stuff
       uint8_t r=0XFF;
       uint8_t g,b=0;
       uint8_t a=0XFF;//no opacity
       uint32_t color =SDL_MapRGBA( screen.surface->format,r,g,b,a);
        // SDL_FillRect(screen.surface,NULL,color);//fill red rect whole screen
        //or fill it with pixel by pixel
        fillColor(&screen,color);
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

void fillColor(Screen* screen,uint32_t color)
{
    SDL_Rect pixel={0,0,.w=1,.h=1};//1 pixel rect size
    for(int x=0;x<WIDTH;x++){
        for(int y=0;y<HEIGHT;y++){
            pixel.x=x;
            pixel.y=y;
            SDL_FillRect(screen->surface,&pixel,color);
        }
    }
}

void destroyAll(Screen* screen,int exit_code)
{
    SDL_FreeSurface(screen->surface);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
    exit(exit_code);
}

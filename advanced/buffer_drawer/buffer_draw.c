#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#define TITLE "BUFFER DRAWER"
#define WIDTH 800
#define HEIGHT 600

 typedef struct FrameBuffer
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* buffer;
}FrameBuffer;

bool SDL_Initialize(FrameBuffer* buffer);
void freeing(FrameBuffer* buffer,int exit_code);
int main(){
    FrameBuffer buffer={
     .window=NULL,
     .renderer=NULL,
     .texture=NULL,
     .buffer=NULL,
    };
    bool init_success= SDL_Initialize(&buffer);
    if(init_success==false){
        freeing(&buffer,EXIT_FAILURE);
    }
    bool running=true;
    while(running){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
           switch (event.type)
           {
           case SDL_QUIT:
                running=false;
                freeing(&buffer,EXIT_SUCCESS);
            break;
           
           default:
            break;
           }
        }
        
        SDL_RenderClear(buffer.renderer);
        SDL_UpdateTexture(buffer.texture,NULL,buffer.buffer,WIDTH);
        SDL_RenderPresent(buffer.renderer);
        SDL_Delay(30); //30fps
    }


    return 0;
}

bool SDL_Initialize(FrameBuffer *buffer)
{
   int init_err=SDL_Init(SDL_INIT_VIDEO);
   if(init_err!=0){
       fprintf(stderr,"error at init_sdl:%s\n",SDL_GetError());
       return false;
   }
   SDL_Window* window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
   if(window==NULL){
       fprintf(stderr,"error on window creation:%s\n",SDL_GetError());
       return false;
   }
   buffer->window=window;
   SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,0);
   if(renderer==NULL){
       fprintf(stderr,"error on creation of renderer:%s\n",SDL_GetError());
       return false;
   }
   buffer->renderer=renderer;
   SDL_Texture* texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STATIC,WIDTH,HEIGHT);
   if(texture==NULL){
       fprintf(stderr,"error on texture creation:%s\n",SDL_GetError());
       return false;
   }
   buffer->texture=texture;
   //initialize buffer
   uint32_t* bufferpixels=malloc(WIDTH*HEIGHT*sizeof(uint32_t));
   if(bufferpixels==NULL){
      printf("error on bufffer allocation");
      return false;
   }
   buffer->buffer=bufferpixels;
 
   return true;
}

void freeing(FrameBuffer *buffer,int exit_code)
{
    free(buffer->buffer);
    SDL_DestroyTexture(buffer->texture);
    SDL_DestroyRenderer(buffer->renderer);
    SDL_DestroyWindow(buffer->window);
    SDL_Quit();
    exit(exit_code);
}

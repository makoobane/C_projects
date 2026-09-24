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
void put_colorAt(int x, int y , uint32_t color,FrameBuffer* buffer);
void fillColor(uint32_t color,FrameBuffer* buffer);
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
    int frame=0;
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
           case SDL_KEYDOWN:
               switch (event.key.keysym.scancode)
               {
               case SDL_SCANCODE_SPACE:
                   put_colorAt(100,100,0XFF000000,&buffer);
                   break;
                case SDL_SCANCODE_F:
                   fillColor(0X00000000,&buffer);
                   break;
                   
                   default:
                   break;
                }
                break;
                default:
                break;
            }
        }
        int x=frame%WIDTH;
        int y=HEIGHT/2;
        put_colorAt(x,y,0XFF00FF,&buffer);
        SDL_UpdateTexture(buffer.texture,NULL,buffer.buffer,WIDTH*sizeof(uint32_t));
        //this clears previus draws
        SDL_RenderClear(buffer.renderer);
        //that draws
        SDL_RenderCopy(buffer.renderer,buffer.texture,NULL,NULL);
        //this shows
        SDL_RenderPresent(buffer.renderer);
        //this keeps
        SDL_Delay(16);
        frame+=1;
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

void put_colorAt(int x, int y, uint32_t color,FrameBuffer* buffer)
{
    if(x<0||x>=WIDTH||y<0||y>=HEIGHT){
        puts("out of range");
        return;
    }
    buffer->buffer[WIDTH*y+x]=color;
}

void fillColor(uint32_t color,FrameBuffer* buffer)
{
    for(int i=0;i<WIDTH*HEIGHT;i++){
        buffer->buffer[i]=color;
    }
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

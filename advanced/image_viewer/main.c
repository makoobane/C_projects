#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#define TITLE "Image Viewer"

int width=-1;
int height=-1;
typedef struct Screen{
    SDL_Window* window;
SDL_Surface* surface;
uint8_t* pixels;
}Screen;
uint8_t* readPPMGetDimensionsAndData(const char* filename);
bool init_SDL(Screen * screen);
void fillColor(Screen* screen,uint32_t color);
void drawPixels(Screen* screen);
void destroyAll(Screen* screen,int exit_code);
int main(){
    Screen screen={.surface=NULL,.window=NULL,.pixels=NULL};
    screen.pixels = readPPMGetDimensionsAndData("giraffe.ppm");
    bool initSuccess=init_SDL(&screen);
    if(initSuccess){
       //do stuff
    //    uint8_t r=0XFF;
    //    uint8_t g=0;
    //    uint8_t b=0;
    //    uint8_t a=0XFF;//no opacity
    //    uint32_t color =SDL_MapRGBA( screen.surface->format,r,g,b,a);
    //    fillColor(&screen,color);
       
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
   screen-> window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
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
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            pixel.x=x;
            pixel.y=y;
            SDL_FillRect(screen->surface,&pixel,color);
        }
    }
}

void drawPixels(Screen *screen)
{
     SDL_Rect pixel={0,0,.w=1,.h=1};//1 pixel rect size
     uint32_t color=0;
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            uint8_t r,g,b;
            //how to get color from pixels ?
            pixel.x=x;
            pixel.y=y;
            color=SDL_MapRGB(screen->surface->format,r,g,b);
            SDL_FillRect(screen->surface,&pixel,color);
        }
    }
}

uint8_t* readPPMGetDimensionsAndData(const char *filename) {
    FILE* fptr = fopen(filename, "rb");   // "rb" — binary mode, matters for portability
    if (fptr == NULL) {
        puts("it failed to read dimensions");
        exit(1);
    }

    char encodingType[10];
    fgets(encodingType, sizeof(encodingType), fptr);

    char line[100];
    fgets(line, sizeof(line), fptr);
    bool commentFound = (strncmp(line, "#", 1) == 0);
    if (commentFound) {
        printf("comment is: %s", line);
        fgets(line, sizeof(line), fptr);   // now this line should be dimensions
    }
    sscanf(line, "%d %d", &width, &height);

    char maxval[16];
    fgets(maxval, sizeof(maxval), fptr);   // big enough buffer avoids the truncation bug
    // no separate fgetc needed now — fgets with a proper size consumes the \n itself

    size_t data_size = (size_t)width * height * 3;
    uint8_t* pixels = malloc(data_size);
    if (pixels == NULL) {
        fputs("allocation failed\n", stderr);
        fclose(fptr);
        exit(1);
    }

    size_t read_count = fread(pixels, 1, data_size, fptr);
    if (read_count != data_size) {
        fprintf(stderr, "expected %zu bytes, got %zu\n", data_size, read_count);
    }

    
    fclose(fptr);
    // pixels now holds raw R,G,B,R,G,B,... — use it, then:
    return pixels;
}

void destroyAll(Screen* screen,int exit_code)
{
    free(screen->pixels);
    SDL_FreeSurface(screen->surface);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
    exit(exit_code);
}

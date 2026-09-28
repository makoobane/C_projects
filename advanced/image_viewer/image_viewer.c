#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#define TITLE "IViewer"


int width=-1;
int height=-1;
typedef struct RGBColor {
    uint8_t r, g, b;
} RGBColor;

typedef struct Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_PixelFormat* format;
    SDL_Texture * image_texture;
    RGBColor* pixels;
    uint32_t* colors;
} Screen;
RGBColor* readPPMGetDimensionsAndData(const char* filename);
uint32_t* mapPixelsToColor(RGBColor* pixels,SDL_PixelFormat* format);
bool init_SDL(Screen * screen);

void destroyAll(Screen* screen,int exit_code);
int main(){
    Screen screen={.renderer=NULL,.window=NULL,.pixels=NULL,.colors=NULL};
    screen.pixels = readPPMGetDimensionsAndData("giraffe.ppm");
    bool initSuccess=init_SDL(&screen);
    if(initSuccess){
       screen.colors=mapPixelsToColor(screen.pixels,screen.format);
       SDL_UpdateTexture(screen.image_texture,NULL,screen.colors,width*sizeof(uint32_t));
       while (true)
       {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                destroyAll(&screen,EXIT_SUCCESS);
                break;
            default:
                break;
            }
        }
        SDL_RenderClear(screen.renderer);
        //NULLs bcs they have same size
        SDL_RenderCopy(screen.renderer,screen.image_texture,NULL,NULL);
        SDL_RenderPresent(screen.renderer);
        SDL_Delay(100);
       }
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
    screen->renderer=SDL_CreateRenderer(screen->window,-1,0);
    if(screen->renderer==NULL){
        fprintf(stderr,"ereror on render creation:%s\n",SDL_GetError());
        return false;
    }
    uint32_t format=SDL_PIXELFORMAT_RGBA8888;
    screen->format = SDL_AllocFormat(format);
    if(screen->format==NULL){
        fprintf(stderr,"error on format:%s\n",SDL_GetError());
        return false;
    }
    screen->image_texture=SDL_CreateTexture(screen->renderer,format,SDL_TEXTUREACCESS_STATIC,width,height);
    if(screen->image_texture==NULL){
        fprintf(stderr,"error on texture creation:%s\n",SDL_GetError());
        return false;
    }
    
    return true;
    
}



RGBColor* readPPMGetDimensionsAndData(const char *filename) {
    FILE* fptr = fopen(filename, "rb");
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
        fgets(line, sizeof(line), fptr);
    }
    sscanf(line, "%d %d", &width, &height);

    char maxval[16];
    fgets(maxval, sizeof(maxval), fptr);

    size_t pixel_count = (size_t)width * height;
    size_t raw_size = pixel_count * 3;

    uint8_t* raw = malloc(raw_size);
    if (raw == NULL) {
        fputs("raw allocation failed\n", stderr);
        fclose(fptr);
        exit(1);
    }

    size_t read_count = fread(raw, 1, raw_size, fptr);
    fclose(fptr);
    if (read_count != raw_size) {
        fprintf(stderr, "expected %zu bytes, got %zu\n", raw_size, read_count);
    }

    RGBColor* pixels = malloc(pixel_count * sizeof(RGBColor));
    if (pixels == NULL) {
        fputs("pixel allocation failed\n", stderr);
        free(raw);
        exit(1);
    }

    for (size_t i = 0; i < pixel_count; i++) {
        pixels[i].r = raw[i * 3 + 0];
        pixels[i].g = raw[i * 3 + 1];
        pixels[i].b = raw[i * 3 + 2];
    }

    free(raw); 
    return pixels;
}


uint32_t *mapPixelsToColor(RGBColor *pixels,SDL_PixelFormat* format)
{
    uint32_t* colors=calloc(width*height,sizeof(uint32_t));
    for(int y=0;y<height;y++){
     for(int x=0;x<width;x++){
        RGBColor color=pixels[y*width+x];
        colors[y*width+x]=SDL_MapRGB(format,color.r,color.g,color.b);
     }
    }
    return colors;
}
void destroyAll(Screen* screen,int exit_code)
{
    free(screen->colors);
    free(screen->pixels);
    SDL_FreeFormat(screen->format);
    SDL_DestroyTexture(screen->image_texture);
    SDL_DestroyRenderer(screen->renderer);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
    exit(exit_code);
}

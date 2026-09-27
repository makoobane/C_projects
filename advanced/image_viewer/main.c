#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#define TITLE "Image Viewer"

int width=-1;
int height=-1;
typedef struct RGBColor {
    uint8_t r, g, b;
} RGBColor;

typedef struct Screen {
    SDL_Window* window;
    SDL_Surface* surface;
    RGBColor* pixels;
} Screen;
RGBColor* readPPMGetDimensionsAndData(const char* filename);
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
       drawPixels(&screen);
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
    SDL_Rect pixel = {0, 0, .w = 1, .h = 1};
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBColor c = screen->pixels[y * width + x];   // row-major index
            uint32_t color = SDL_MapRGB(screen->surface->format, c.r, c.g, c.b);
            pixel.x = x;
            pixel.y = y;
            SDL_FillRect(screen->surface, &pixel, color);
        }
    }
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

    free(raw);   // done with the flat buffer now that it's copied into structs
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

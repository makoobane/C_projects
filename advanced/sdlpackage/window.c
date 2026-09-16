#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define Window_title "window"
#define IMAGE_FLAGS IMG_INIT_PNG
#define FONT_SIZE 80
#define windows_width 800
#define windows_height 600

struct Game{
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * background;
TTF_Font* font;
SDL_Color text_color;
SDL_Rect text_rect;
SDL_Texture* text_texture;

};
bool initialize_SDL(struct Game* game);
bool load_media(struct Game* game);
void freeing(struct Game* game,int exit_status);
int main(){
    // const SDL_Rect rect={350,250,100,100};
    struct Game game={.window=NULL,
        .text_texture=NULL,
        .renderer=NULL,.background=NULL,.font=NULL,.text_color={.r=255,.g=255,.b=255,.a=255},.text_rect={0,0,0,0}};
    bool initflag=initialize_SDL(&game);
    srand(time(NULL));
    //NOTE: init flag will be true if main init or renderer or windows one of them fails so it is true on error state
    if(initflag==true){
        //error occured 
        freeing(&game,EXIT_FAILURE);
    }//else if there is no error on initializing it will return false then we will continue 
    //check if background image is failed
    bool isLoadingbcgfailed=load_media(&game);
    if(isLoadingbcgfailed){
        freeing(&game,EXIT_FAILURE);
    }
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
                case SDL_SCANCODE_SPACE://if space bar is clicked
                  unsigned short int r=rand()%255;
                  unsigned short int g=rand()%255;
                  unsigned short int b=rand()%255;
                  unsigned short int a=rand()%255;
                  SDL_SetRenderDrawColor(game.renderer,r,g,b,a);//set color of background to red;
                    break;

                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        
        SDL_RenderClear(game.renderer);//delete each frame to render another
        SDL_RenderCopy(game.renderer,game.background,NULL,NULL);//sets background image
        SDL_RenderCopy(game.renderer,game.text_texture,NULL, &(game.text_rect));
        SDL_RenderPresent(game.renderer);//draw every frame again
        SDL_Delay(16); //16X60 frames/second=960ms so it fit for 60fps
    }
    
    freeing(&game,EXIT_SUCCESS); 
    return 0;
}
bool initialize_SDL(struct Game* game){
  
    //initailize sdl
    int error=SDL_Init(SDL_INIT_VIDEO);
    if(error!=0){
        fprintf(stderr, "error happened at main initalization off sdl: %s\n",SDL_GetError());
        return true;
    }
    //initialize background image
    /* Image flag works like this : there are many image types like jpeg,png,...bit . then we want to present jpeg as 1000 
    png as 0100 ... then when we want png we may use 4 or (0100) as flag then it will return 0100 & 1111 if all 4 types where present
    . then the result must be same as imageflag*/
    int init_flag=IMG_Init(IMAGE_FLAGS);
    if((init_flag & IMAGE_FLAGS)!=IMAGE_FLAGS){//0100&0100=0100 if not it is error
        fprintf(stderr,"error ocuured at image initialization: %s\n",IMG_GetError());
        return true;
    }
    //initialize text ttf
   short int ttf_i= TTF_Init();//0 for success and -1 for error
    if(ttf_i==-1){
        fprintf(stderr,"it failed ttf init:%s\n",TTF_GetError());
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
bool load_media(struct Game *game)
{
    game->background=  IMG_LoadTexture(game->renderer,"images/background.png");
    if(game->background==NULL){
        fprintf(stderr,"loading image failed: %s\n",IMG_GetError());
        return true;
    }
    //oopen font and make sdl_font
    game->font= TTF_OpenFont("fonts/freesansbold.ttf",FONT_SIZE);
    if(game->font==NULL){
        fprintf(stderr,"loading font failed:%s\n",TTF_GetError());
        return true;
    }
    //convert it to surface based on that font size and text
    SDL_Surface* surface=TTF_RenderText_Blended(game->font,"Hello",game->text_color);
    if(surface==NULL){
        fprintf(stderr,"error on rendertext:%s\n",SDL_GetError());
        return true;
    }
    //convert it textue
    game->text_texture=SDL_CreateTextureFromSurface(game->renderer,surface);
    //keeep your text size from that surface
    game->text_rect.w=surface->w;
    game->text_rect.h=surface->h;
    //clean that surface dont store it
    SDL_FreeSurface(surface);
    if(game->text_texture==NULL){
        fprintf(stderr,"error on texture from surface:%s\n",SDL_GetError());
        return true;
    }
    return false;
}
void freeing(struct Game *game, int exit_status)
{
    SDL_DestroyTexture(game->text_texture);
    TTF_CloseFont(game->font);
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    exit(exit_status);
}
/*in IMG of sdl to present image you need just
1)initialize img
2)load image wwith render and it will return texture
3)use texture
4)destroy texture and quit img
 */

/* in TTF to present text you need to take those steps:
1) init ttf
2)open font and make font
3)convert that font to surface and keep size
4)convert surface to texture
5)rendercopy that new text_texture
//....de allocating
6)free surface
7)close font
8)destroytexture
9)quit ttf
*/
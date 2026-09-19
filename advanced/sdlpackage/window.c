#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#define Window_title "window"
#define IMAGE_FLAGS IMG_INIT_PNG
#define MIXER_FLAGS MIX_INIT_OGG
#define FONT_SIZE 80


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
/* in SDL2_mixer to play sound you need this below:
1) Mix init to initialize it with flag
2)open Audio which needs frequancey, channel,format,
3)load chunk which needs file path of sound file
4) playChannel to produce that sound it takes chunk* and -1 for first channel and 0 for not looping
..........de allocatin
5)haltchannel (-1 for all channels)
6)freechunck
7)close audio
8)quit mixer

also in this library to play music you need:
1)init mixer
2)load music file to mix_music*
3) play music while giving -1 for looping but it must be outside of the game loop
//NOTE: you need to have abitity to pause it when needed
//de allocating
4)halt music
4)free music

*/
int  windows_width =800;
int windows_height =600;

struct Game{
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * background;
TTF_Font* font;
SDL_Color text_color;
SDL_Rect text_rect;
SDL_Texture* text_texture;
bool inText;
SDL_Texture* sprite_image;
SDL_Rect sprite_position;
int sprite_speed;
const uint8_t* keyState;
Mix_Chunk* sound;
Mix_Music* music;
};
bool initialize_SDL(struct Game* game);
bool load_media(struct Game* game);
void updateSprite(struct Game* game);
void freeing(struct Game* game,int exit_status);
int main(){
    // const SDL_Rect rect={350,250,100,100};
    struct Game game={.window=NULL,
        .text_texture=NULL,
        .renderer=NULL,
        .background=NULL,
        .font=NULL,
        .text_color={.r=255,.g=255,.b=255,.a=255},
        .text_rect={0,0,0,0},
        .inText=false,
        .sprite_image=NULL,
        .sprite_position={.x=0,.y=0,.w=0,.h=0},
        .sprite_speed=5,
        .sound=NULL,
        .keyState=SDL_GetKeyboardState(NULL)};
        srand(time(NULL));
        bool initflag=initialize_SDL(&game);
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
        //play background music
        int perr=Mix_PlayMusic(game.music,-1);//-1 means loop infinitely
    if(perr){
        fprintf(stderr,"music play of the background failed:%s\n",Mix_GetError());
        freeing(&game,EXIT_FAILURE);
    }
    printf("current driver :%s\n",SDL_GetCurrentVideoDriver());
    //game loop
    bool running =true;
    while (running)// keep screen alive
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))//listen events in every 16ms 
        {
            switch (event.type)//separate event based on their types like draging,close window,click key
            {
                //you can close screen while closing
            case SDL_QUIT:
                running=false;
                freeing(&game,EXIT_SUCCESS);
                break;
                //if mouse started draging or pushing
            case SDL_MOUSEBUTTONDOWN:
                puts("down");
                bool isInText=false;
                bool xIn=(game.text_rect.w+game.text_rect.x)>event.motion.x&&event.motion.x> game.text_rect.x;
                bool yIn=(game.text_rect.h+game.text_rect.y)>event.motion.y && event.motion.y>game.text_rect.y;
                if(xIn && yIn){
                    puts("it is in");
                    game.inText=true;// you are on texx
                }
                 break;
            case SDL_MOUSEMOTION:
                 if(game.inText){
                     //instead of top right corner this center make center of the text to be moving point
                     int centerx=game.text_rect.w/2;
                     int centery=game.text_rect.h/2;
                     int x=  event.motion.x -centerx;
                     int y= event.motion.y -centery;
                    bool Xinborder=(x<(windows_width - game.text_rect.w))&&x>0;
                    if(Xinborder){
                    game.text_rect.x=x;//move text rect in x
                    }
                    bool Yinborder=(y<(windows_height-game.text_rect.h))&&y>0;
                    if(Yinborder){
                        game.text_rect.y=y;//move text rect in y
                    }
                 }
                 break;
            case SDL_MOUSEBUTTONUP:
             
                 game.inText=false;//draging ended
                 break;
                 case SDL_KEYDOWN://if event is key is pressed
                 switch (event.key.keysym.scancode)//separate keys based on their scancode
                 {
                 case SDL_SCANCODE_M:
                     //or click M
                    running=false;
                    freeing(&game,EXIT_SUCCESS);
                    break;
                case SDL_SCANCODE_SPACE://if space bar is clicked
                  unsigned short int r=rand()%255;
                  unsigned short int g=rand()%255;
                  unsigned short int b=rand()%255;
                  unsigned short int a=rand()%255;
                  SDL_SetRenderDrawColor(game.renderer,r,g,b,a);//set color of background to red;
                  Mix_PlayChannel(-1,game.sound,0);// this sound will not be stopped by the loop
                    break;
                case SDL_SCANCODE_P:
                    int paused=Mix_PausedMusic();//1 for pause 0 for not paused
                    if(paused==1){
                       Mix_ResumeMusic();
                    }else{//not paussed
                        
                       Mix_PauseMusic();
                    }
                    break;
                default:
                    break;
                }
                break;
            case SDL_WINDOWEVENT://if it is window event
                if(event.window.event==SDL_WINDOWEVENT_RESIZED){// and it is rezised resize
                    windows_width=event.window.data1;
                    windows_height=event.window.data2;
                }
                break;
            default:
                break;
            }
        }
        //update sprite position by listening which key is pressed
        updateSprite(&game);
        //delete each frame to render another
        SDL_RenderClear(game.renderer);
        //sets background image
        SDL_RenderCopy(game.renderer,game.background,NULL,NULL);
        //render text
        SDL_RenderCopy(game.renderer,game.text_texture,NULL, &(game.text_rect));
        //render sprite
        SDL_RenderCopy(game.renderer,game.sprite_image,NULL,&(game.sprite_position));
        //draw every frame again
        SDL_RenderPresent(game.renderer);
        //16X60 frames/second=960ms so it fit for 60fps
        SDL_Delay(16); 
    }
    
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
    //initialize background image
    /* Image flag works like this : there are many image types like jpeg,png,...bit . then we want to present jpeg as 1000 
    png as 0100 ... then when we want png we may use 4 or (0100) as flag then it will return 0100 & 1111 if all 4 types where present
    . then the result must be same as imageflag*/
    int init_flag=IMG_Init(IMAGE_FLAGS);
    if((init_flag & IMAGE_FLAGS)!=IMAGE_FLAGS){//0100&0100=0100 if not it is error
        fprintf(stderr,"error ocuured at image initialization: %s\n",IMG_GetError());
        return true;
    }
    //initialize mixer
    int flag= Mix_Init(MIXER_FLAGS);
    if((flag&MIXER_FLAGS)!=MIXER_FLAGS){
       fprintf(stderr,"error happended at mixer init:%s\n",Mix_GetError());
       return true;
    }
    //initialize text ttf
   short int ttf_i= TTF_Init();//0 for success and -1 for error
    if(ttf_i==-1){
        fprintf(stderr,"it failed ttf init:%s\n",TTF_GetError());
        return true;
    }
    //create window
    game->window=SDL_CreateWindow(Window_title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windows_width,windows_height,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
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
    //set iccon
    SDL_Surface* icon=IMG_Load("images/c.png");
    if(icon==NULL){
        fprintf(stderr,"error at icon making:%s\n",IMG_GetError());
        return true;
    }
    SDL_SetWindowIcon(game->window,icon);
    SDL_FreeSurface(icon);// you free it after use
    return false;
}
bool load_media(struct Game *game)
{
    game->background=  IMG_LoadTexture(game->renderer,"images/background.png");
    if(game->background==NULL){
        fprintf(stderr,"loading image failed: %s\n",IMG_GetError());
        return true;
    }
    //load sprite image
    game->sprite_image=IMG_LoadTexture(game->renderer,"images/c.png");
    if(game->sprite_image==NULL){
        fprintf(stderr,"error happened at sprite load:%s\n",IMG_GetError());
        return true;
    }
    //query texture: 0 on success and negative on error
    int query=SDL_QueryTexture(game->sprite_image,NULL,NULL,&(game->sprite_position.w),&(game->sprite_position.h));
    if(query){//negative
        fprintf(stderr,"error happened at query:%s\n",SDL_GetError());
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
    //open mixer audio
    int err= Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    if(err!=0){
        fprintf(stderr,"error at openAudo:%s\n",Mix_GetError());
        return true;
    }
   //load sound file
   game->sound=Mix_LoadWAV("sounds/SDL.ogg");
   if(game->sound==NULL){
    fprintf(stderr,"it failed sound loading:%s\n",Mix_GetError());
    return true;
   }
   game->music=Mix_LoadMUS("music/freesoftwaresong-8bit.ogg");
   if(game->music==NULL){
    fprintf(stderr,"error at music file load:%s\n",Mix_GetError());
    return true;
   }
    return false;
}
void updateSprite(struct Game* game){
    
    if(game->keyState[SDL_SCANCODE_LEFT]){
        game->sprite_position.x-=game->sprite_speed;
    }
    if(game->keyState[SDL_SCANCODE_RIGHT]){
        game->sprite_position.x+=game->sprite_speed;
    }
 
    if(game->keyState[SDL_SCANCODE_UP]){
        game->sprite_position.y-=game->sprite_speed;
    }
    if(game->keyState[SDL_SCANCODE_DOWN]){
        game->sprite_position.y+=game->sprite_speed;
    
}
}
void freeing(struct Game *game, int exit_status)
{
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    TTF_CloseFont(game->font);
    Mix_FreeMusic(game->music);
    Mix_FreeChunk(game->sound);
    SDL_DestroyTexture(game->sprite_image);
    SDL_DestroyTexture(game->text_texture);
    SDL_DestroyTexture(game->background);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    exit(exit_status);
}


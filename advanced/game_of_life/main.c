#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
struct Box
{
    bool isAlive;
};
enum GameState{
    PUTTING=1,
    RUNNING=2
};
struct Game{
    SDL_Window * window;
    SDL_Renderer* renderer;
    struct Box** boxes;
    int how_many_boxes_in_side;
    int width;
    int height;
    int s_side_of_box;
    int padding;//padding between boxes and their outside
    enum GameState state;
};
bool initializeGame(struct Game* game);
void drawGridAndFillAliveOnes(struct Game* game);
void freeing(struct Game* game,int exit_status);
int main(){
    struct Game game={
        .state=PUTTING,
        .padding=1,
        .how_many_boxes_in_side=20,// try 20 first
        .s_side_of_box=0,
        .width=600,
        .height=600,
        .window=NULL,
        .renderer=NULL,
        .boxes=NULL};
    bool success=initializeGame(&game);
    if(success==false){
        freeing(&game,EXIT_FAILURE);
    }
    while(true){
        //listen event;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {  
            switch (event.type)
            {
                case SDL_QUIT:
                   puts("quit detected");
                   freeing(&game,EXIT_SUCCESS);
                   break;
                case SDL_MOUSEBUTTONDOWN:
                    if(game.state==PUTTING){
                         int x=event.motion.x;
                         int y=event.motion.y;
                         int col=x/game.s_side_of_box;
                         int row=y/game.s_side_of_box;
                    
                         game.boxes[row][col].isAlive=true;
                    }
                    break;
                case SDL_KEYDOWN://listen key pressed
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_Q://Q for quiting game
                        puts("quit detected");
                        freeing(&game,EXIT_SUCCESS);
                        break;
                    // if space is clicked then we want to change state of the game
                    case SDL_SCANCODE_SPACE:
                        if(game.state==1) game.state=RUNNING;
                        if(game.state==2) game.state=PUTTING;
                        break;
                    default:
                        break;
                }
                    break;
                default:
                    break;
            }
        }
        //render screen
        SDL_SetRenderDrawColor(game.renderer,0,0,0,255);//black background
        //clear frame
        SDL_RenderClear(game.renderer);
        //draw grid and filled ones
        drawGridAndFillAliveOnes(&game);
        //present
        SDL_RenderPresent(game.renderer);
        //run 60fps
        SDL_Delay(16);
    }
    freeing(&game,EXIT_SUCCESS);
    return 0;
}

bool initializeGame(struct Game *game)
{
   //init sdl
   int init_error=  SDL_Init(SDL_INIT_EVERYTHING);
   if(init_error!=0){
      fprintf(stderr,"error on sdl_init:%s\n",SDL_GetError());
      return false;
   }

   //get available size
   SDL_DisplayMode displayMode;
   int mode_error= SDL_GetDesktopDisplayMode(0,&displayMode);
   if(mode_error!=0){
       fprintf(stderr,"error at size fetching:%s\n",SDL_GetError());
       return false;
   }
   //use height to ake it square 
   // width of my screen must be forced smaller size, assuming that most PCs have min dimension on height 
   game->width=displayMode.h;
   game->height=displayMode.h;
   //create window
   SDL_Window* window=SDL_CreateWindow("GAME OF LIFE",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,game->width,game->height,0);
   if(window==NULL){
      fprintf(stderr,"error at window_creation: %s\n",SDL_GetError());
      return false;
   }
   game->window=window;
   //create  renderer
   SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,0);
   if(renderer==NULL){
      fprintf(stderr,"error at renderer creation:%s\n",SDL_GetError());
      return false;
   }
   game->renderer=renderer;
   //recalculate size of side of the box : make it 20 boxes on each side
   game->s_side_of_box=game->width/game->how_many_boxes_in_side;
   printf("size of box: %d\n",game->s_side_of_box);
   // allocate boxes 
   game->boxes=(struct Box**)malloc(game->how_many_boxes_in_side* sizeof(struct Box*));
   for(int i=0;i<game->how_many_boxes_in_side;i++){
      game->boxes[i]=(struct Box*)malloc(game->how_many_boxes_in_side*sizeof(struct Box));
   }
   if(game->boxes==NULL){
      printf("error happened at allocating %d X %d boxes\n",game->how_many_boxes_in_side,game->how_many_boxes_in_side);
      return false;
   }
   //fill the boxes that they are not alive
   for(int row=0;row<game->how_many_boxes_in_side;row++){
      for(int col=0;col<game->how_many_boxes_in_side;col++){
         game->boxes[row][col].isAlive=false;
      }
   }
//    game->boxes[10][10].isAlive=true;//to view alive fill color
   

   return true;
}

void drawGridAndFillAliveOnes(struct Game *game)
{

    for(int row = 0; row < game->how_many_boxes_in_side; row++){
        for(int col = 0; col < game->how_many_boxes_in_side; col++){  
            SDL_Rect rect = {
                .x = col * game->s_side_of_box,
                .y = row * game->s_side_of_box,
                .w = (int)game->s_side_of_box-2*game->padding,
                .h = (int)game->s_side_of_box -2* game->padding,
            }; 
            if(game->boxes[row][col].isAlive){
                //yellow fill
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
                SDL_RenderFillRect(game->renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255); 
                SDL_RenderDrawRect(game->renderer, &rect);
            }
        }
    }
}

void freeing(struct Game *game,int exit_status)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    for(int i=0;i<game->how_many_boxes_in_side;i++){
            free(game->boxes[i]);
    }
    free(game->boxes);
    SDL_Quit();
    exit(exit_status);
}

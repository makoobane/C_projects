#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#define TITLE "GAME OF LIFE"
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
    struct Box** alt_boxes;
    int how_many_boxes_in_side;
    int width;
    int height;
    int s_side_of_box;
    int padding;//padding between boxes and their outside
    enum GameState state;
    int frame_counter;
    int frames_per_step;
};
//SDL_related functions
bool initializeGame(struct Game* game);
void drawGridAndFillAliveOnes(struct Game* game);
//game related functions
void clickBox(int x, int y,struct Game* game);
struct Box** allocateBoxes(int how_many);

int  howManyLiveNeighbors(int row,int col,int how_many_each_dim,struct Box** boxes);
void run(struct Game* game);
void freeBoxes(struct Box** boxes,int how_many_in_side);
void freeing(struct Game* game,int exit_status);
int main(){
    struct Game game={
        .frame_counter=0,
        .frames_per_step=6,
        .alt_boxes=NULL,
        .state=PUTTING,
        .padding=1,
        .how_many_boxes_in_side=30,// try 20 first
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
                         int x=event.button.x;
                         int y=event.button.y;
                         clickBox(x,y,&game);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (game.state == PUTTING && (event.motion.state & SDL_BUTTON_LMASK)) {
                    clickBox(event.motion.x, event.motion.y,&game);
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
                        if(game.state==PUTTING) {
                            game.state=RUNNING;
                        }
                        else {
                            game.state=PUTTING;
                        }
                        break;
                    default:
                        break;
                }
                    break;
                default:
                    break;
            }
        }
        run(&game);
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
   int init_error=  SDL_Init(SDL_INIT_VIDEO);
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
   SDL_Window* window=SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,game->width,game->height,0);
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
  game->boxes=allocateBoxes(game->how_many_boxes_in_side);
   if(game->boxes==NULL){
      printf("error happened at allocating %d X %d boxes\n",game->how_many_boxes_in_side,game->how_many_boxes_in_side);
      return false;
   }
   //alocate temperory boxes;
   game->alt_boxes=allocateBoxes(game->how_many_boxes_in_side);
   if(game->alt_boxes==NULL){
      printf("error happened at allocating %d X %d alt_boxes\n",game->how_many_boxes_in_side,game->how_many_boxes_in_side);
      return false;
   }
   //fill the boxes and temp(optional) that they are not alive
   for(int row=0;row<game->how_many_boxes_in_side;row++){
      for(int col=0;col<game->how_many_boxes_in_side;col++){
         game->boxes[row][col].isAlive=false;
         game->alt_boxes[row][col].isAlive=false;
      }
   }
   

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


int howManyLiveNeighbors(int row, int col, int how_many_each_dim,struct Box **boxes)
{
    int how_many_lives=0;
    //farthest column
    int starting_col=col-1;
    int ending_col=col+1;
    //farthest row
    int starting_row=row-1;
    int ending_row=row+1;
    //adjust columns if they are first or last 
    if(starting_col==-1) starting_col=col;
    if(ending_col==how_many_each_dim) ending_col=col;
    //adjust row just similar to columns
    if(starting_row==-1) starting_row=row;
    if(ending_row ==how_many_each_dim) ending_row=row;
    //count 
    for(int r=starting_row;r<=ending_row;r++){
        for(int cl=starting_col;cl<=ending_col;cl++){
           if(r==row && cl==col){
            //just skip it is the main cell
            continue;
           }else{
            //this time we are at neighbors
            if(boxes[r][cl].isAlive){
                //if neighbor is alive count as how many lives
                how_many_lives+=1;
            }
           }
        }
    }
    return how_many_lives;
}

void run(struct Game *game)
{
    if(game->state==RUNNING){
        //each frames per step it runs once others it skipps
            game->frame_counter++;
            if (game->frame_counter < game->frames_per_step) return;
            game->frame_counter = 0;
            for(int row=0;row<game->how_many_boxes_in_side;row++){
                for(int col=0;col<game->how_many_boxes_in_side;col++){
                    int aliveNeighbors = howManyLiveNeighbors(row, col,
                        game->how_many_boxes_in_side, game->boxes);  
                    if (game->boxes[row][col].isAlive) {                                 
                       game->alt_boxes[row][col].isAlive = (aliveNeighbors == 2 || aliveNeighbors == 3);
                    } else {
                       game->alt_boxes[row][col].isAlive = (aliveNeighbors == 3);
                    }
                }
            }
            //swaap boxes
            struct Box** tmp = game->boxes;
            game->boxes = game->alt_boxes;
            game->alt_boxes = tmp;
          
    }
}

void freeBoxes(struct Box **boxes, int how_many_in_box_side)
{
    if(boxes==NULL) return;
    for(int i=0;i<how_many_in_box_side;i++){
            free(boxes[i]);
    }
    free(boxes);
}

void clickBox(int x, int y,struct Game* game)
{
    int col=x/game->s_side_of_box;
    int row=y/game->s_side_of_box;
    //clamp
     if (row >= 0 && row < game->how_many_boxes_in_side &&
         col >= 0 && col < game->how_many_boxes_in_side) {
         game->boxes[row][col].isAlive = true;
     }    
}

struct Box** allocateBoxes(int how_many) {
    struct Box** boxes = malloc(how_many * sizeof(struct Box*));
    if (boxes == NULL) return NULL;
    for (int i = 0; i < how_many; i++) {
        boxes[i] = malloc(how_many * sizeof(struct Box));
        if (boxes[i] == NULL) {
            for (int j = 0; j < i; j++) free(boxes[j]);
            free(boxes);
            return NULL;
        }
    }
    return boxes;
}

void freeing(struct Game *game,int exit_status)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    freeBoxes(game->boxes,game->how_many_boxes_in_side);
    freeBoxes(game->alt_boxes,game->how_many_boxes_in_side);
    SDL_Quit();
    exit(exit_status);
}

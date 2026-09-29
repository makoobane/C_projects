#ifndef SNAKE
#define SNAKE
#include <SDL2/SDL.h>
typedef struct 
{
  SDL_Rect positionOfNode;
  SnakeNode* next;
}SnakeNode;

typedef struct{
  SnakeNode* head;//head of the snake for movements
  SnakeNode* tail;//tail for adding new element to lengthn snake
}Snake;

Snake* create3BoxesSnake(SDL_Rect* box);
void moveSnake(Snake* snake,int newX, int newY);
void eatMouse(Snake* snake);
#endif
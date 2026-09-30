#ifndef SNAKE
#define SNAKE

#include <stdint.h>

typedef struct Vector2 {
float xUnit_dir;
float yUnit_dir;
int centerX;
int centerY;
}Vector2;

typedef struct SnakeNode
{
  Vector2* vector;
  SnakeNode* before;
  SnakeNode* next;
}SnakeNode;

typedef struct Snake{
  uint8_t side_length;//of one block of snake 
  SnakeNode* head;//head of the snake for movements
  SnakeNode* tail;//tail for adding new element to lengthn snake
}Snake;

Snake* createSmallSnake(uint8_t side_length,int posx,int posy);
void changeDirection(Snake* snake,float xDir,float yDir);
void move(Snake* snake,int distance);
void addBox(Snake* snake);
void destroySnake(Snake* snake);

#endif
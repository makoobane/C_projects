#ifndef SNAKE
#define SNAKE

#include <stdint.h>

struct Vector2 {
float xUnit_dir;
float yUnit_dir;
int centerX;
int centerY;
};

struct SnakeNode
{
  struct  Vector2* vector;
  struct SnakeNode* before;
  struct  SnakeNode* next;
};

struct Snake {
  uint8_t side_length;//of one block of snake 
  struct  SnakeNode* head;//head of the snake for movements
  struct  SnakeNode* tail;//tail for adding new element to lengthn snake
};

struct Snake* createSmallSnake(uint8_t side_length,int posx,int posy);
void changeDirection(struct Snake* snake,float xDir,float yDir);
void move(struct Snake* snake,int speed);
void addBox(struct Snake* snake);
void destroySnake(struct Snake* snake);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "snake.h"

Snake* create3BoxesSnake(SDL_Rect* box){
    Snake* snake=(Snake*)malloc(sizeof(Snake));
    if(snake!=NULL){
        //allocate three boxes;
        SnakeNode* node1=(SnakeNode*)malloc(sizeof(SnakeNode));
        SnakeNode* node2=(SnakeNode*)malloc(sizeof(SnakeNode));
        SnakeNode* node3=(SnakeNode*)malloc(sizeof(SnakeNode));
        if(node1!=NULL && node2!=NULL && node3!=NULL){
           //box1
           node1->positionOfNode.x=box->x;
           node1->positionOfNode.y=box->y;
           node1->positionOfNode.w=box->w;
           node1->positionOfNode.h=box->h;
           //box2
            node2->positionOfNode.x=box->x+box->w;
            node2->positionOfNode.y=box->y+box->h;
            node2->positionOfNode.w=box->w;
            node2->positionOfNode.h=box->h;
           //box3
           node3->positionOfNode.x=box->x+(box->w)*2;
           node3->positionOfNode.y=box->y+(box->h)*2;
           node3->positionOfNode.w=box->w;
           node3->positionOfNode.h=box->h;
           //fill next
           node1->next=node2;
           node2->next=node3;
           node3->next=NULL;
           //fill snake data
           snake->head=node1;
           snake->tail=node3;
           return snake;
        }else{
            puts("one of the nodes failed to be allocated for simplicity");
            if(node1!=NULL) free(node1);
            if(node2!=NULL) free(node2);
            if(node3!=NULL) free(node3);
            free(snake);
        }
        
    }
    return NULL;
}

void moveSnake(Snake *snake, int newX, int newY)
{
    //move head to new position
    SnakeNode* head=snake->head;
    head->positionOfNode.x=newX;
    head->positionOfNode.y=newY;
    //problem: how to force other parts to follow foot stepps of the head?:
    //current IDEA: may be i need to store where head was before movement then next box after head will move to where head was and ....
    // till  the tail will go where 'before tail box' was
}

void eatMouse(Snake *snake)
{
    SnakeNode* tail=snake->tail;
    //create snakenode same size as its tail
    SnakeNode* newNode=(SnakeNode*)malloc(sizeof(SnakeNode));
    if(newNode!=NULL){
      newNode->positionOfNode.w=tail->positionOfNode.w;
      newNode->positionOfNode.h=tail->positionOfNode.h;
      //but where?
      //for now just add straight line after tail
      newNode->positionOfNode.x=tail->positionOfNode.x+tail->positionOfNode.w;
      newNode->positionOfNode.y=tail->positionOfNode.y+tail->positionOfNode.h;
      newNode->next=NULL;//it is last element in a queue
      //link it tail node 
      tail->next=newNode;
      //link it to snake as tail
      snake->tail=newNode;
    }else{
        puts("it failed to allocate new thing");
    }
}

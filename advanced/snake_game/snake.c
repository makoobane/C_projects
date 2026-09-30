#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "snake.h"



Snake *createSmallSnake(uint8_t side_length, int posx, int posy)
{
    Snake* snake=(Snake*)malloc(sizeof(Snake));
    if(snake!=NULL){
        snake->side_length=side_length;
        //create 3 snake nodes
        SnakeNode* node1=(SnakeNode*)malloc(sizeof(SnakeNode));
        SnakeNode* node2=(SnakeNode*)malloc(sizeof(SnakeNode));
        SnakeNode* node3=(SnakeNode*)malloc(sizeof(SnakeNode));
        bool isNode1Exists=node1!=NULL;
        bool isNode2Exists=node2!=NULL;
        bool isNode3Exists=node3!=NULL;
        bool Allright=isNode1Exists && isNode2Exists && isNode3Exists;
        if(Allright){
            //box1
           node1->before=NULL;
           node1->next=node2;
           //put head position to assigned position 
           node1->vector->centerX=posx;
           node1->vector->centerY=posy;
           node1->vector->xUnit_dir=1.0;
           node1->vector->yUnit_dir=0.0;
           //box2
           node2->before=node1;
           node2->next=node3;
           node2->vector->centerX=posx+ side_length;
           node2->vector->centerY=posy+ side_length;
           node2->vector->xUnit_dir=1.0;
           node2->vector->yUnit_dir=0.0;
           
           //box3
           node3->before=node2;
           node3->next=NULL;
           node3->vector->centerX=posx+ 2*side_length;
           node3->vector->centerY=posy+ 2*side_length;
           node3->vector->xUnit_dir=1.0;
           node3->vector->yUnit_dir=0.0;

           //snake
           //this will make three horizontal line snake 
           snake->head=node1;
           snake->tail=node3;


        }else{
            if(isNode1Exists){free(node1);}else{ puts("failed to allocate node1");}
            if(isNode2Exists){free(node2);}else{ puts("failed to allocate node2");}
            if(isNode3Exists){free(node3);}else{ puts("failed to allocate node3");}
            free(snake);
            return NULL;
        }

    }else{
        return NULL;
    }
}

void changeDirection(Snake *snake,float xDir, float yDir)
{
    float currentDirX=snake->head->vector->xUnit_dir;
    float currentDirY=snake->head->vector->yUnit_dir;
    float diffX=currentDirX-xDir;
    float diffY=currentDirY-yDir;
    float overflowCorrection=0.01;
    if(atan(diffY/diffX)<=(M_PI/2)+overflowCorrection){
      snake->head->vector->xUnit_dir=xDir;
      snake->head->vector->yUnit_dir=yDir;
    }
}

void move(Snake* snake,int distance)
{
      SnakeNode* head=snake->head;
      SnakeNode* part=head->next;
      while (part!=NULL)
      {
        SnakeNode* before=part->before;
        int xApart=part->vector->centerX - before->vector->centerX;
        int yApart=part->vector->centerY -before->vector->centerY;
        float length=(float)sqrt(xApart*xApart + yApart*yApart);
        //find unit vector
        float xUnit=xApart / length;
        float yUnit=yApart/length;
        part->vector->xUnit_dir=xUnit;
        part->vector->yUnit_dir=yUnit;
        
        //move its center towards that new position
        int dispX=distance* (xUnit*xUnit);
        int dispY=distance*(yUnit*yUnit);
        part->vector->centerX+=dispX;
        part->vector->centerY+=dispY;
        part=part->next;
      }
               
            
}

void addBox(Snake *snake)
{
    SnakeNode* node=(SnakeNode*)malloc(sizeof(SnakeNode));
    if(node!=NULL){

    }else{
        puts("it failed to make new node");
    }
}

void destroySnake(Snake *snake)
{
    SnakeNode* part=snake->head;
    while(part!=NULL){
     SnakeNode* next=part->next;
     free(part);
     part=next;
    }
    free(snake);
}

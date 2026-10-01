#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "snake.h"



struct Snake *createSmallSnake(uint8_t side_length, int posx, int posy)
{
    struct  Snake* snake=(struct Snake*)malloc(sizeof(struct Snake));
    if(snake!=NULL){
        snake->side_length=side_length;
        //create 3 snake nodes
       struct SnakeNode* node1=(struct SnakeNode*)malloc(sizeof(struct SnakeNode));
        struct SnakeNode* node2=(struct SnakeNode*)malloc(sizeof(struct SnakeNode));
        struct SnakeNode* node3=(struct SnakeNode*)malloc(sizeof(struct SnakeNode));
        bool isNode1Exists=node1!=NULL;
        bool isNode2Exists=node2!=NULL;
        bool isNode3Exists=node3!=NULL;
        bool Allright=isNode1Exists && isNode2Exists && isNode3Exists;
        if(Allright){
            //box1
           node1->before=NULL;
           node1->next=node2;
           //put head position to assigned position 
           struct Vector2* node1Vector=(struct Vector2*)malloc(sizeof(struct Vector2));
           if(node1Vector==NULL){
            puts("it failed to allocate node1 vector");
            free(node1);
            free(node2);
            free(node3);
            free(snake);
            return NULL;
           }
           node1->vector=node1Vector;
           node1->vector->centerX=posx;
           node1->vector->centerY=posy;
           node1->vector->xUnit_dir=-1.0;
           node1->vector->yUnit_dir=0.0;
           //box2
           node2->before=node1;
           node2->next=node3;
           struct Vector2* node2Vector=(struct Vector2*)malloc(sizeof(struct Vector2));
           if(node2Vector==NULL){
            puts("it failed to allocate node2 vector");
            free(node1Vector);
            free(node1);
            free(node2);
            free(node3);
            free(snake);
            return NULL;
           }
           node2->vector=node2Vector;
           node2->vector->xUnit_dir=-1.0;
           node2->vector->yUnit_dir=0.0;
           node2->vector->centerX=posx+side_length;
           node2->vector->centerY=posy;
           
           //box3
           node3->before=node2;
           node3->next=NULL;
           struct Vector2* node3Vector=(struct Vector2*)malloc(sizeof(struct Vector2));
           if(node3Vector==NULL){
            puts("it failed to allocate node3 vector");
            free(node1Vector);
            free(node2Vector);
            free(node1);
            free(node2);
            free(node3);
            free(snake);
            return NULL;
           }       
           node3->vector=node3Vector;    
           node3->vector->xUnit_dir=-1.0;
           node3->vector->yUnit_dir=0.0;
           node3->vector->centerX=posx+ 2*side_length;
           node3->vector->centerY=posy;

           //snake
           //this will make three horizontal line snake 
           snake->head=node1;
           snake->tail=node3;
          return snake;

        }else{
            if(isNode1Exists){free(node1);}else{ puts("failed to allocate node1");}
            if(isNode2Exists){free(node2);}else{ puts("failed to allocate node2");}
            if(isNode3Exists){free(node3);}else{ puts("failed to allocate node3");}
            free(snake);
            return NULL;
        }
    }

        return NULL;

    
}

void changeDirection(struct Snake *snake,float xDir, float yDir)
{
    float currentDirX=snake->head->vector->xUnit_dir;
    float currentDirY=snake->head->vector->yUnit_dir;
    float diffX=currentDirX-xDir;
    float diffY=currentDirY-yDir;
    float angleRad=atan((diffY/diffX));
    int angleDeg=(int)((180/M_PI) * angleRad);
    printf("angle:%d\n",angleDeg);
    if(angleDeg<91){
        //you dont need to flip the neck of the snake so just do it when it is less then 90 degree or same as 90
        snake->head->vector->xUnit_dir=xDir;
        snake->head->vector->yUnit_dir=yDir;
    }

}

void move(struct Snake* snake,int speed)
{
     struct  SnakeNode* head=snake->head;
     //move head
      int headXmag=(speed)*(head->vector->xUnit_dir*head->vector->xUnit_dir);
      if(head->vector->xUnit_dir<=0){
         head->vector->centerX+=(headXmag)*(-1);
      }else{
         head->vector->centerX+=headXmag;
      }

      int headYmag=(speed)*(head->vector->yUnit_dir*head->vector->yUnit_dir);
      if(head->vector->yUnit_dir<=0){
         head->vector->centerY+=(headYmag)*(-1);
      }else{
          head->vector->centerY+=headYmag;
      }
      //NOTE: keep heads direction
     struct  SnakeNode* part=head->next;
      while (part!=NULL)
      {
       struct  SnakeNode* before=part->before;
       //calculate vector between them(each box and  the one  before)
        int xApart=before->vector->centerX -  part->vector->centerX;
        int yApart=before->vector->centerY - part->vector->centerY;
        float length=(float)sqrt(xApart*xApart + yApart*yApart);
        //find unit vector
        float xUnit=xApart / length;
        float yUnit=yApart/length;
        //parse unit vector
        part->vector->xUnit_dir=xUnit;
        part->vector->yUnit_dir=yUnit;
        //calculate what magntude it will move each direction
        int dispXM=speed* (xUnit*xUnit);
        int dispYM=speed*(yUnit*yUnit);
     
        if(part->vector->xUnit_dir>=0){

            part->vector->centerX+=dispXM;
        }else{
            part->vector->centerX+=(dispXM)*(-1);
        }
        if(part->vector->yUnit_dir>=0){

            part->vector->centerY+=dispYM;
        }else{
            part->vector->centerY+=(dispYM)*(-1);
        }
        part=part->next;
      }
               
            
}

void addBox(struct Snake *snake)
{
   struct  SnakeNode* node=(struct SnakeNode*)malloc(sizeof(struct SnakeNode));

    if(node!=NULL){
        struct Vector2* vector=malloc(sizeof(struct Vector2));
        if(vector==NULL){
            puts("it failed to allocate vector for new node to add");
            free(node);
        }else{
            //i will do this later after snake view, move, change direction


        }

        

    }else{
        puts("it failed to make new node");
    }
}

void destroySnake(struct Snake *snake)
{
    if(snake==NULL) return;
    struct SnakeNode* part=snake->head;
    while(part!=NULL){
     struct SnakeNode* next=part->next;
     free(part->vector);
     free(part);
     part=next;
    }
    free(snake);
}

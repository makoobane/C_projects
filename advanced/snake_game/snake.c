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
    //only if the current direction and new direction are 90 to each other then we change head dir
    int8_t dotproduct=(int8_t) (xDir*snake->head->vector->xUnit_dir + yDir* snake->head->vector->yUnit_dir);
    if(dotproduct==0){

        snake->head->vector->xUnit_dir=xDir;
        snake->head->vector->yUnit_dir=yDir;
    }
 

}
void move(struct Snake* snake, float speed)
{
    // 1. head moves along its direction
    struct Vector2* h = snake->head->vector;
    h->centerX += h->xUnit_dir * speed;
    h->centerY += h->yUnit_dir * speed;

    // 2. each follower aims at the block before it (already updated)
    float gap = snake->side_length;
    for (struct SnakeNode* n = snake->head->next; n != NULL; n = n->next) {
        struct Vector2* me  = n->vector;
        struct Vector2* pre = n->before->vector;

        float dx  = pre->centerX - me->centerX;
        float dy  = pre->centerY - me->centerY;
        float len = sqrtf(dx * dx + dy * dy);
        if (len < 0.0001f) continue;          // avoid division by zero

        me->xUnit_dir = dx / len;             // unit vector toward the block before
        me->yUnit_dir = dy / len;

        float step = len - gap;               // how far to close to restore spacing
        if (step > 0.0f) {
            me->centerX += me->xUnit_dir * step;
            me->centerY += me->yUnit_dir * step;
        }
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
            struct SnakeNode* tail=snake->tail;
            //copy data from tail to vector
            vector->centerX=tail->vector->centerX;
            vector->centerY=tail->vector->centerY;
            vector->xUnit_dir=tail->vector->xUnit_dir;
            vector->yUnit_dir=tail->vector->yUnit_dir;
            //point nodes vector to that vector
            node->vector=vector;
            //make tail its next
            tail->next=node;
            //make node before as current tail
            node->before=tail;
            node->next=NULL;
            //assign tail
            snake->tail=node;
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

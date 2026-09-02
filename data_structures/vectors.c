#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

typedef struct Point
{
  int x;
  int y;
}Point;

struct Vector2{
float val1;
float val2;
Point* origin;
};
struct Vector2* vector_2(float u,float v,Point* origin);
float length(struct Vector2* vector);
struct Vector2* add(struct Vector2* v1,struct Vector2* v2);
void de_allocate(struct Vector2* v);
int main(){
    struct Vector2* v1=vector_2(4.0,4.0,NULL);
    struct Vector2* v2=vector_2(4.0,3.0,NULL);
    if(v1!=NULL&& v2!=NULL){
        float l1=length(v1);

        printf("length of vecctor1=%.3f\n",l1);
        printf("length of vecctor2=%.3f\n",length(v2));
        struct Vector2* sumv=add(v1,v2);
        printf("sum vector length is %.3f\n",length(sumv));
        de_allocate(sumv);
    }else{
        printf("failed for vector2 creation");
    }
    de_allocate(v1);
    de_allocate(v2);
    return 0;
}

struct Vector2 *vector_2(float u, float v,Point *origin)
{
    struct Vector2* vector=malloc(sizeof(struct Vector2));
    if(vector!=NULL){
        vector->val1=u;
        vector->val2=v;
        if(origin!=NULL){
            vector->origin=origin;
        }else{
            Point* zero=malloc(sizeof(Point));
            if(zero!=NULL){
                zero->x=0;
                zero->y=0;
                vector->origin=zero;
            }else{
                printf("error code %d,\n",errno);
                printf("message is: %s\n",strerror(errno));
            }
        }
        return vector;
    }
    return NULL;
    
}

float length(struct Vector2 *vector)
{
    float udir=vector->val1- vector->origin->x;
    float vdir=vector->val2- vector->origin->y;
    float sumsqred= pow(udir,2)+pow(vdir,2);
    return sqrt(sumsqred);
    
}
struct Vector2* add(struct Vector2* v1,struct Vector2* v2){
    float sumu=v1->val1+v2->val1;
    float sumv=v1->val2+v2->val2;
    Point* origin1=malloc(sizeof(Point));// to make de alocating every vector easy... may be this will be better for keeping result origin 
    //bcs head to tail . head of second vector and tail of first vector is our sum vector
    origin1->x=v1->origin->x;
    origin1->y=v1->origin->y;
    struct Vector2* sumVector=malloc(sizeof(struct Vector2));
    if(sumVector!=NULL){
        sumVector->origin=origin1;
        sumVector->val1=sumu;
        sumVector->val2=sumv;
        return sumVector;
    }else{
        return NULL;
    }
}

void de_allocate(struct Vector2 *v)
{
    free(v->origin);
    free(v);
}

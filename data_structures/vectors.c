#include <stdio.h>
#include <stdlib.h>
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
typedef struct Vector3{
float val1;
float val2;
float val3;
Point* origin;
}Vector3;
struct Vector2* vector_2(float u,float v,Point* origin);
float length(struct Vector2* vector);
struct Vector2* add(struct Vector2* v1,struct Vector2* v2);
struct Vector2* subtract(struct Vector2* v1,struct Vector2* v2);
void scale(struct Vector2* vector,float scale);
float dot(struct Vector2* v1,struct Vector2* v2);
Vector3* cross(struct Vector2* v1,struct Vector2* v2);
void makeThemOrthognal(struct Vector2* v1,struct Vector2* v2);
void de_allocate(struct Vector2* v);
int main(){
    struct Vector2* v1=vector_2(4.0,4.0,NULL);
    struct Vector2* v2=vector_2(4.0,3.0,NULL);
    if(v1!=NULL&& v2!=NULL){
        printf("length of vecctor1=%.3f\n",length(v1));
        printf("length of vecctor2=%.3f\n",length(v2));
        struct Vector2* sumv=add(v1,v2);
        struct Vector2* diffv=subtract(v1,v2);
        printf("sum vector length is %.3f\n",length(sumv));
        printf("diff vector length is %.3f\n",length(diffv));
        scale(v1,10);
        printf("vector 1 length after scaled 10 times %.3f\n",length(v1));
        float dot_v1_v2=dot(v1,v2);
        printf("dot product of 10v1 and v2 is equal:%.4f\n",dot_v1_v2);
        float extendedDot=length(v1)*length(v2)*cos(atan(v1->val2/v1->val1)-atan(v2->val2/v2->val1));//just assume they had same origin...
        printf("10v1.v2=%f and |10v1||v2|cos(M)=%f\n",dot_v1_v2,extendedDot);//try to assert it
        Vector3* crossv=cross(v1,v2);
        printf("length of cross of 10V1XV2=%f\n",fabs(crossv->val3));
        printf("|10v1|v2||sin(m)=%f\n",length(v1)*length(v2)*sin(atan(v1->val2/v1->val1)-atan(v2->val2/v2->val1)));
        makeThemOrthognal(v1,v2);
        printf("to make sure that they are orthognal, dot product must be zero : %f\n",dot(v1,v2));
        de_allocate(diffv);
        de_allocate(sumv);
        free(crossv->origin);
        free(crossv);
    }else{
        printf("failed for vector2 creation");
    }
    de_allocate(v1);
    de_allocate(v2);
    return 0;
}

struct Vector2 *vector_2(float u, float v,Point *origin)
{
    struct Vector2* vector = malloc(sizeof(struct Vector2));
    if (vector == NULL) return NULL;

    Point* copy = malloc(sizeof(Point));
    if (copy == NULL) {
        perror("it failed to allocate origin for this vector's creation");
        free(vector);
        exit(1);
    }
    copy->x = origin ? origin->x : 0;
    copy->y = origin ? origin->y : 0;
    vector->origin = copy;
    vector->val1 = u - copy->x;
    vector->val2 = v - copy->y;
    return vector;
    
}

float length(struct Vector2 *vector)
{
    return sqrt(pow(vector->val1,2)+pow(vector->val2,2));
    
}
struct Vector2* add(struct Vector2* v1, struct Vector2* v2){
    return vector_2(v1->val1 + v2->val1 + v1->origin->x,
                     v1->val2 + v2->val2 + v1->origin->y,
                     v1->origin);
}
struct Vector2* subtract(struct Vector2* v1, struct Vector2* v2){
    return vector_2(v1->val1 - v2->val1 + v1->origin->x,
                     v1->val2 - v2->val2 + v1->origin->y,
                     v1->origin);
}

void  scale(struct Vector2 *vector,float scale)
{
      vector->val1=(vector->val1)*scale;
      vector->val2=(vector->val2)*scale;
}
float dot(struct Vector2* v1,struct Vector2* v2){
    return (v1->val1 * v2->val1)+(v1->val2 * v2->val2);
}

Vector3 *cross(struct Vector2 *v1,struct  Vector2 *v2)
{
    if(v1->origin->x==v2->origin->x&&v1->origin->y==v2->origin->y){
        Vector3* cross=malloc(sizeof(Vector3));
         //two D arrays cross only makes third direction and it has this formula:
        //v2->val2*v1->val1(primary diagnal) - secondary diagnal;
        if(cross!=NULL){
           Point* origin3D=malloc(sizeof(Point));
           origin3D->x=v1->origin->x;
           origin3D->y=v1->origin->y;
           cross->origin=origin3D;
           cross->val1=0;
           cross->val2=0;
           cross->val3=v2->val2 * v1->val1 -v1->val2 * v2->val1;
           return cross;
        }
        perror("it failed to make 3D vector in cross product");
        exit(1);
    }
    return NULL;
}

void makeThemOrthognal(struct Vector2 *v1,struct  Vector2 *v2)
{
    if(v1->origin->x==v2->origin->x && v1->origin->y==v2->origin->y){
       //keep v1 as it is
       //v2=v2-unitprojection on v1
       scale(v1,0.1);
       float unitprojection=dot(v1,v2)/dot(v1,v1);
       float x=unitprojection*v1->val1;
       float y=unitprojection*v1->val2;
       struct Vector2* projectionOnV1=vector_2(x,y,v1->origin);
       struct Vector2* v2orthognized=subtract(v2,projectionOnV1);
       v2->val1=v2orthognized->val1;
       v2->val2=v2orthognized->val2;
       de_allocate(projectionOnV1);
       de_allocate(v2orthognized);
       
    }else{
        puts("those two vectors dont have same origin");
        exit(1);
    }
}

void de_allocate(struct Vector2 *v)
{
    free(v->origin);
    free(v);
}

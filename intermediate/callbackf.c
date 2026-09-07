#include <stdlib.h>
#include <stdio.h>

struct Array
{
  int* arr;
  int length;
};

void forEach(void (*func)(const int,const int ,const  int ),struct Array* array);

void print(int element, int index,int length);

int main(){
    struct Array* array=malloc(sizeof(struct Array));
    
    if(array!=NULL){
        array->length=4;
        array->arr=calloc(array->length,sizeof(int));
        array->arr[0]=13;
        array->arr[1]=10;
        array->arr[2]=-3;
        array->arr[3]=23;
        if(array->arr!=NULL){
        forEach(print,array);
        free(array->arr);
        array->arr=NULL;
        }
        free(array);
    }
    array=NULL;
    return 0;
}

void print(int element,int index, int length)
{
    if(length>1){
        if(index==0){
          printf("[%d,",element);
       }else if(index<length-1){
          printf("%d,",element);
       }else{
         printf("%d]\n",element);
       }
    }else{
         printf("[%d]\n",element);
    }
}
void forEach(void (*func)(int,int ,int), struct Array* array)
{
    for(int i=0;i<array->length;i++){
        func(array->arr[i],i,array->length);
    }
}


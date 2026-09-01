#include <stdio.h>
#include <stdlib.h>
//making stack with array
typedef struct 
{
 int* elements;
 int length;
}Stack;
Stack* createStack();
void push(Stack* stack,int data);
void undo(Stack* stack);
void print(Stack* stack);
void peek(Stack* stack);
void clear(Stack* stack);
int main(){
    Stack* stack=createStack();
    if(stack!=NULL){
        stack->elements=malloc(sizeof(int));
        if(stack->elements!=NULL){
            push(stack,10);
            push(stack,3);
            push(stack,12);
            print(stack);
            undo(stack);
            // undo(stack);
            // undo(stack);
            // undo(stack);
            // peek(stack);
            clear(stack);
            print(stack);
            free(stack->elements);
        }
        stack->elements=NULL;
        free(stack);
    }else{
       perror("it can't allocate the stack pointer");
       exit(1);
    }
    stack=NULL;
    return 0;
}

Stack *createStack()
{   int initLength=0;
    Stack* stack=malloc(sizeof(Stack));
    if(stack!=NULL){
          stack->length=initLength;
          return stack;
    }
    return NULL;
}

void push(Stack *stack, int data)
{  
    int* elements=realloc(stack->elements,(stack->length+1)*sizeof(int));
    if(elements!=NULL){
         stack->elements=elements;
         stack->elements[stack->length]=data;
         stack->length+=1;      
    }else{
        perror("it failed to increase size of elements in \"push\" fuunction");
        exit(1);
    }
   
}

void undo(Stack *stack)
{ if(stack->length>1){
    stack->elements=realloc(stack->elements,(stack->length-1)*sizeof(int));
    stack->length-=1;
 }else if(stack->length==1){
    stack->length--;
 }else{
    perror("NOthing to remove is in here");
    exit(1);
 }
}

void print(Stack *stack)
{  
    puts("stack data:");
    for(int i=0;i<stack->length;i++){
        printf("elemens[%d]=%d\n",i,stack->elements[i]);
    }
    if(stack->length==0)puts("isEmpty");
}

void peek(Stack *stack)
{
  if(stack->length>0){
    printf("top element in this stack is %d:\n",stack->elements[stack->length-1]);
  }else{
    printf("no element is available in here");
    EXIT_SUCCESS;
  }
}

void clear(Stack *stack)
{
    stack->elements=realloc(stack->elements,1);// it is ok to reduce it size of char
    stack->length=0;
}

#include <stdio.h>
#include <stdlib.h>
struct StackNode{
int data;
struct StackNode* next;
};
typedef struct{
struct StackNode* top;
}Stack;

Stack* createStack(int data);
struct StackNode* push(Stack* stack,int data);
int main(){
    Stack* stack=createStack(14);
    if(stack!=NULL){
        if(stack->top!=NULL){
            struct StackNode* previusTop1= push(stack,100);
            free(previusTop1);
            free(stack->top);
        }
        stack->top=NULL;
        free(stack);
    }else{
        perror("it failed to construct Stack");
        exit(1);
    }
    stack=NULL;
    return 0;
}
Stack* createStack(int data){
Stack* stack=malloc(sizeof(Stack));
if(stack!=NULL){
    stack->top=malloc(sizeof(struct StackNode));
    if(stack->top!=NULL){
          stack->top->data=data;
          stack->top->next=NULL;
    }else{
        perror("it failed to allocate first node in stack");
        exit(1);
    }  
    return stack;
}
return NULL;
}

struct StackNode* push(Stack* stack, int data){
    //create new node
    struct StackNode* newNode=malloc(sizeof(struct StackNode));
    newNode->data=data;
    newNode->next=NULL;
    //keeps one before or previus top
    struct StackNode* belowTop=stack->top;
    belowTop->next=newNode;
    //make new node top
    stack->top=newNode;
    //return previus one for freeing
    return belowTop;
}

void *pop(Stack *stack)
{
    
}

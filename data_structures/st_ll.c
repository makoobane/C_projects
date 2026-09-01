#include <stdio.h>
#include <stdlib.h>
struct StackNode{
int data;
struct StackNode* below;
};
struct Stack{
struct StackNode* top;
};
//this is like assuming that stack is box which contains items ... the top item points the one below and that below
//points the one below it till they reach the bottom of the container box;
struct Stack* createStack(int topItem);
struct StackNode* put(struct Stack* stack,int data);// like we put item X on top of others in container box
int grab(struct Stack* stack);//remove the top element in container
void visualize(struct Stack* stack);
void destroy(struct Stack* stack);
int main(){
    struct  Stack* stack=createStack(9);
    if(stack!=NULL){
        if(stack->top!=NULL){
            struct  StackNode* nn1=put(stack,17);
            struct  StackNode* nn2=put(stack,15);
            visualize(stack);
            int whatleft=grab(stack);
            printf("%d is deleted\n",whatleft);
            visualize(stack);
            destroy(stack);
        }
      
    }else{
        perror("it failed to create stack");
        exit(1);
    }
    stack=NULL;
    return 0;
}

struct Stack *createStack(int topItem)
{
    struct Stack* stack= malloc(sizeof(struct Stack));
    if(stack!=NULL){
        stack->top=malloc(sizeof(struct StackNode));
        if(stack->top!=NULL){
            stack->top->data=topItem;
            stack->top->below=NULL;// it is first so no body is below it just container bottom(hhh...)
            return stack;
        }else{
            perror("it failed to allocate stack's top Stacknode");
            exit(1);
        }
    }
    return NULL;
}

struct StackNode* put(struct Stack* stack,int data)
{   //create newnode/allocate
    struct StackNode* newNode=malloc(sizeof(struct StackNode));
    if(newNode!=NULL){
        newNode->data=data;
        //it is on top of our previus top
        newNode->below=stack->top;
        //our new top is itself
        stack->top=newNode;
        return newNode;
    }else{
        perror("it failed to alloccate in put");
        return NULL;
    }
}

int grab(struct Stack *stack)
{   struct StackNode* oldtop=stack->top;
    int data=oldtop->data;
    //make the top the one below 
    stack->top=oldtop->below;
    free(oldtop);
    return data;
}

void visualize(struct Stack* stack)
{
    struct StackNode* top=stack->top;
    while(top){
        if(top->below==NULL){
           printf("%d||bottom of the box\n",top->data);
        }else{
           printf("%d=>",top->data);
        }
        top=top->below;
    }
}
void destroy(struct Stack* stack)
{
    struct StackNode* current = stack->top;
    while (current != NULL) {
        struct StackNode* next = current->below;
        free(current);
        current = next;
    }
    free(stack);
}
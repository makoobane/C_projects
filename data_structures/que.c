#include <stdlib.h>
#include <stdio.h>
struct QueNode {
int data;
struct QueNode* next;
};
struct Queue{
   struct  QueNode* first;
   struct  QueNode* last;
};
struct Queue* createQue();
void push(struct Queue* que,int data);
int pop(struct Queue* que);
void view(struct Queue* que);
void freeing(struct Queue* que);

int main(){
    struct  Queue* que=createQue();
    if(que!=NULL){
       push(que,30);
       push(que,77);
       push(que,23);
       view(que);
       int data=pop(que);
       printf("%d was deleted\n",data);
       view(que);
       //freeing
       freeing(que);
       que=NULL;
    }else{
        perror("it failed to create");
        exit(1);
    }

    return 0;
}
struct Queue* createQue(){
    struct  Queue* que=malloc(sizeof( struct Queue));
    if(que!=NULL){
        que->first=NULL;
        que->last=NULL;
        return que;
    }
    return NULL;
}

void push(struct Queue *que, int data)
{
    struct QueNode* queNode=malloc(sizeof(struct QueNode));
    queNode->data=data;
    queNode->next=NULL;

    if(que->first==NULL){
        //this is first item in que
        que->first=queNode;
    }else{
        if(que->first->next==NULL){
            // it had one element so add next
            que->first->next=queNode;
        }else{
            // it had more than one element
            que->last->next=queNode;
        }
        que->last=queNode;
    }
}

int pop(struct Queue *que)
{
    struct QueNode* firstNode=que->first;
    int firstData=firstNode->data;
    que->first=firstNode->next;
    free(firstNode);
    return firstData;
}

void view(struct Queue *que)
{
    struct QueNode* thisNode=que->first;
    while(thisNode!=NULL){
        struct QueNode* next=thisNode->next;
        if(next!=NULL){
            printf("%d->",thisNode->data);
        }else{
            printf("%d||\n",thisNode->data);
        }
        thisNode=next;
    }

}

void freeing(struct Queue *que)
{
     struct QueNode* this=que->first;
     while(this!=NULL){
       struct QueNode* next=this->next;
       free(this);
       this=next;
     }
     free(que);
}

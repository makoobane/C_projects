#include <stdio.h>
#include <stdlib.h>

struct LinkedListNode
{
    struct LinkedListNode* before;
    struct LinkedListNode* after;
    int data;
};
struct LinkedListNode* createLinkedList(int data);
struct LinkedListNode* append(struct LinkedListNode* head,int data);
void printLinkedList(struct LinkedListNode* head);
void removeOtherThanHead(struct LinkedListNode*head,struct LinkedListNode* leavingOne);
struct LinkedListNode* removehead(struct LinkedListNode* head);
//to insert linkedlist what should we use for link ... index?? if we had index and knew list length why we need linkedlist?
// pointer?? maybe
struct LinkedListNode* insert(struct LinkedListNode* head,struct LinkedListNode* reference,int data);
int main(){
    struct LinkedListNode* head=createLinkedList(10);
    if(head!=NULL){
    struct LinkedListNode* nn1=append(head,13);
    struct LinkedListNode* nn2=append(head,16);
    struct LinkedListNode* nn3=append(head,32);
    struct LinkedListNode* nn4=append(head,41);

    printLinkedList(head);
    // removeOtherThanHead(head,nn4);
    // removeOtherThanHead(head,nn3);
    // struct LinkedListNode* newhead= removehead(head);//new head is just one of our previus nodes
    // printLinkedList(newhead);
    struct LinkedListNode* nn5=insert(head,nn3,100);
    printf("%d,before:%p,after:%p\n",nn5->data,nn5->before,nn5->after);
    printLinkedList(head);// it did not added nn5 in the list why?
    if(head!=NULL)free(head);
    if(nn1!=NULL)free(nn1);
    if(nn2!=NULL)free(nn2);
    if(nn3!=NULL)free(nn3);
    if(nn4!=NULL)free(nn4);
    if(nn5!=NULL)free(nn5);
    nn1=NULL;
    nn2=NULL;
    nn3=NULL;
    nn4=NULL;
    nn5=NULL;
    }else{
        perror("it failed to allocated the head");
    }
    head=NULL;
    return 0;
}

struct LinkedListNode* createLinkedList(int data)
{
 struct LinkedListNode* ll=malloc(sizeof(struct LinkedListNode));
 ll->before=NULL;
 ll->after=NULL;
 ll->data=data;
 return ll;
}

struct LinkedListNode* append(struct LinkedListNode* head, int data){
    struct LinkedListNode* newNode=malloc(sizeof(struct LinkedListNode));
    if(newNode!=NULL){
    newNode->data=data;
    newNode->after=NULL;
    struct LinkedListNode* current=head;
    int islast=0;
    while(islast!=1){
        if(current->after!=NULL){
            current=current->after;
        }else{
            islast=1;
        }
    }
    //we are at last
    current->after=newNode;
    newNode->before=current;
    return newNode;
    }else{
        perror("it failed to allocate new linkedlist Node");
        return NULL;
    }
   
}

void printLinkedList(struct LinkedListNode *head)
{
    struct LinkedListNode* current=head;
    int isLast=0;
    while(isLast!=1){
        if(current->after!=NULL&&current->before!=NULL){
            //middle one
            printf("<-%d->",current->data);
            current=current->after;
        }else if(current->after!=NULL && current->before==NULL){
            //it is first
            printf("(%d->",current->data);
            current=current->after;
        }
        else if(current->after==NULL){
            //it is last
            printf("%d)\n",current->data);
            isLast=1;
        }
        
    }
}

void removeOtherThanHead(struct LinkedListNode *head,struct  LinkedListNode *leavingOne)
{
    if(leavingOne==NULL){
        perror("you want to remove non existant element");
        exit(1);
    }
    if(head==leavingOne){
        perror("you cant remove head with this function use \"removehead(head)\" instead ");
        exit(1);
    }
    struct LinkedListNode* current=head;
    while(current!=NULL){
        if(current->after!=leavingOne){
           current=current->after;
        }else{
            //validation
            if(leavingOne->before!=current){
                perror("this node you want to remove is not really linked to this linkedlist ... it dont have before(previus) pointer");
                exit(1);
            }
            //point behind
            struct LinkedListNode* afterLeavingOne=leavingOne->after;
            if(afterLeavingOne!=NULL){
               current->after=afterLeavingOne;
               afterLeavingOne->before=current;//so it will cut ties to leaving one
            }else{
                current->after=NULL;
            }
            break;
        }
       }
    
}

struct LinkedListNode *removehead(struct LinkedListNode* head)
{
  if(head->after==NULL){
    puts("this linkedlist had single element and you distroyed");
    return NULL;
  }else{
    struct LinkedListNode* next=head->after;
    next->before=NULL;
    return next;
  }
}

struct LinkedListNode* insert(struct LinkedListNode *head,struct  LinkedListNode *reference, int data)
{
    struct LinkedListNode* current=head;
    while(current!=NULL){
        if(current==reference){
           struct LinkedListNode* n=malloc(sizeof(struct LinkedListNode));
           if(n!=NULL){
            n->data=data;
            n->before=current->before;
            struct LinkedListNode* nb= current->before;
            if(nb!=NULL){
                nb->after=n;
                n->after=reference;
                return n;
            }
            return NULL;
           }else{
            return NULL;
           }
        }
        current=current->after;
    }
    return NULL;
    
}

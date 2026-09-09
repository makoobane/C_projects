#include <stdio.h>
#include <stdlib.h>
typedef struct Node{
    int data;
    struct Node* left;
    struct Node* right;
}Node;
typedef struct BST
{
    int count;
    struct  Node* parent;
}BST;
BST* createBSTWithParentDataOf(int data);
void append(BST* tree,int data);
void deallocate(BST* tree);
void freeing(Node* root);
int main(){
    BST* tree=createBSTWithParentDataOf(10);
    if(tree!=NULL){
      append(tree,12);
      deallocate(tree);
    }

    return 0;
}

BST *createBSTWithParentDataOf(int data)
{
    BST* tree=(BST*) malloc(sizeof(BST));
    if(tree!=NULL){
    Node* parent=(Node*) malloc(sizeof(Node));
    if(parent!=NULL){
       parent->data=data;
       parent->left=NULL;
       parent->right=NULL;
       tree->parent=parent;
       tree->count=1;
       return tree; 
       }else{
        printf("it failed to allocate parent");
        free(tree);
        return NULL;
       }
    }
    return NULL;
    
}

void append(BST *tree, int data)
{
    Node* new_Node=(Node*)malloc(sizeof(Node));
    if(new_Node!=NULL){
        new_Node->data=data;
        new_Node->left=NULL;
        new_Node->right=NULL;
        Node* current=tree->parent;
        while (current!=NULL)
           {
               //if it is greater than node data
            if(data>= current->data){
                 //go right
                //if it is last append
            if(current->right==NULL){
                current->right=new_Node;

                break;
            }else{
                 //if it is not go right down
                current=current->right;
            }

         }else{
          //go left
          //if it is last append
          if(current->left==NULL){
            current->left=new_Node;
            break;
        }else{
            //if it is not last go down to reach last
            current=current->left;
        }
    }
    
}
tree->count+=1;
puts("it appended successfully");   
}else{
    perror("it failed to allocate that inserted node");
    exit(1);
}
}

void freeing(Node* root)
{
    int capacity=16;
    int top=0;
    Node** stack=(Node**)calloc(capacity,sizeof(Node*));
    if(stack==NULL){
        perror("it failed freeing bcs it is not allocating stack for freeing");
        exit(1);

    }else{
        stack[top]=root;
        top+=1;
        while(top>0){
            top-=1;
            Node* node=stack[top];
            if(node->left!=NULL){
                if(top==capacity){
                   capacity*=2;
                   Node** resizedStack=realloc(stack,capacity*sizeof(Node*));
                   if(resizedStack==NULL){
                       perror("failed to reallocate freeing stack");
                       free(stack);
                       return;
                   }
                   stack=resizedStack;
                 }
             stack[top]=node->left;
             top+=1;
            }
            if(node->right!=NULL){
               if(top==capacity){
                   capacity*=2;
                   Node** resizedStack=realloc(stack,capacity*sizeof(Node*));
                   if(resizedStack==NULL){
                       perror("failed to reallocate freeing stack");
                       free(stack);
                       return;
                   }
                   stack=resizedStack;
                 }
             stack[top]=node->right;
             top+=1;
            }
            free(node);
        }
        free(stack);
    }
}

void deallocate(BST* tree){
 freeing(tree->parent);
 free(tree);
}

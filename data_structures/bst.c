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
    struct Node** helper;
}BST;
BST* createBSTWithParentDataOf(int data);
void append(BST* tree,int data);
void freeing(BST* tree);
int main(){
    BST* tree=createBSTWithParentDataOf(10);
    if(tree!=NULL){
      append(tree,12);
      freeing(tree);
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
       tree->helper=(Node**)calloc(1,sizeof(Node*));
       if(tree->helper!=NULL){
           tree->helper[0]=parent;
           tree->count=1;
           tree->parent=parent;
           return tree;
       }else{
        //eventhough it may work but freeing will be difficult so
        perror("it failed to allocate garbage collector like");
        free(parent);
        free(tree);
        return NULL;
       }
    }else{
        perror("it failed to allocate the parent");
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
Node** resizedHelper=(Node**) realloc(tree->helper,(tree->count+1 )*sizeof(Node*));
if(resizedHelper!=NULL){
    resizedHelper[tree->count]=new_Node;
    tree->count+=1;
    tree->helper=resizedHelper;
    puts("successfully added");
   }else{
      printf("it failed to reallocatte freeer at append of %d\n",data);
      if(current->left==new_Node) current->left=NULL;
      if(current->right==new_Node) current->right=NULL;
      free(new_Node);
   }
   
}else{
    perror("it failed to allocate that inserted node");
    exit(1);
}
}

void freeing(BST *tree)
{
    for(int i=0;i<tree->count;i++){
        Node* node=tree->helper[i];
        if(node!=NULL){
            free(node);
        }else{
            puts("wo this was not supposed ");
        }
    }
    free(tree->helper);
    free(tree);
}

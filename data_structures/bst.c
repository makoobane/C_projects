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
int contains(BST* tree, int data);
Node* find(BST* tree, int data);
Node* findParentOf(BST* tree, Node* node);
Node* findMax(BST* tree);
Node* findMin(BST* tree);
int countNodes(Node* node);
int countLeft(BST* tree);
int countRight(BST* tree);
BST* makeSubTree(Node* subparent);
void removeNode(BST* tree, int data);
void deallocate(BST* tree);
void freeing(Node* root);
int main(){
    BST* tree=createBSTWithParentDataOf(10);
    if(tree!=NULL){
      append(tree,12);
      append(tree,6);
      append(tree,2);
      append(tree,20);
      append(tree,13);
      append(tree,101);
      append(tree,61);
      append(tree,-22);
      append(tree,110);
      append(tree,0);
      append(tree,16);
      printf("contains: %d\n",contains(tree,62));
      Node* n=find(tree,-20);
      if(n!=NULL) printf("found?%d\n",n->data);
      printf("min in this tree is : %d, where max is : %d\n",findMin(tree)->data,findMax(tree)->data);
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

int contains(BST *tree, int data)
{
    Node* current=tree->parent;
    while (current!=NULL)
    {
       if(current->data==data){
          return 1;
       }else if(data>current->data){
        current=current->right;
       }else{
        //data<current.data
        current=current->left;
       }
    }
    return 0;
    
}

Node *find(BST *tree, int data)
{
    Node* current=tree->parent;
    while (current!=NULL)
    {
       if(current->data==data){
          return current;
       }else if(data>current->data){
        current=current->right;
       }else{
        current=current->left;
       }
    }
    printf("No such node with data of %d,you can register it now with \"append(tree,data)\" function\n",data);
    return NULL;
}

Node *findParentOf(BST *tree,Node* node)
{
  Node* parent=tree->parent;
  if(parent==node){
    //it is main parent so it is parentless
    return NULL;
  }else{
    Node* current=tree->parent;
    while (current!=NULL)
    {
        if(current==node&& current->data==node->data){
            return parent;
        }else if(node->data>current->data){
            parent=current;
            current=current->right;
        }else{
            parent=current;
            current=current->left;
        }
    }
    return parent;
  }
}

Node *findMax(BST *tree)
{  
    //max is righ most node always available if not parent is max and min at same time hhh
    Node* current=tree->parent;
    while (current->right!=NULL)
    {
        current=current->right;
    }
    return current;
}

Node *findMin(BST *tree)
{
    //minimum is left most node
    Node* current=tree->parent;
    while(current->left!=NULL){
        current=current->left;
    }
    return current;
}
//count total
int countNodes(Node* node)
{
    if(node == NULL) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}
//count left side
int countLeft(BST* tree)
{
    return countNodes(tree->parent->left);
}
//count right side
int countRight(BST* tree)
{
    return countNodes(tree->parent->right);
}
//make tree that starts from this node
BST *makeSubTree(Node *subparent)
{
    BST* subtree=malloc(sizeof(BST));
    if(subtree!=NULL){
       int count=countNodes(subparent);
       subtree->count=count;
       subtree->parent=subparent;
       return subtree;
    }else{
        return NULL;
    }
}

// remove that node
void removeNode(BST *tree, int data)
{
    Node* node=find(tree,data);
    if(node!=NULL){
      Node* parent=findParentOf(tree,node);
      if(parent==NULL){
        // node is main parent then don't remove it 
        printf("you cant remove main parent of the tree ... atleast for now\n");
        return ;
      }
      //divide this to three cases that i learned from claude for now ... atleast
      //state one: it has no children , free it and stop from its parents pointing it
      if(node->left==NULL&&node->right==NULL){
        if(parent->left==node) parent->left=NULL;
        if(parent->right==node) parent->right=NULL;
        free(node);
      }
      if(node->left!=NULL||node->right!=NULL){//-----------either or both
          //state three: it has ttwo children,
          if(node->left!=NULL&&node->right!=NULL){//------------both
              //--todo remove that node and replace what deserves its position?
              
        }else{//_----either
          //state two: it has one child, move that child its position
           Node* child=node->left==NULL?node->right:node->left;
           if(parent->left==node) parent->left=child;
           if(parent->right==node)parent->right=child;
            free(node);
          }   
      }
      tree->count-=1;
    }else{
     printf("------:so you cant remove what you dont have\n");
     return;
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

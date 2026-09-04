#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entry{
char* key;
char* value;
struct Entry* next;
}Entry;
Entry* createMap(const char* key, const char* value);
void dealocate(Entry* head);
int main(){
    Entry* head=createMap("key1","value1");
    if(head!=NULL){

        dealocate(head);
    }

    return 0;
}

Entry *createMap(const char* key, const char* value)
{
   Entry* head=malloc(sizeof(Entry));
   if(head!=NULL){
        head-> key=malloc(strlen(key)+1);
        head-> value=malloc(strlen(value)+1);
        if(head->key!=NULL &&head-> value!=NULL){
            strcpy(head->key,key);
            strcpy(head->value,value);
            head->next=NULL;
            return head;
        }else{
        perror("it failed to make key or value ");
        free(head->key);
        free(head->value);
        free(head);
        }
        
   }
   return NULL;
}

void dealocate(Entry *head)
{
    Entry* e=head;
    while (e)
    {  
        Entry* next=e->next;
        free(e->key);
        free(e->value);
        free(e);
        e=next;
    }
    
}

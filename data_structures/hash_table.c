#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 16
typedef struct HashMap{
    struct Entry* bucket[TABLE_SIZE];
    struct Entry* top;
}HashMap;
typedef struct Entry{
char* key;
char* value;
struct Entry* next;
}Entry;
unsigned long hash(const char* key);
HashMap* createHashMap();
void set(HashMap* map,const  char* key,const  char* value);
char* get(HashMap* map, const char* key);
void printAll(HashMap* map);
void removeKey(HashMap* map,const char* key);
void dealocate(HashMap* map);
int main(){

    HashMap* hashmap=createHashMap();
    if(hashmap!=NULL){
        
        set(hashmap,"first","first something");
        set(hashmap,"second","second something");
        set(hashmap,"second","duplicate something");
        set(hashmap,"third","third item");
        printf("\'%s\' is stored in third \n",get(hashmap,"third"));
        removeKey(hashmap,"seconds");//to fail
        removeKey(hashmap,"second");

        printAll(hashmap);

        dealocate(hashmap);
    }else{
        printf("it failed to create %d hashmap",TABLE_SIZE);
        exit(1);
    }

    return 0;
}

unsigned long  hash(const  char* key){
    
    unsigned long  value=5381;
    int i=0;
    while (key[i])
    {
       value=((value<<5)+value)+key[i];
       i+=1;
    }
    return value%TABLE_SIZE;
    
}



HashMap *createHashMap()
{
    HashMap* map=malloc(sizeof(HashMap));
    if(map!=NULL){
        for(int k=0;k<TABLE_SIZE;k++){
            map->bucket[k]=NULL;
        }
        map->top=NULL;
        return map;
    }
    return NULL;
}

void set(HashMap *map,const  char *key,const  char *value)
{   

    unsigned long indexAfterHashing=hash(key);
    if(map->bucket[indexAfterHashing]==NULL){
        //enter this item
        Entry* entry=malloc(sizeof(Entry));
        if(entry!=NULL){
            char* keyp=malloc(strlen(key)+1);
            strcpy(keyp,key);
            char* valuep=malloc(strlen(value)+1);
            strcpy(valuep,value);
            entry->key=keyp;
            entry->value=valuep;
            entry->next=NULL;
            map->bucket[indexAfterHashing]=entry;
            if(map->top==NULL){
                map->top=entry;
            }else{
                entry->next=map->top;
                map->top=entry;
            }
            puts("it added new item succesfully");
        }else{
            perror("it failed to allocate entery");
            exit(1);
        }
    }else{
        //this key is registerd already eventhoug jdb2 hash have collisions around 1 in every 77k distinct items
        Entry* existedEntry=map->bucket[indexAfterHashing];
        //check if they are really duplicates
        if(strcmp(existedEntry->key,key)==0){
            //this is updating
            free(existedEntry->value);
            char* valuep=malloc(strlen(value)+1);
            strcpy(valuep,value);
            existedEntry->value=valuep;
            printf("it updated \"%s\" key value\n",existedEntry->key);
        }else{
            //the problem is hashing so
            perror("it has collision in hash jdb2");
        }
    }
}

char *get(HashMap *map, const char *key)
{
    unsigned long index=hash(key);
    Entry* entry= map->bucket[index];
    if(entry!=NULL){
        return entry->value;
    }
    return NULL;
}

void printAll(HashMap *map)
{
  Entry* current=map->top;
  //it is stack form it reads newest first and first one will be last
  puts("----------------HashMap Data-------------");
  while (current!=NULL)
  {
      printf("%s :%s \n",current->key,current->value);
      current=current->next;
    }
  puts("----------------End-------------");
  
    
}
void removeKey(HashMap *map, const char *key)
{
    unsigned long index = hash(key);
    Entry* rmEntry = map->bucket[index];

    if (rmEntry == NULL || strcmp(rmEntry->key, key) != 0) {
        printf("invalid key \"%s\": this key is not present in this map\n", key);
        return;
    }

    Entry** indirect = &map->top;          
    while (*indirect != rmEntry) {
        indirect = &(*indirect)->next;     
    }
    *indirect = rmEntry->next;             

    printf("\"%s\" : \"%s\" (is deleted)\n", rmEntry->key, rmEntry->value);
    free(rmEntry->key);
    free(rmEntry->value);
    free(rmEntry);
    map->bucket[index] = NULL;
}

void dealocate(HashMap *map)
{
    for(int j=0;j<TABLE_SIZE;j++){
     Entry* e=map->bucket[j];
     if(e!=NULL){
        free(e->key);
        free(e->value);
        free(e);
        e=NULL;
     }
    }
    free(map);
}

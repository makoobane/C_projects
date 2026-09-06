#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 16
typedef struct HashMap{
    struct Entry* bucket[TABLE_SIZE];
}HashMap;
typedef struct Entry{
char* key;
char* value;
struct Entry* next;
}Entry;
unsigned long hash(const char* key);
HashMap* createHashMap();
Entry* makeNewEntry( const char* key, const char* value);
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
        set(hashmap,"fourth","fourth item");
        set(hashmap,"fifth","fifth item");
        set(hashmap,"sixth","sixth item");
        printf("\'%s\' is stored in third \n",get(hashmap,"third"));
        printAll(hashmap);
        removeKey(hashmap,"fourth");
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
        return map;
    }
    return NULL;
}
Entry* makeNewEntry(const char* key, const char* value){
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
            return entry;
        }else{
            return NULL;
        }
    }

void set(HashMap *map, const char *key, const char *value)//this one is  from claude
{
    unsigned long index = hash(key);
    Entry* current = map->bucket[index];

    // search the whole chain for an existing match
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            char* valuep = malloc(strlen(value) + 1);
            strcpy(valuep, value);
            current->value = valuep;
            printf("it updated \"%s\" key value\n", current->key);
            return;
        }
        if (current->next == NULL) break;   // stop at the tail
        current = current->next;
    }

    // no match found — append (or set as head, if bucket was empty)
    Entry* new_entry = makeNewEntry(key, value);
    if (new_entry == NULL) {
        perror("it failed to allocate new entry in set");
        return;
    }
    if (current == NULL) {
        map->bucket[index] = new_entry;      
    } else {
        current->next = new_entry;           
        printf("\"%s\" key is branched to \"%s\" key\n", new_entry->key, map->bucket[index]->key);
    }
}

char *get(HashMap *map, const char *key)
{
    unsigned long index=hash(key);
    Entry* entry= map->bucket[index];
    
        while (entry!=NULL)
        {
            if(strcmp(entry->key,key)==0){
                return entry->value;
            }
            entry=entry->next;
        }
    return NULL;
}

void printAll(HashMap *map)
{
  puts("----------------HashMap Data-------------");
  for(int i=0;i<TABLE_SIZE;i++){
    if(map->bucket[i]!=NULL){
        Entry* present=map->bucket[i];
            while (present!=NULL)
            {
                printf("%s : %s \n", present->key,present->value);
                present=present->next;
            }
    }
  }
  puts("----------------End-------------");
  
    
}
// void removeKey(HashMap *map,const char *key)
// {
//     unsigned long index=hash(key);
//     Entry* entry=map->bucket[index];
//     if(strcmp(entry->key,key)==0){
//         // we found it in the top
//         if(entry->next==NULL){
//             free(entry->key);
//             free(entry->value);
//             free(entry);
//             map->bucket[index]=NULL;
//         }else{
//             //it is mother of chain
//             Entry* next=entry->next;
//             free(entry->key);
//             free(entry->value);
//             free(entry);
//             map->bucket[index]=next;

//         }
//         printf("\"%s \" was deleted from top\n",key);
//     }else{
//         // it is in deep
//         if(entry->next==NULL){
//             perror("fetching error occured: why not top and no next??"); return;
//         }
//         Entry* top=entry;
//         entry=entry->next;
//         while(entry!=NULL){
//             if(strcmp(entry->key,key)==0){
//                 // we found it inside
//                 free(entry->key);
//                 free(entry->value);
//                 top->next=entry->next;
//                 free(entry);
//                 printf("\"%s \" was deleted from inside\n",key);
//                 break;
//             }
//             top=entry;
//             entry=entry->next;
//         }


//     }
// }
void removeKey(HashMap *map, const char *key)
{
    unsigned long index = hash(key);
    Entry* entry = map->bucket[index];

    if (entry == NULL) {
        printf("\"%s\" not found: bucket is empty\n", key);
        return;
    }

    if (strcmp(entry->key, key) == 0) {
        map->bucket[index] = entry->next;   // works whether next is NULL or not — no need to branch on it
        free(entry->key);
        free(entry->value);
        free(entry);
        printf("\"%s\" was deleted from top\n", key);
        return;
    }

    Entry* top = entry;
    entry = entry->next;
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            top->next = entry->next;
            free(entry->key);
            free(entry->value);
            free(entry);
            printf("\"%s\" was deleted from inside\n", key);
            return;
        }
        top = entry;
        entry = entry->next;
    }
    printf("\"%s\" not found in this chain\n", key);   // bug 2, see below
}
void dealocate(HashMap *map)
{
    for(int j=0;j<TABLE_SIZE;j++){
     Entry* e=map->bucket[j];
        while(e!=NULL){
            Entry* next=e->next;
            free(e->key);
            free(e->value);
            free(e);
            e=next;
     }
    }
    free(map);
}

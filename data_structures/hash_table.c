#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct HashMap{
    struct Entry** bucket;
    int capacity;
    int count;
}HashMap;
typedef struct Entry{
char* key;
char* value;
struct Entry* next;
}Entry;
unsigned long hash(const char* key,int capacity);
HashMap* createHashMap(int capacity);
Entry* makeNewEntry( const char* key, const char* value);
void set(HashMap* map,const  char* key,const  char* value);
char* get(HashMap* map, const char* key);
void printAll(HashMap* map);
void removeKey(HashMap* map,const char* key);
void migrate(Entry* entry,Entry** newBucket,int newCapacity);
void resize(HashMap* map,int newCapacity);
void dealocate(HashMap* map);
int main(){

    HashMap* hashmap=createHashMap(4);
    if(hashmap!=NULL){
        
        set(hashmap,"first","first something");
        set(hashmap,"second","second something");
        set(hashmap,"second","duplicate something");
        set(hashmap,"third","third item");
        set(hashmap,"fourth","fourth item");
        printf("\'%s\' is stored in third \n",get(hashmap,"five"));
        set(hashmap,"fifth","fifth item");
        set(hashmap,"sixth","6 item");
        set(hashmap,"seventh","7 item");
        set(hashmap,"eight","8 item");
        set(hashmap,"ninth","9 item");
        printf("\'%s\' is stored in third \n",get(hashmap,"third"));
        printAll(hashmap);
        removeKey(hashmap,"fourth");
        removeKey(hashmap,"second");
        removeKey(hashmap,"seventh");
        removeKey(hashmap,"seventh");// try if it is still there
        removeKey(hashmap,"eight");
        removeKey(hashmap,"ninth");
        printAll(hashmap);
        dealocate(hashmap);
    }else{
        puts("it failed to create that hashmap");
        exit(1);
    }

    return 0;
}

unsigned long  hash(const  char* key,int capacity){
    
    unsigned long  value=5381;
    int i=0;
    while (key[i])
    {
       value=((value<<5)+value)+key[i];
       i+=1;
    }
    return value%capacity;
    
}



HashMap *createHashMap(int capacity)
{
    HashMap* map=malloc(sizeof(HashMap));
    if(map!=NULL){
        map->bucket=calloc(capacity,sizeof(Entry*));
       if(map->bucket!=NULL){
         map->capacity=capacity;
        map->count=0;
        return map;
       }else{
        perror("it failed to allocate bucket");
        return NULL;
       }
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
    unsigned long index = hash(key,map->capacity);
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
        map->count+=1;  
      if((float)(map->count + 1) / map->capacity > 0.7){
            int newSize=2*map->capacity;
            resize(map,newSize);
        }
    } else {
        current->next = new_entry;           
        printf("\"%s\" key is branched to \"%s\" key\n", new_entry->key, map->bucket[index]->key);
        map->count+=1;
       if((float)(map->count + 1) / map->capacity > 0.7){
            int newSize=2*map->capacity;
            resize(map,newSize);
        }
    }
}

char *get(HashMap *map, const char *key)
{
    unsigned long index=hash(key,map->capacity);
    Entry* entry= map->bucket[index];
    
        while (entry!=NULL)
        {
            if(strcmp(entry->key,key)==0){
                return entry->value;
            }
            entry=entry->next;
        }
    printf("\" %s\" this key is not present now in this map",key);
    return NULL;
}

void printAll(HashMap *map)
{
  puts("----------------HashMap Data-------------");
  for(int i=0;i<map->capacity;i++){
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

void removeKey(HashMap *map, const char *key)
{
    unsigned long index = hash(key,map->capacity);
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
        map->count-=1;
        if((float)(map->count) / map->capacity<0.3){
            int newSize= map->capacity /2;
            if (newSize < 4) return;   // pick some sensible floor, don't shrink below it
            resize(map,newSize);
        }
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
            map->count-=1;
            if((float)(map->count) / map->capacity<0.3){
            int newSize= map->capacity /2;
            if (newSize < 4) return;   // pick some sensible floor, don't shrink below it
            resize(map,newSize);
        }
            return;
        }
        top = entry;
        entry = entry->next;
    }
    printf("\"%s\" not found in this chain\n", key);   // bug 2, see below
}
void resize(HashMap *map, int newCapacity)
{
    Entry** newBucket = calloc(newCapacity, sizeof(Entry*));
    if (newBucket == NULL) { perror("resize failed"); exit(1); }

    for (int i = 0; i < map->capacity; i++) {
        Entry* entry = map->bucket[i];
        while (entry != NULL) {
            Entry* next = entry->next;   // save BEFORE migrate mutates entry->next
            migrate(entry, newBucket, newCapacity);
            entry = next;
        }
    }
    free(map->bucket);
    map->bucket = newBucket;
    map->capacity = newCapacity;
    printf("it was resized\n");
}

void migrate(Entry *entry, Entry **newBucket, int newCapacity)
{
    entry->next = NULL;   // detach from the old chain before relinking
    unsigned long newIndex = hash(entry->key, newCapacity);
    Entry* head = newBucket[newIndex];
    if (head == NULL) {
        newBucket[newIndex] = entry;
    } else {
        Entry* last = head;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = entry;
    }
}
void dealocate(HashMap *map)
{
    for(int j=0;j<map->capacity;j++){
     Entry* e=map->bucket[j];
        while(e!=NULL){
            Entry* next=e->next;
            free(e->key);
            free(e->value);
            free(e);
            e=next;
     }
    }
    free(map->bucket);
    free(map);
}
